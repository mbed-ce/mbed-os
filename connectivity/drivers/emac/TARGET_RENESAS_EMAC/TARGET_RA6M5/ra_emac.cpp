#include "ra_emac.h"
#include "r_ioport.h"
#include "rtos/ThisThread.h"

/* The EK-RA6M5 carries a KSZ8091RNB PHY at MDIO address 0, driven through
 * the FSP r_ether_phy built-in target support (no custom PHY hooks needed). */

ether_instance_ctrl_t RA_EMAC::s_ether_ctrl;
ether_phy_instance_ctrl_t RA_EMAC::s_phy_ctrl;
ether_instance_descriptor_t RA_EMAC::s_rx_descriptors[4] __attribute__((aligned(16)));
ether_instance_descriptor_t RA_EMAC::s_tx_descriptors[4] __attribute__((aligned(16)));
uint8_t RA_EMAC::s_ether_buffers[8][1536] __attribute__((aligned(32)));
uint8_t *RA_EMAC::s_pp_ether_buffers[8] =
{
    s_ether_buffers[0], s_ether_buffers[1], s_ether_buffers[2], s_ether_buffers[3],
    s_ether_buffers[4], s_ether_buffers[5], s_ether_buffers[6], s_ether_buffers[7],
};

static const ether_phy_lsi_cfg_t s_phy_lsi_cfg =
{
    .address = 0,
    .type = ETHER_PHY_LSI_TYPE_KSZ8091RNB,
};

static const ether_phy_extended_cfg_t s_phy_extended_cfg =
{
    .p_target_init = NULL,
    .p_target_link_partner_ability_get = NULL,
    .p_phy_lsi_cfg_list = { &s_phy_lsi_cfg },
    .default_phy_lsi_cfg_index = 0,
};

static const ether_phy_cfg_t s_phy_cfg =
{
    .channel = 0,
    .phy_lsi_address = 0,
    .phy_reset_wait_time = 0x00020000,
    /* Busy-loop count per MDIO half-cycle: ~8 loop iterations at 200 MHz
     * ICLK clock MDC at ~15 MHz, way above the 2.5 MHz MDIO limit - the
     * KSZ8091RNB ignores the transfer. 100 iterations keep MDC around
     * 1 MHz. */
    .mii_bit_access_wait_time = 100,
    .phy_lsi_type = ETHER_PHY_LSI_TYPE_KSZ8091RNB,
    .flow_control = ETHER_PHY_FLOW_CONTROL_DISABLE,
    .mii_type = ETHER_PHY_MII_TYPE_RMII,
    .p_extend = &s_phy_extended_cfg,
};

static const ether_phy_instance_t s_phy_instance =
{
    .p_ctrl = &RA_EMAC::s_phy_ctrl,
    .p_cfg = &s_phy_cfg,
    .p_api = &g_ether_phy_on_ether_phy,
};

static const ether_extended_cfg_t s_ether_extended_cfg =
{
    .p_rx_descriptors = RA_EMAC::s_rx_descriptors,
    .p_tx_descriptors = RA_EMAC::s_tx_descriptors,
    .eesr_event_filter = 0x0007800F,
    .ecsr_event_filter = 0,
};

static uint8_t s_mac_address[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

static const ether_cfg_t s_ether_cfg =
{
    .channel = 0,
    .zerocopy = ETHER_ZEROCOPY_DISABLE,
    .multicast = ETHER_MULTICAST_ENABLE,
    .promiscuous = ETHER_PROMISCUOUS_DISABLE,
    .flow_control = ETHER_FLOW_CONTROL_DISABLE,
    .padding = ETHER_PADDING_DISABLE,
    .padding_offset = 0,
    .broadcast_filter = 0,
    .p_mac_address = s_mac_address,
    .num_tx_descriptors = 4,
    .num_rx_descriptors = 4,
    .pp_ether_buffers = RA_EMAC::s_pp_ether_buffers,
    .ether_buffer_size = 1536,
    .irq = VECTOR_NUMBER_EDMAC0_EINT,
    .interrupt_priority = 12,
    .p_callback = RA_EMAC::ether_isr_cb,
    .p_ether_phy_instance = &s_phy_instance,
    .p_context = nullptr,
    .p_extend = &s_ether_extended_cfg,
};

static const ether_instance_t s_ether_instance =
{
    .p_ctrl = &RA_EMAC::s_ether_ctrl,
    .p_cfg = &s_ether_cfg,
    .p_api = &g_ether_on_ether,
};

RA_EMAC::RA_EMAC()
    : _memory_manager(nullptr), _rx_sem(0, 0xFFFFFFFF), _rx_thread(nullptr), _phy_thread(nullptr)
{
    _link_state = false;
    _powered_up = false;
    memset(_mac_addr, 0, sizeof(_mac_addr));
}

bool RA_EMAC::get_hwaddr(uint8_t *addr) const
{
    memcpy(addr, s_mac_address, 6);
    return true;
}

void RA_EMAC::set_hwaddr(const uint8_t *addr)
{
    memcpy(s_mac_address, addr, 6);
}

void RA_EMAC::configure_pins()
{
    /* PHY nRST on P4_3: hold high. Do NOT pulse it low - strap options
     * latch on the rising edge and pulsing reset while RMII pins float
     * may corrupt them (official EK example just drives it high). */
    R_IOPORT_PinCfg(NULL, BSP_IO_PORT_04_PIN_03,
                     IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_DRIVE_HIGH);
    R_IOPORT_PinWrite(NULL, BSP_IO_PORT_04_PIN_03, BSP_IO_LEVEL_HIGH);
    rtos::ThisThread::sleep_for(std::chrono::milliseconds(10));

    /* RMII signal pins: MDC/MDIO on P4, TX_EN/TXD1 on P4, TXD0/REF50CK/
     * RXD0/RXD1/RX_ER/CRS_DV on P7 (EK-RA6M5 routing) */
    const bsp_io_port_pin_t eth_pins[] =
    {
        BSP_IO_PORT_04_PIN_01,  /* MDC */
        BSP_IO_PORT_04_PIN_02,  /* MDIO */
        BSP_IO_PORT_04_PIN_05,  /* TX_EN */
        BSP_IO_PORT_04_PIN_06,  /* TXD1 */
        BSP_IO_PORT_07_PIN_00,  /* TXD0 */
        BSP_IO_PORT_07_PIN_01,  /* REF50CK */
        BSP_IO_PORT_07_PIN_02,  /* RXD0 */
        BSP_IO_PORT_07_PIN_03,  /* RXD1 */
        BSP_IO_PORT_07_PIN_04,  /* RX_ER */
        BSP_IO_PORT_07_PIN_05,  /* CRS_DV */
    };

    for(size_t i = 0; i < sizeof(eth_pins) / sizeof(eth_pins[0]); i++)
    {
        R_IOPORT_PinCfg(NULL, eth_pins[i],
                        IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN |
                        IOPORT_PERIPHERAL_ETHER_RMII);
    }
}

bool RA_EMAC::power_up()
{
    if(_powered_up) return true;

    configure_pins();

    fsp_err_t err = R_ETHER_Open(&s_ether_ctrl, &s_ether_cfg);
    if(err != FSP_SUCCESS)
    {
        printf("[ETH] R_ETHER_Open failed: %d\r\n", (int)err);
        R_ETHER_Close(&s_ether_ctrl);
        return false;
    }

    /* Note: FSP initializes descriptors lazily via ether_configure_mac()
     * called when the link comes up in R_ETHER_LinkProcess().
     * RX requires link up first. */

    _rx_thread = new rtos::Thread(osPriorityAboveNormal, 2048, nullptr, "eth_rx");
    _rx_thread->start(mbed::callback(this, &RA_EMAC::rx_thread));

    /* The PHY/link poller must run even while the application blocks inside
     * EMACInterface::connect(); a dedicated thread is used instead of the
     * shared event queue, which is only dispatched by the application when
     * events.shared-dispatch-from-application is set. */
    _phy_thread = new rtos::Thread(osPriorityNormal, 1024, nullptr, "eth_phy");
    _phy_thread->start([this]() {
        while(true)
        {
            rtos::ThisThread::sleep_for(std::chrono::milliseconds(200));
            phy_poll();
        }
    });

    _powered_up = true;
    return true;
}

void RA_EMAC::power_down()
{
    if(!_powered_up) return;
    R_ETHER_Close(&s_ether_ctrl);
    if(_rx_thread)
    {
        _rx_thread->terminate();
        delete _rx_thread;
        _rx_thread = nullptr;
    }
    if(_phy_thread)
    {
        _phy_thread->terminate();
        delete _phy_thread;
        _phy_thread = nullptr;
    }
    _powered_up = false;
}

void RA_EMAC::ether_callback(ether_callback_args_t *p_args)
{
    if(p_args->event == ETHER_EVENT_RX_COMPLETE || p_args->event == ETHER_EVENT_RX_MESSAGE_LOST)
    {
        RA_EMAC &self = get_instance();
        self._rx_sem.release();
    }
}

void RA_EMAC::rx_thread()
{
    /* R_ETHER_Read (zerocopy disabled) memcpy's into p_buffer, which must be
     * 32-byte aligned and at least ether_buffer_size bytes. */
    static uint8_t read_buf[1536] __attribute__((aligned(32)));

    while(true)
    {
        _rx_sem.acquire();

        /* Always drain and release received frames, regardless of the
         * link state reported to lwip: dropping the semaphore without
         * releasing the descriptor leaves the RX ring full and the
         * EDMAC receive DMA (EDRRR) stopped for good. */
        if(s_ether_ctrl.p_rx_descriptor == nullptr ||
           s_rx_descriptors[0].p_buffer == nullptr)
        {
            continue;
        }

        uint32_t rx_len = 0;
        fsp_err_t read_err = R_ETHER_Read(&s_ether_ctrl, read_buf, &rx_len);
        while(read_err == FSP_SUCCESS && rx_len > 0)
        {
            if(rx_len > 1536)
            {
                rx_len = 1536;
            }
            if(_memory_manager != nullptr && _emac_input_data_cb)
            {
                emac_mem_buf_t *p = _memory_manager->alloc_pool(rx_len, 0);
                if(p != nullptr)
                {
                    _memory_manager->copy_to_buf(p, read_buf, rx_len);
                    _emac_input_data_cb(p);
                }
            }
            R_ETHER_BufferRelease(&s_ether_ctrl);
            rx_len = 0;
            read_err = R_ETHER_Read(&s_ether_ctrl, read_buf, &rx_len);
        }
    }
}

void RA_EMAC::phy_poll()
{
    fsp_err_t err = R_ETHER_LinkProcess(&s_ether_ctrl);
    if(err != FSP_SUCCESS) return;

    bool new_state = (s_ether_ctrl.link_establish_status == ETHER_LINK_ESTABLISH_STATUS_UP);
    if(new_state != _link_state)
    {
        printf("[ETH] link %s\r\n", new_state ? "UP" : "DOWN");
        _link_state = new_state;
        if(_emac_link_state_cb)
        {
            _emac_link_state_cb(new_state);
        }
    }
}

bool RA_EMAC::link_out(emac_mem_buf_t *buf)
{
    if(buf == nullptr || _memory_manager == nullptr || !_powered_up)
    {
        return false;
    }

    static uint8_t frame[1536];
    uint32_t offset = 0;
    emac_mem_buf_t *chain = buf;

    while(chain != nullptr && offset < sizeof(frame))
    {
        uint8_t *ptr = (uint8_t *)_memory_manager->get_ptr(chain);
        uint32_t len = _memory_manager->get_len(chain);
        if(offset + len > sizeof(frame))
        {
            len = sizeof(frame) - offset;
        }
        if(ptr != nullptr && len > 0)
        {
            memcpy(frame + offset, ptr, len);
        }
        offset += len;
        chain = _memory_manager->get_next(chain);
    }

    _memory_manager->free(buf);

    if(offset < 60)
    {
        memset(frame + offset, 0, 60 - offset);
        offset = 60;
    }

    fsp_err_t err = R_ETHER_Write(&s_ether_ctrl, frame, offset);
    return err == FSP_SUCCESS;
}

/* Weak override: register as the default EMAC */
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return RA_EMAC::get_instance();
}

#include "ra_emac.h"
#include "r_ioport.h"
#include "rtos/ThisThread.h"
#include "events/mbed_shared_queues.h"

/* RTL8201F PHY target functions (custom PHY interface) */
#define RTL8201F_REG_BMCR       (0x00)
#define RTL8201F_REG_BMSR       (0x01)
#define RTL8201F_REG_ANAR       (0x04)
#define RTL8201F_REG_ANLPAR     (0x05)
#define RTL8201F_REG_PHYCTRL    (0x16)
#define RTL8201F_BMCR_RESET     (0x8000)
#define RTL8201F_RMII_MODE      (0x0100)

static void rtl8201f_initialize(ether_phy_instance_ctrl_t *ctrl)
{
    uint32_t reg;

    R_ETHER_PHY_Write(ctrl, RTL8201F_REG_BMCR, RTL8201F_BMCR_RESET);
    uint32_t timeout = 100000;
    do
    {
        R_ETHER_PHY_Read(ctrl, RTL8201F_REG_BMCR, &reg);
        timeout--;
    }
    while((reg & RTL8201F_BMCR_RESET) && (timeout > 0));

    R_ETHER_PHY_Read(ctrl, RTL8201F_REG_PHYCTRL, &reg);
    reg |= RTL8201F_RMII_MODE;
    R_ETHER_PHY_Write(ctrl, RTL8201F_REG_PHYCTRL, reg);

    R_ETHER_PHY_Write(ctrl, RTL8201F_REG_ANAR,
                      (0x01 | 0x0040 | 0x0020 | 0x0010 | 0x0008 | 0x0004 | 0x0002 | 0x0001));
}

static bool rtl8201f_link_partner_ability(ether_phy_instance_ctrl_t *ctrl, uint32_t speed_duplex)
{
    uint32_t lpar = 0;
    R_ETHER_PHY_Read(ctrl, RTL8201F_REG_ANLPAR, &lpar);
    switch(speed_duplex)
    {
        case 0x00000006: return (lpar & 0x0100) != 0;
        case 0x00000004: return (lpar & 0x0080) != 0;
        case 0x00000002: return (lpar & 0x0040) != 0;
        case 0x00000000: return (lpar & 0x0020) != 0;
        default: return false;
    }
}

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
    .type = ETHER_PHY_LSI_TYPE_CUSTOM,
};

static const ether_phy_extended_cfg_t s_phy_extended_cfg =
{
    .p_target_init = rtl8201f_initialize,
    .p_target_link_partner_ability_get = rtl8201f_link_partner_ability,
    .p_phy_lsi_cfg_list = { &s_phy_lsi_cfg },
    .default_phy_lsi_cfg_index = 0,
};

static const ether_phy_cfg_t s_phy_cfg =
{
    .channel = 0,
    .phy_lsi_address = 0,
    .phy_reset_wait_time = 0x00020000,
    .mii_bit_access_wait_time = 8,
    .phy_lsi_type = ETHER_PHY_LSI_TYPE_CUSTOM,
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
    : _memory_manager(nullptr), _rx_sem(0, 0xFFFFFFFF), _rx_thread(nullptr)
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
    /* PHY reset: P4_0 as plain GPIO output (NOT Ethernet peripheral!) */
    R_IOPORT_PinCfg(NULL, BSP_IO_PORT_04_PIN_00,
                     IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_DRIVE_HIGH);
    bsp_io_port_pin_t reset_pin = BSP_IO_PORT_04_PIN_00;
    R_IOPORT_PinWrite(NULL, reset_pin, BSP_IO_LEVEL_LOW);
    rtos::ThisThread::sleep_for(std::chrono::milliseconds(100));
    R_IOPORT_PinWrite(NULL, reset_pin, BSP_IO_LEVEL_HIGH);
    rtos::ThisThread::sleep_for(std::chrono::milliseconds(100));

    /* RMII signal pins */
    const bsp_io_port_pin_t eth_pins[] =
    {
        BSP_IO_PORT_04_PIN_01,  /* MDC */
        BSP_IO_PORT_04_PIN_02,  /* MDIO */
        BSP_IO_PORT_04_PIN_08,  /* CRS_DV */
        BSP_IO_PORT_04_PIN_09,  /* RX_ER */
        BSP_IO_PORT_04_PIN_10,  /* RXD1 */
        BSP_IO_PORT_04_PIN_11,  /* RXD0 */
        BSP_IO_PORT_04_PIN_12,  /* REF50CK */
        BSP_IO_PORT_04_PIN_13,  /* TXD0 */
        BSP_IO_PORT_04_PIN_14,  /* TXD1 */
        BSP_IO_PORT_04_PIN_15,  /* TX_EN */
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
        // printf("[ETH] R_ETHER_Open failed: %d\r\n", (int)err);
        R_ETHER_Close(&s_ether_ctrl);
        return false;
    }

    /* Note: FSP initializes descriptors lazily via ether_configure_mac()
     * called when the link comes up in R_ETHER_LinkProcess().
     * RX requires link up first. */

    /* Debug: check descriptor init and EDMAC registers */
    /*{
        printf("[ETH] ctrl.open=%08X p_rx_desc=%p p_tx_desc=%p\r\n",
               (unsigned)s_ether_ctrl.open,
               (void *)s_ether_ctrl.p_rx_descriptor,
               (void *)s_ether_ctrl.p_tx_descriptor);
        printf("[ETH] rx_desc[0]: st=%08X sz=%u bs=%u buf=%p next=%p\r\n",
               (unsigned)s_rx_descriptors[0].status,
               s_rx_descriptors[0].size,
               s_rx_descriptors[0].buffer_size,
               s_rx_descriptors[0].p_buffer,
               s_rx_descriptors[0].p_next);
        if(s_ether_ctrl.p_reg_edmac != nullptr)
        {
            volatile uint32_t *edmac = (volatile uint32_t *)s_ether_ctrl.p_reg_edmac;
            printf("[ETH] EDMAC regs: %08X %08X %08X %08X\r\n",
                   (unsigned)edmac[0], (unsigned)edmac[1],
                   (unsigned)edmac[8], (unsigned)edmac[9]);
        }
        uint32_t id1 = 0, id2 = 0;
        R_ETHER_PHY_Read(&s_phy_ctrl, 0x02, &id1);
        R_ETHER_PHY_Read(&s_phy_ctrl, 0x03, &id2);
        printf("[ETH] PHY ID: %04X %04X\r\n", (unsigned)id1, (unsigned)id2);
        uint32_t bmcr = 0, bmsr = 0;
        R_ETHER_PHY_Read(&s_phy_ctrl, 0x00, &bmcr);
        R_ETHER_PHY_Read(&s_phy_ctrl, 0x01, &bmsr);
        printf("[ETH] BMCR=%04X BMSR=%04X (link=%d aneg=%d)\r\n",
               (unsigned)bmcr, (unsigned)bmsr,
               (bmsr & 4) ? 1 : 0, (bmsr & 0x20) ? 1 : 0);
    }*/

    _rx_thread = new rtos::Thread(osPriorityAboveNormal, 2048, nullptr, "eth_rx");
    _rx_thread->start(mbed::callback(this, &RA_EMAC::rx_thread));

    mbed::mbed_event_queue()->call_every(200, this, &RA_EMAC::phy_poll);

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
    _powered_up = false;
}

void RA_EMAC::ether_callback(ether_callback_args_t *p_args)
{
    static int isr_count = 0;
    if(isr_count < 10)
    {
        /* Read EESR to see raw interrupt source */
        // volatile uint32_t eesr = ((volatile uint32_t *)0x400C402C)[0]; /* EDMAC EESR */
        // printf("[ETH] ISR evt=%d eesr=%08X\r\n", (int)p_args->event, (unsigned)eesr);
        isr_count++;
    }
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

        if(!_link_state)
        {
            continue;
        }

        /* Verify descriptors were initialized by ether_configure_mac() */
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
        // printf("[ETH] link %s\r\n", new_state ? "UP" : "DOWN");
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
    // printf("[ETH] TX %u bytes: %s\r\n", offset, err == FSP_SUCCESS ? "OK" : "FAIL");
    return err == FSP_SUCCESS;
}

/* Weak override: register as the default EMAC */
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return RA_EMAC::get_instance();
}

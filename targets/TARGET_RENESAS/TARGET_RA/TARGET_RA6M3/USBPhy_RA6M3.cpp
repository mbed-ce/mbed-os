/* mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited, All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* USB device (peripheral) driver for the Renesas RA6M3 USB_FS0 module.
 *
 * Ported from the register level USBPhy implementations of the RZ/A2
 * (targets/TARGET_RENESAS/TARGET_RZ_A2XX) which drive the same Renesas
 * pipe based USB 2.0 IP, adapted to the RA6M3 register layout:
 *  - USB_FS0 at 0x40090000, module stop bit MSTPB11, 48 MHz UCLK is
 *    configured by the BSP clock initialisation (SCKDIVCR2).
 *  - 10 pipes (DCP + 9). Pipes 1/2: 256 byte buffers, pipes 3-9: 64 byte
 *    buffers. No PIPEBUF register, buffer mapping is fixed.
 *  - Single combined USBFS_INT interrupt event (no separate FIFO IRQs
 *    needed, all FIFO access is done from the CFIFO by the CPU).
 *
 * Reference: Renesas RA6M3 Group User's Manual: Hardware (R01UH0886EJ),
 * chapter "USB 2.0 Module" and the FSP r_usb_basic driver in
 * ra/fsp/src/r_usb_basic used for the module bring-up sequence.
 */

#if defined(DEVICE_USBDEVICE) && DEVICE_USBDEVICE

extern "C" {
#include "bsp_api.h"
#include "vector_data.h"
}

#include "USBPhyHw.h"
#include <string.h>

/* ---------------------------------------------------------------------------
 * Register bit definitions (USB_FS0)
 * ------------------------------------------------------------------------- */

/* System Configuration Control Register */
#define USB_SCKE                (0x0400u)   /* b10: USB clock enable */
#define USB_CNEN                (0x0100u)   /* b8: Single end receiver enable */
#define USB_DCFM                (0x0040u)   /* b6: Controller function select */
#define USB_DRPD                (0x0020u)   /* b5: D+/D- pull down control */
#define USB_DPRPU               (0x0010u)   /* b4: D+ pull up control */
#define USB_DMRPU               (0x0008u)   /* b3: D- pull up control */
#define USB_USBE                (0x0001u)   /* b0: USB module enable */

/* CFIFO Port Select Register */
#define USB_RCNT                (0x8000u)   /* b15: Read count mode */
#define USB_REW                 (0x4000u)   /* b14: Buffer rewind */
#define USB_MBW                 (0x0C00u)   /* b11-10: FIFO access bit width */
#define USB_MBW_32              (0x0800u)   /* FIFO access : 32bit (USBHS module only) */
#define USB_MBW_16              (0x0400u)   /* FIFO access : 16bit */
#define USB_MBW_8               (0x0000u)   /* FIFO access : 8bit */

/* The USB_FS0 module has a 16-bit CPU bus interface: the FIFO ports must
 * be accessed with MBW=16 (32-bit accesses are only supported by the
 * USBHS module). This mirrors the FSP driver where USB0_CFIFO_MBW is
 * USB_MBW_16. */
#define USB0_FIFO_MBW           USB_MBW_16
#define USB_BIGEND              (0x0100u)   /* b8: Big endian mode */
#define USB_ISEL                (0x0020u)   /* b5: DCP FIFO port direction select */
#define USB_ISEL_WRITE          (0x0020u)   /* write */
#define USB_ISEL_READ           (0x0000u)   /* read */
#define USB_CURPIPE             (0x000Fu)   /* b3-0: PIPE select */

/* CFIFO Port Control Register */
#define USB_BVAL                (0x8000u)   /* b15: Buffer valid flag */
#define USB_BCLR                (0x4000u)   /* b14: Buffer clear */
#define USB_FRDY                (0x2000u)   /* b13: FIFO ready */
#define USB_DTLN                (0x0FFFu)   /* b11-0: FIFO data length */

/* Interrupt Enable Register 0 */
#define USB_VBSE                (0x8000u)   /* b15: VBUS interrupt */
#define USB_RSME                (0x4000u)   /* b14: Resume interrupt */
#define USB_SOFE                (0x2000u)   /* b13: Frame update interrupt */
#define USB_DVSE                (0x1000u)   /* b12: Device state transition interrupt */
#define USB_CTRE                (0x0800u)   /* b11: Control transfer stage transition interrupt */
#define USB_BEMPE               (0x0400u)   /* b10: Buffer empty interrupt */
#define USB_NRDYE               (0x0200u)   /* b9: Buffer not ready interrupt */
#define USB_BRDYE               (0x0100u)   /* b8: Buffer ready interrupt */

/* Device State Control Register */
#define USB_RESUME              (0x0020u)   /* b5: Resume output */

/* Interrupt Status Register 0 */
#define USB_VBINT               (0x8000u)   /* b15: VBUS interrupt */
#define USB_RESM                (0x4000u)   /* b14: Resume interrupt */
#define USB_SOFR                (0x2000u)   /* b13: SOF update interrupt */
#define USB_DVST                (0x1000u)   /* b12: Device state transition interrupt */
#define USB_CTRT                (0x0800u)   /* b11: Control transfer stage transition interrupt */
#define USB_BEMP                (0x0400u)   /* b10: Buffer empty interrupt */
#define USB_NRDY                (0x0200u)   /* b9: Buffer not ready interrupt */
#define USB_BRDY                (0x0100u)   /* b8: Buffer ready interrupt */
#define USB_VBSTS               (0x0080u)   /* b7: VBUS input port */
#define USB_DVSQ                (0x0070u)   /* b6-4: Device state */
#define USB_DS_SPD_CNFG         (0x0070u)   /* Suspend Configured */
#define USB_DS_SPD_ADDR         (0x0060u)   /* Suspend Address */
#define USB_DS_SPD_DFLT         (0x0050u)   /* Suspend Default */
#define USB_DS_SPD_POWR         (0x0040u)   /* Suspend Powered */
#define USB_DS_CNFG             (0x0030u)   /* Configured */
#define USB_DS_ADDS             (0x0020u)   /* Address */
#define USB_DS_DFLT             (0x0010u)   /* Default */
#define USB_DS_POWR             (0x0000u)   /* Powered */
#define USB_VALID               (0x0008u)   /* b3: Setup packet detect flag */
#define USB_CTSQ                (0x0007u)   /* b2-0: Control transfer stage */
#define USB_CS_SQER             (0x0006u)   /* Sequence error */
#define USB_CS_WRND             (0x0005u)   /* Ctrl write nodata status stage */
#define USB_CS_WRSS             (0x0004u)   /* Ctrl write status stage */
#define USB_CS_WRDS             (0x0003u)   /* Ctrl write data stage */
#define USB_CS_RDSS             (0x0002u)   /* Ctrl read status stage */
#define USB_CS_RDDS             (0x0001u)   /* Ctrl read data stage */
#define USB_CS_IDST             (0x0000u)   /* Idle or setup stage */

/* USB IO Register Reserved bit masks */
#define BRDYSTS_MASK            (0x03FFu)
#define NRDYSTS_MASK            (0x03FFu)
#define BEMPSTS_MASK            (0x03FFu)

/* Pipe Configuration Register */
#define USB_TYPFIELD            (0xC000u)   /* b15-14: Transfer type */
#define USB_TYPFIELD_ISO        (0xC000u)   /* Isochronous */
#define USB_TYPFIELD_INT        (0x8000u)   /* Interrupt */
#define USB_TYPFIELD_BULK       (0x4000u)   /* Bulk */
#define USB_BFREOFF             (0x0000u)   /* b10: BRDY interrupt mode off */
#define USB_CFG_DBLBOFF         (0x0000u)   /* b9: double buffer off */
#define USB_CFG_CNTMDOFF        (0x0000u)   /* b8: continuous transfer mode off */
#define USB_DIRFIELD            (0x0010u)   /* b4: Transfer direction select */
#define USB_DIR_P_IN            (0x0010u)   /* PERI IN (buffer -> FIFO) */
#define USB_DIR_P_OUT           (0x0000u)   /* PERI OUT (FIFO -> buffer) */
#define USB_EPNUMFIELD          (0x000Fu)   /* b3-0: Endpoint number select */

/* Pipe Maximum Packet Size Register */
#define USB_MXPS                (0x07FFu)   /* b10-0: Maxpacket size */

/* DCP/Pipe Control Register */
#define USB_BSTS                (0x8000u)   /* b15: Buffer status */
#define USB_SUREQ               (0x4000u)   /* b14: Send USB request */
#define USB_INBUFM              (0x4000u)   /* b14: IN buffer monitor (PIPE1-5) */
#define USB_SUREQCLR            (0x0800u)   /* b11: stop setup request */
#define USB_ACLRM               (0x0200u)   /* b9: buffer auto clear mode */
#define USB_SQCLR               (0x0100u)   /* b8: Sequence bit clear */
#define USB_SQSET               (0x0080u)   /* b7: Sequence bit set */
#define USB_SQMON               (0x0040u)   /* b6: Sequence bit monitor */
#define USB_PBUSY               (0x0020u)   /* b5: pipe busy */
#define USB_CCPL                (0x0004u)   /* b2: Enable control transfer complete */
#define USB_PID                 (0x0003u)   /* b1-0: Response PID */
#define USB_PID_STALL2          (0x0003u)   /* STALL2 */
#define USB_PID_STALL           (0x0002u)   /* STALL */
#define USB_PID_BUF             (0x0001u)   /* BUF */
#define USB_PID_NAK             (0x0000u)   /* NAK */

/* DCP Maximum Packet Size Register */
#define USB_MAXP                (0x007Fu)   /* b6-0: Maxpacket size of DCP */

/* Pipe Transaction Counter Register */
#define USB_TRENB               (0x0200u)   /* b9: Transaction count enable */
#define USB_TRCLR               (0x0100u)   /* b8: Transaction count clear */

/* FIFO read / write result */
#define USB_FIFOERROR           (0x00ffu)   /* FIFO not ready */
#define USB_WRITEEND            (0x0000u)   /* End of write */
#define USB_WRITESHRT           (0x0001u)   /* End of write (short packet) */
#define USB_WRITING             (0x0002u)   /* Write continues */
#define USB_READEND             (0x0000u)   /* End of read */
#define USB_READSHRT            (0x0001u)   /* End of read (short packet) */
#define USB_READING             (0x0002u)   /* Read continues */
#define USB_READOVER            (0x0003u)   /* Buffer size over */

/* Transfer status */
#define USB_DATA_NONE           (1u)
#define USB_DATA_OK             (3u)
#define USB_DATA_SHT            (4u)
#define USB_DATA_OVR            (5u)
#define USB_DATA_STALL          (6u)
#define USB_DATA_ERR            (7u)
#define USB_DATA_READING        (19u)
#define USB_DATA_WRITING        (20u)

/* Pipe numbers */
#define USB_PIPE0               (0x0u)

#define USB_BITSET(x)           ((uint16_t)((uint16_t)1 << (x)))

/* Endpoint 0 maximum packet size (full-speed) */
#define MAX_PACKET_SIZE_EP0     (64u)

/* PHY cross point adjustment (reset value recommended by BSP) */
#ifndef BSP_FEATURE_USB_REG_PHYSLEW_VALUE
#define BSP_FEATURE_USB_REG_PHYSLEW_VALUE    (0x0Eu)
#endif

/* USB interrupt priority */
#define USB_IRQ_PRIORITY        (5u)

/* ---------------------------------------------------------------------------
 * Driver state
 * ------------------------------------------------------------------------- */

#define USB_MX  R_USB_FS0

#ifdef USBPHY_RA_DEBUG
/* Ring log for debugging with a debugger: 64 entries x 4 words. */
__attribute__((section(".bss.usbphy_dbg"), used))
volatile uint32_t usbphy_log[64 * 4];
__attribute__((section(".bss.usbphy_dbg"), used))
volatile uint32_t usbphy_log_idx;
#define LOG4(a, b, c, d) do { \
    uint32_t li_ = usbphy_log_idx & 63U; \
    usbphy_log[li_ * 4U] = (a); \
    usbphy_log[li_ * 4U + 1U] = (b); \
    usbphy_log[li_ * 4U + 2U] = (c); \
    usbphy_log[li_ * 4U + 3U] = (d); \
    usbphy_log_idx++; \
} while (0)
#else
#define LOG4(a, b, c, d) do { } while (0)
#endif

static USBPhyHw *instance;
static bool run_later_ctrl_comp = false;

/* ---------------------------------------------------------------------------
 * Interrupt handler (called from the FSP vector table)
 * ------------------------------------------------------------------------- */
extern "C" void usbfs_int_isr(void)
{
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);


    USBPhyHw::_usbisr();
}

USBPhy *get_usb_phy()
{
    static USBPhyHw usbphy;
    return &usbphy;
}

USBPhyHw::USBPhyHw() : events(NULL)
{
    for (int i = 0; i < PIPE_NUM; i++) {
        pipe_ctrl[i].enable = false;
        pipe_ctrl[i].status = USB_DATA_NONE;
        pipe_ctrl[i].req_size = 0;
        pipe_ctrl[i].data_cnt = 0;
        pipe_ctrl[i].p_data = NULL;
        pipe2ep[i] = 0;
    }
}

USBPhyHw::~USBPhyHw()
{
}

void USBPhyHw::init(USBPhyEvents *events)
{
    this->events = events;

    /* registers me */
    instance = this;

    /* Disable the USB interrupt until connect() */
    R_BSP_IrqDisable(USBFS_INT_IRQn);

    /* Enable power for the USB_FS0 module (clear MSTPB11) */
    R_BSP_MODULE_START(FSP_IP_USBFS, 0u);

    /* Deep software standby: clear transceiver output fix */
    R_USB_FS0->DPUSR0R_FS_b.FIXPHY0 = 0U;

    /* Reset interrupt enables and pipe states */
    USB_MX->INTENB0 = 0;
    USB_MX->BRDYENB = 0;
    USB_MX->NRDYENB = 0;
    USB_MX->BEMPENB = 0;
    USB_MX->BRDYSTS = 0;
    USB_MX->NRDYSTS = 0;
    USB_MX->BEMPSTS = 0;
    USB_MX->DCPCTR = USB_SQSET;

    /* Enable the USB clock to the module and power it on.
     * The 48 MHz UCLK is set up by the BSP clock initialisation
     * (SCKDIVCR2, USB clock divider). */
    USB_MX->SYSCFG |= USB_SCKE;
    while (USB_SCKE != (USB_MX->SYSCFG & USB_SCKE)) {
        /* Wait for the SCKE bit to be set */
    }

    /* PHY cross point adjustment */
    USB_MX->PHYSLEW = BSP_FEATURE_USB_REG_PHYSLEW_VALUE;

    USB_MX->SYSCFG &= (uint16_t) ~USB_DRPD;
    USB_MX->SYSCFG |= USB_USBE;

    USB_MX->CFIFOSEL = USB0_FIFO_MBW;
    USB_MX->D0FIFOSEL = USB0_FIFO_MBW;
    USB_MX->D1FIFOSEL = USB0_FIFO_MBW;

    USB_MX->DCPCFG = 0;
    USB_MX->DCPMAXP = MAX_PACKET_SIZE_EP0;
}

void USBPhyHw::deinit()
{
    disconnect();

    /* Reset the module registers and power it down */
    USB_MX->DVSTCTR0 = 0;
    USB_MX->DCPCTR = USB_SQSET;
    for (int i = 0; i < 9; i++) {
        USB_MX->PIPE_CTR[i] = 0;
    }
    USB_MX->BRDYENB = 0;
    USB_MX->NRDYENB = 0;
    USB_MX->BEMPENB = 0;
    USB_MX->INTENB0 = 0;
    USB_MX->SYSCFG &= (uint16_t) ~(USB_DPRPU | USB_DRPD | USB_DCFM | USB_USBE);

    R_BSP_MODULE_STOP(FSP_IP_USBFS, 0u);

    events = NULL;
}

bool USBPhyHw::powered()
{
    /* The board does not route VBUS to the USB module, assume always
     * powered (the reference EK-RA6M3G examples do the same). */
    return true;
}

void USBPhyHw::connect()
{
    /* Clear stale interrupt status */
    USB_MX->INTSTS0 = 0;
    USB_MX->BRDYSTS = 0;
    USB_MX->NRDYSTS = 0;
    USB_MX->BEMPSTS = 0;

    /* Default control pipe configuration */
    USB_MX->DCPCFG = 0;
    USB_MX->DCPMAXP = MAX_PACKET_SIZE_EP0;
    set_pid(USB_PIPE0, USB_PID_NAK);

    /* Enable the single end receiver for resume detection */
    USB_MX->SYSCFG |= USB_CNEN;

    /* Enable the interrupts used by this driver (the VBUS interrupt is
     * not used, see powered()). */
    USB_MX->INTENB0 = (USB_BEMPE | USB_BRDYE | USB_DVSE | USB_CTRE | USB_RSME);

    /* Enable the combined USBFS interrupt in the ICU/NVIC */
    R_BSP_IrqCfgEnable(USBFS_INT_IRQn, USB_IRQ_PRIORITY, NULL);

    /* Pull up D+ so the host can enumerate this device */
    USB_MX->SYSCFG |= USB_DPRPU;
}

void USBPhyHw::disconnect()
{
    /* Disable pull up on D+ */
    USB_MX->SYSCFG &= ~USB_DPRPU;

    /* Stop the USB interrupt */
    R_BSP_IrqDisable(USBFS_INT_IRQn);

    USB_MX->INTSTS0 = 0;
    USB_MX->BRDYSTS = 0;
    USB_MX->NRDYSTS = 0;
    USB_MX->BEMPSTS = 0;
    USB_MX->INTENB0 = 0;
    USB_MX->BRDYENB = 0;
    USB_MX->NRDYENB = 0;
    USB_MX->BEMPENB = 0;
}

void USBPhyHw::configure()
{
}

void USBPhyHw::unconfigure()
{
}

void USBPhyHw::sof_enable()
{
    /* Enable SOF interrupt */
    USB_MX->INTENB0 |= USB_SOFE;
}

void USBPhyHw::sof_disable()
{
    /* Disable SOF interrupt */
    USB_MX->INTENB0 &= ~USB_SOFE;
}

void USBPhyHw::set_address(uint8_t address)
{
    (void)address;
    /* The peripheral latches the address from the SET_ADDRESS request into
     * USBADDR when the status stage completes. Enabling the DCP buffer
     * acknowledges the status stage. */
    set_pid(USB_PIPE0, USB_PID_BUF);
}

void USBPhyHw::remote_wakeup()
{
    /* Output resume signalling for ~10 ms */
    USB_MX->DVSTCTR0 |= USB_RESUME;
    cpu_delay_1us(10 * 1000);
    USB_MX->DVSTCTR0 &= (uint16_t) ~USB_RESUME;
}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    /* 9 usable pipes in addition to the control pipe. Pipes 1 and 2 have
     * 256 byte buffers (isochronous capable), pipes 3-9 have 64 byte
     * buffers (bulk/interrupt only). */
    static const usb_ep_table_t ra6m3_table = {
        1, // No cost per endpoint - everything allocated up front
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_ISO | USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_ISO | USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
        }
    };
    return &ra6m3_table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    (void)max_packet;
    return MAX_PACKET_SIZE_EP0;
}

void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    /* mbed always requests _max_packet_size_ep0 bytes, copy only the
     * 8 setup bytes actually captured. */
    if (size > 8) {
        size = 8;
    }
    memcpy(buffer, setup_buffer, size);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    if ((data == NULL) || (size == 0)) {
        /* Status stage of a control read (IN direction) transfer.
         * The mbed USBDevice stack arms the status stage immediately
         * after the last ep0_write(), while the IN data packet may
         * still be pending in the DCP FIFO. Do not touch the FIFO here
         * (switching CFIFOSEL or writing BCLR would discard the armed
         * packet) - just enable the DCP buffer so the OUT status token
         * is acknowledged. The CCPL bit completes the transfer in
         * hardware when the RDSS stage is reached. */
        set_pid(USB_PIPE0, USB_PID_BUF);
        return;
    }

    pipe_ctrl[USB_PIPE0].req_size  = size;
    pipe_ctrl[USB_PIPE0].data_cnt  = size;
    pipe_ctrl[USB_PIPE0].p_data    = data;

    chg_curpipe(USB_PIPE0, USB_ISEL_READ);      /* Switch FIFO and pipe number. */
    USB_MX->CFIFOCTR = USB_BCLR;                /* Buffer clear */
    set_pid(USB_PIPE0, USB_PID_BUF);            /* Set BUF */
    USB_MX->BRDYENB |= (1 << USB_PIPE0);        /* Enable ready interrupt */
    USB_MX->NRDYENB |= (1 << USB_PIPE0);        /* Enable not ready interrupt */
}

uint32_t USBPhyHw::ep0_read_result()
{
    return pipe_ctrl[USB_PIPE0].req_size;
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    if ((buffer == NULL) || (size == 0)) {
        set_pid(USB_PIPE0, USB_PID_BUF);            /* Set BUF */
        return;
    }

    pipe_ctrl[USB_PIPE0].req_size  = size;
    pipe_ctrl[USB_PIPE0].data_cnt  = size;
    pipe_ctrl[USB_PIPE0].p_data    = buffer;

    chg_curpipe(USB_PIPE0, USB_ISEL_WRITE);         /* Switch FIFO and pipe number. */
    USB_MX->CFIFOCTR = USB_BCLR;                    /* Buffer clear */
    /* Clear the PIPE0BEMP status bit */
    USB_MX->BEMPSTS = (uint16_t)((~(1 << USB_PIPE0)) & BEMPSTS_MASK);

    /* Peripheral control sequence */
    switch (write_data(USB_PIPE0)) {
        case USB_WRITING :                          /* Continue of data write */
            USB_MX->BRDYENB |= (1 << USB_PIPE0);    /* Enable Ready interrupt */
            USB_MX->NRDYENB |= (1 << USB_PIPE0);    /* Enable Not Ready Interrupt */
            set_pid(USB_PIPE0, USB_PID_BUF);
            break;
        case USB_WRITEEND :                         /* End of data write */
        case USB_WRITESHRT :                        /* End of data write */
            USB_MX->BEMPENB |= (1 << USB_PIPE0);    /* Enable Empty Interrupt */
            USB_MX->NRDYENB |= (1 << USB_PIPE0);    /* Enable Not Ready Interrupt */
            set_pid(USB_PIPE0, USB_PID_BUF);
            break;
        case USB_FIFOERROR :                        /* FIFO access error */
            ctrl_end((uint16_t)USB_DATA_ERR);
            break;
        default :
            break;
    }
}

void USBPhyHw::ep0_stall()
{
    set_pid(USB_PIPE0, USB_PID_STALL);
    run_later_ctrl_comp = false;
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    uint16_t pipe;
    uint16_t pipecfg = 0;

    if ((endpoint == 0x00) || (endpoint == 0x80)) {
        /* endpoint 0 */
        return true;
    }

    if ((endpoint & 0x0F) > 9) {
        return false;
    }

    /* Find a free pipe for this endpoint. Isochronous endpoints can only
     * use pipes 1 and 2 (256 byte buffers); other types prefer pipes 3-9
     * to keep the isochronous capable pipes free. */
    if (type == USB_EP_TYPE_ISO) {
        if (max_packet > 256) {
            /* Pipes 1 and 2 have 256 byte buffers */
            return false;
        }
        if (pipe2ep[1] == 0) {
            pipe = 1;
        } else if (pipe2ep[2] == 0) {
            pipe = 2;
        } else {
            return false;
        }
    } else {
        for (pipe = 3; pipe < PIPE_NUM; pipe++) {
            if (pipe2ep[pipe] == 0) {
                break;
            }
        }
        if (pipe == PIPE_NUM) {
            /* fall back to the iso-capable pipes */
            if (pipe2ep[1] == 0) {
                pipe = 1;
            } else if (pipe2ep[2] == 0) {
                pipe = 2;
            } else {
                return false;
            }
        }
    }

    switch (type) {
        case USB_EP_TYPE_ISO:
            pipecfg = USB_TYPFIELD_ISO;
            break;
        case USB_EP_TYPE_INT:
            pipecfg = USB_TYPFIELD_INT;
            break;
        case USB_EP_TYPE_BULK:
        default:
            pipecfg = USB_TYPFIELD_BULK;
            break;
    }
    pipecfg |= (endpoint & 0x80) ? USB_DIR_P_IN : USB_DIR_P_OUT;
    pipecfg |= (endpoint & 0x0F);
    pipecfg |= (USB_BFREOFF | USB_CFG_DBLBOFF | USB_CFG_CNTMDOFF);

    /* Interrupt Disable */
    USB_MX->BRDYENB &= (uint16_t) ~(1 << pipe);
    USB_MX->NRDYENB &= (uint16_t) ~(1 << pipe);
    USB_MX->BEMPENB &= (uint16_t) ~(1 << pipe);

    set_pid(pipe, USB_PID_NAK);

    /* CurrentPIPE Clear */
    if ((USB_MX->CFIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX->CFIFOSEL &= ~USB_CURPIPE;
    }
    if ((USB_MX->D0FIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX->D0FIFOSEL &= ~USB_CURPIPE;
    }
    if ((USB_MX->D1FIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX->D1FIFOSEL &= ~USB_CURPIPE;
    }

    /* PIPE Configuration */
    USB_MX->PIPESEL  = pipe;                    /* Pipe select */
    USB_MX->PIPECFG  = pipecfg;
    USB_MX->PIPEMAXP = (uint16_t)max_packet;
    USB_MX->PIPEPERI = 0;

    /* Set toggle bit to DATA0 */
    *get_pipectr_reg(pipe) |= USB_SQCLR;
    /* Buffer Clear */
    *get_pipectr_reg(pipe) |= USB_ACLRM;
    *get_pipectr_reg(pipe) &= ~USB_ACLRM;

    pipe2ep[pipe] = endpoint;
    LOG4(0xC1, endpoint, pipe, pipecfg);
    pipe_ctrl[pipe].enable = false;
    pipe_ctrl[pipe].status = USB_DATA_NONE;

    return true;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    uint16_t pipe = EP2PIPE(endpoint);

    if (pipe == 0) {
        return;
    }

    /* Interrupt Disable */
    USB_MX->BRDYENB &= (uint16_t) ~(1 << pipe);
    USB_MX->NRDYENB &= (uint16_t) ~(1 << pipe);
    USB_MX->BEMPENB &= (uint16_t) ~(1 << pipe);

    set_pid(pipe, USB_PID_NAK);

    /* CurrentPIPE Clear */
    if ((USB_MX->CFIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX->CFIFOSEL &= ~USB_CURPIPE;
    }
    if ((USB_MX->D0FIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX->D0FIFOSEL &= ~USB_CURPIPE;
    }
    if ((USB_MX->D1FIFOSEL & USB_CURPIPE) == pipe) {
        USB_MX->D1FIFOSEL &= ~USB_CURPIPE;
    }

    /* PIPE Configuration */
    USB_MX->PIPESEL = pipe;                     /* Pipe select */
    USB_MX->PIPECFG = 0;

    pipe2ep[pipe] = 0;
    pipe_ctrl[pipe].enable = false;
    pipe_ctrl[pipe].status = USB_DATA_NONE;
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    uint16_t pipe = EP2PIPE(endpoint);

    if (pipe == 0) {
        return;
    }

    set_pid(pipe, USB_PID_STALL);

    pipe_ctrl[pipe].enable = false;
    pipe_ctrl[pipe].status = USB_DATA_STALL;
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    uint16_t pipe = EP2PIPE(endpoint);

    if (pipe == 0) {
        return;
    }

    set_pid(pipe, USB_PID_NAK);

    /* Set toggle bit to DATA0 */
    *get_pipectr_reg(pipe) |= USB_SQCLR;
    /* Buffer Clear */
    *get_pipectr_reg(pipe) |= USB_ACLRM;
    *get_pipectr_reg(pipe) &= ~USB_ACLRM;

    pipe_ctrl[pipe].enable = false;
    pipe_ctrl[pipe].status = USB_DATA_NONE;
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    uint16_t mxps;
    uint16_t trncnt;
    volatile uint16_t *p_reg;
    uint16_t pipe = EP2PIPE(endpoint);

    LOG4(0xC2, endpoint, size, pipe);

    if (pipe == 0) {
        return false;
    }

    if (pipe_ctrl[pipe].status == USB_DATA_STALL) {
        return false;
    }

    pipe_ctrl[pipe].status   = USB_DATA_READING;
    pipe_ctrl[pipe].req_size = size;
    pipe_ctrl[pipe].data_cnt = size;
    pipe_ctrl[pipe].p_data   = data;
    pipe_ctrl[pipe].enable   = true;

    set_pid(pipe, USB_PID_NAK);                                     /* Set NAK */

    USB_MX->BEMPSTS = (uint16_t)((~(1 << pipe)) & BEMPSTS_MASK);    /* BEMP Status Clear */
    USB_MX->BRDYSTS = (uint16_t)((~(1 << pipe)) & BRDYSTS_MASK);    /* BRDY Status Clear */
    USB_MX->NRDYSTS = (uint16_t)((~(1 << pipe)) & NRDYSTS_MASK);    /* NRDY Status Clear */

    chg_curpipe(pipe, USB_ISEL_READ);                               /* Switch FIFO and pipe number. */
    USB_MX->CFIFOCTR = USB_BCLR;                                    /* Clear BCLR */

    if (size != 0) {
        /* Max Packet Size */
        USB_MX->PIPESEL = pipe;                                     /* Pipe select */
        mxps = (uint16_t)(USB_MX->PIPEMAXP & USB_MXPS);
        /* Data size check */
        if ((size % mxps) == (uint32_t)0u) {
            trncnt = (uint16_t)(size / mxps);
        } else {
            trncnt = (uint16_t)((size / mxps) + (uint32_t)1u);
        }

        /* Set Transaction counter */
        p_reg = get_pipetre_reg(pipe);
        if (p_reg != NULL) {
            *p_reg |= USB_TRCLR;
        }
        p_reg = get_pipetrn_reg(pipe);
        if (p_reg != NULL) {
            *p_reg = trncnt;
        }
        p_reg = get_pipetre_reg(pipe);
        if (p_reg != NULL) {
            *p_reg |= USB_TRENB;
        }

        p_reg = get_pipectr_reg(pipe);
        /* Buffer Clear */
        *p_reg |= USB_ACLRM;
        *p_reg &= ~USB_ACLRM;
    }

    set_pid(pipe, USB_PID_BUF);                                     /* Set BUF */
    USB_MX->BRDYENB |= (1 << pipe);                                 /* Enable Ready Interrupt */
    USB_MX->NRDYENB |= (1 << pipe);                                 /* Enable Not Ready Interrupt */

    return true;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    uint16_t pipe = EP2PIPE(endpoint);

    if (pipe == 0) {
        return 0;
    }

    return pipe_ctrl[pipe].req_size;
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    volatile uint16_t *p_reg;
    uint16_t pipe = EP2PIPE(endpoint);

    LOG4(0xC3, endpoint, size, pipe);

    if (pipe == 0) {
        return false;
    }

    if (pipe_ctrl[pipe].status == USB_DATA_STALL) {
        return false;
    }

    pipe_ctrl[pipe].status   = USB_DATA_WRITING;
    pipe_ctrl[pipe].req_size = size;
    pipe_ctrl[pipe].data_cnt = size;
    pipe_ctrl[pipe].p_data   = data;
    pipe_ctrl[pipe].enable   = true;

    set_pid(pipe, USB_PID_NAK);                                     /* Set NAK */

    USB_MX->BEMPSTS = (uint16_t)((~(1 << pipe)) & BEMPSTS_MASK);    /* BEMP Status Clear */
    USB_MX->BRDYSTS = (uint16_t)((~(1 << pipe)) & BRDYSTS_MASK);    /* BRDY Status Clear */
    USB_MX->NRDYSTS = (uint16_t)((~(1 << pipe)) & NRDYSTS_MASK);    /* NRDY Status Clear */

    p_reg = get_pipectr_reg(pipe);
    /* Buffer Clear */
    *p_reg |= USB_ACLRM;
    *p_reg &= ~USB_ACLRM;

    buf_to_fifo(pipe);                                              /* Buffer to FIFO data write */
    set_pid(pipe, USB_PID_BUF);                                     /* Set BUF */

    return true;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    uint16_t pipe = EP2PIPE(endpoint);

    if (pipe == 0) {
        return;
    }

    forced_termination(pipe, (uint16_t)USB_DATA_NONE);
}

void USBPhyHw::process()
{
    /* Register Save */
    uint16_t intsts0 = USB_MX->INTSTS0;

    LOG4(0xA1, intsts0, (USB_MX->BRDYSTS << 16) | USB_MX->BEMPSTS, USB_MX->DCPCTR);
    uint16_t brdysts = USB_MX->BRDYSTS;
    uint16_t nrdysts = USB_MX->NRDYSTS;
    uint16_t bempsts = USB_MX->BEMPSTS;
    uint16_t intenb0 = USB_MX->INTENB0;
    uint16_t brdyenb = USB_MX->BRDYENB;
    uint16_t nrdyenb = USB_MX->NRDYENB;
    uint16_t bempenb = USB_MX->BEMPENB;

    /* Interrupt status get */
    uint16_t ists0 = (uint16_t)(intsts0 & intenb0);
    uint16_t bsts  = (uint16_t)(brdysts & brdyenb);
    uint16_t nsts  = (uint16_t)(nrdysts & nrdyenb);
    uint16_t ests  = (uint16_t)(bempsts & bempenb);

    uint16_t i;

    if ((intsts0 & (USB_VBINT | USB_RESM | USB_SOFR | USB_DVST |
                    USB_CTRT | USB_BEMP | USB_NRDY | USB_BRDY)) == 0u) {
        return;
    }

    /***** Processing USB bus signal *****/
    /***** Resume signal *****/
    if ((ists0 & USB_RESM) == USB_RESM) {
        USB_MX->INTSTS0 = (uint16_t)~USB_RESM;
        events->suspend(false);
    }

    /***** Vbus change (not used on this board) *****/
    else if ((ists0 & USB_VBINT) == USB_VBINT) {
        USB_MX->INTSTS0 = (uint16_t)~USB_VBINT;
    }

    /***** SOFR change *****/
    else if ((ists0 & USB_SOFR) == USB_SOFR) {
        USB_MX->INTSTS0 = (uint16_t)~USB_SOFR;
        events->sof(USB_MX->FRMNUM & 0x07FFu);
    }

    /***** Processing device state *****/
    /***** DVST change *****/
    else if ((ists0 & USB_DVST) == USB_DVST) {
        USB_MX->INTSTS0 = (uint16_t)~USB_DVST;

        switch ((uint16_t)(intsts0 & USB_DVSQ)) {
            case USB_DS_POWR :
                break;
            case USB_DS_DFLT :
                /* USB bus reset */
                USB_MX->DCPCFG = 0;                    /* DCP configuration register */
                USB_MX->DCPMAXP = MAX_PACKET_SIZE_EP0; /* DCP maxpacket size register */

                /* Remove all endpoints except endpoint 0 */
                for (i = USB_MIN_PIPE_NO; i < PIPE_NUM; i++) {
                    if (pipe2ep[i] != 0) {
                        forced_termination(i, (uint16_t)USB_DATA_NONE);

                        USB_MX->PIPESEL = i;
                        USB_MX->PIPECFG = 0;
                        pipe2ep[i] = 0;
                    }
                }

                events->reset();
                break;
            case USB_DS_ADDS :
                break;
            case USB_DS_CNFG :
                break;
            case USB_DS_SPD_POWR :
            case USB_DS_SPD_DFLT :
            case USB_DS_SPD_ADDR :
            case USB_DS_SPD_CNFG :
                events->suspend(true);
                break;
            default :
                break;
        }
    }

    /***** Processing PIPE0 data *****/
    else if (((ists0 & USB_BRDY) == USB_BRDY) && ((bsts & USB_BITSET(USB_PIPE0)) == USB_BITSET(USB_PIPE0))) {
        /* ==== BRDY PIPE0 ==== */
        USB_MX->BRDYSTS = (uint16_t)((~USB_BITSET(USB_PIPE0)) & BRDYSTS_MASK);

        /* When operating by the peripheral function, usb_brdy_pipe() is executed with PIPEx request because */
        /* two BRDY messages are issued even when the demand of PIPE0 and PIPEx has been generated at the same time. */
        if ((USB_MX->CFIFOSEL & USB_ISEL_WRITE) == USB_ISEL_WRITE) {
            switch (write_data(USB_PIPE0)) {
                case USB_WRITEEND :
                case USB_WRITESHRT :
                    USB_MX->BRDYENB &= (uint16_t) ~(1 << USB_PIPE0);
                    break;
                case USB_WRITING :
                    set_pid(USB_PIPE0, USB_PID_BUF);
                    break;
                case USB_FIFOERROR :
                    ctrl_end((uint16_t)USB_DATA_ERR);
                    break;
                default :
                    break;
            }
            events->ep0_in();
        } else {
            switch (read_data(USB_PIPE0)) {
                case USB_READEND :
                case USB_READSHRT :
                    USB_MX->BRDYENB &= (uint16_t) ~(1 << USB_PIPE0);
                    pipe_ctrl[USB_PIPE0].req_size -= pipe_ctrl[USB_PIPE0].data_cnt;
                    break;
                case USB_READING :
                    set_pid(USB_PIPE0, USB_PID_BUF);
                    break;
                case USB_READOVER :
                    ctrl_end((uint16_t)USB_DATA_OVR);
                    pipe_ctrl[USB_PIPE0].req_size -= pipe_ctrl[USB_PIPE0].data_cnt;
                    break;
                case USB_FIFOERROR :
                    ctrl_end((uint16_t)USB_DATA_ERR);
                    break;
                default :
                    break;
            }
            events->ep0_out();
        }
    } else if (((ists0 & USB_BEMP) == USB_BEMP) && ((ests & USB_BITSET(USB_PIPE0)) == USB_BITSET(USB_PIPE0))) {
        /* ==== BEMP PIPE0 ==== */
        USB_MX->BEMPSTS = (uint16_t)((~USB_BITSET(USB_PIPE0)) & BEMPSTS_MASK);

        events->ep0_in();
    } else if (((ists0 & USB_NRDY) == USB_NRDY) && ((nsts & USB_BITSET(USB_PIPE0)) == USB_BITSET(USB_PIPE0))) {
        /* ==== NRDY PIPE0 ==== */
        USB_MX->NRDYSTS = (uint16_t)((~USB_BITSET(USB_PIPE0)) & NRDYSTS_MASK);
        /* Non processing. */
    }

    /***** Processing setup transaction *****/
    else if ((ists0 & USB_CTRT) == USB_CTRT) {
        USB_MX->INTSTS0 = (uint16_t)~USB_CTRT;
        LOG4(0xA4, intsts0 & (USB_CTSQ | USB_VALID), USB_MX->USBREQ, USB_MX->DCPCTR);

        /* CTSQ reloading */
        uint16_t stginfo = (uint16_t)(intsts0 & USB_CTSQ);
        if (stginfo != USB_CS_IDST) {
            if (((USB_CS_RDDS == stginfo) || (USB_CS_WRDS == stginfo)) || (USB_CS_WRND == stginfo)) {
                /* Save request register */
                uint16_t *bufO = &setup_buffer[0];

                USB_MX->INTSTS0 = (uint16_t)~USB_VALID;
                *bufO++ = USB_MX->USBREQ;     /* data[0] <= bmRequest, data[1] <= bmRequestType */
                *bufO++ = USB_MX->USBVAL;     /* data[2] data[3] <= wValue */
                *bufO++ = USB_MX->USBINDX;    /* data[4] data[5] <= wIndex */
                *bufO++ = USB_MX->USBLENG;    /* data[6] data[7] <= wLength */
            }
        }

        /* Switch on the control transfer stage (CTSQ). */
        switch (stginfo) {
            case USB_CS_IDST :  /* Idle or setup stage */
                break;
            case USB_CS_RDDS :  /* Control read data stage */
                events->ep0_setup();
                break;
            case USB_CS_WRDS :  /* Control write data stage */
                events->ep0_setup();
                break;
            case USB_CS_WRND :  /* Status stage of a control write where there is no data stage. */
                events->ep0_setup();
                run_later_ctrl_comp = true;
                break;
            case USB_CS_RDSS :  /* Control read status stage */
                USB_MX->DCPCTR |= USB_CCPL;
                break;
            case USB_CS_WRSS :  /* Control write status stage */
                USB_MX->DCPCTR |= USB_CCPL;
                break;
            case USB_CS_SQER :  /* Control sequence error */
            default :           /* Illegal */
                ctrl_end((uint16_t)USB_DATA_ERR);
                break;
        }
    }

    /***** Processing PIPE1-PIPE9 data *****/
    else if ((ists0 & USB_BRDY) == USB_BRDY) {
        /* ==== BRDY PIPEx ==== */
        USB_MX->BRDYSTS = (uint16_t)((~bsts) & BRDYSTS_MASK);

        for (i = USB_MIN_PIPE_NO; i < PIPE_NUM; i++) {
            if ((bsts & USB_BITSET(i)) != 0u) {
                /* Interrupt check */
                if (pipe_ctrl[i].enable) {
                    USB_MX->PIPESEL = i;    /* Pipe select */
                    if (USB_DIR_P_IN == (uint16_t)(USB_MX->PIPECFG & USB_DIRFIELD)) {
                        /* write */
                        LOG4(0xC4, i, USB_MX->CFIFOCTR, pipe_ctrl[i].data_cnt);
                        buf_to_fifo(i);         /* Buffer to FIFO data write */
                        events->in(PIPE2EP(i));
                    } else {
                        /* read */
                        LOG4(0xC5, i, USB_MX->CFIFOCTR, pipe_ctrl[i].data_cnt);
                        fifo_to_buf(i);         /* FIFO to Buffer data read */
                        events->out(PIPE2EP(i));
                    }
                }
            }
        }
    } else if ((ists0 & USB_BEMP) == USB_BEMP) {
        /* ==== BEMP PIPEx ==== */
        USB_MX->BEMPSTS = (uint16_t)((~ests) & BEMPSTS_MASK);

        for (i = USB_MIN_PIPE_NO; i < PIPE_NUM; i++) {
            if ((ests & USB_BITSET(i)) != 0) {
                /* Interrupt check */
                if (pipe_ctrl[i].enable) {
                    LOG4(0xC6, i, *get_pipectr_reg(i), pipe_ctrl[i].data_cnt);
                    /* MAX packet size error ? */
                    if (((get_pid(i) & USB_PID_STALL) == USB_PID_STALL) || ((get_pid(i) & USB_PID_STALL2) == USB_PID_STALL2)) {
                        forced_termination(i, (uint16_t)USB_DATA_STALL);
                    } else {
                        if ((i >= 6) || ((*get_pipectr_reg(i) & USB_INBUFM) != USB_INBUFM)) {
                            data_end(i, (uint16_t)USB_DATA_NONE);       /* End of data transfer */
                        } else {
                            USB_MX->BEMPENB |= (1 << i);
                        }
                    }
                    events->in(PIPE2EP(i));
                }
            }
        }
    } else if ((ists0 & USB_NRDY) == USB_NRDY) {
        /* ==== NRDY PIPEx ==== */
        USB_MX->NRDYSTS = (uint16_t)((~nsts) & NRDYSTS_MASK);

        for (i = USB_MIN_PIPE_NO; i < PIPE_NUM; i++) {
            if ((nsts & USB_BITSET(i)) != 0) {
                /* Interrupt check */
                if (pipe_ctrl[i].enable) {
                    if (((get_pid(i) & USB_PID_STALL) != USB_PID_STALL) && ((get_pid(i) & USB_PID_STALL2) != USB_PID_STALL2)) {
                        set_pid(i, USB_PID_BUF);
                    }
                }
            }
        }
    } else {
        /* Non processing. */
    }
}

void USBPhyHw::_usbisr(void)
{
    if ((instance == NULL) || (instance->events == NULL)) {
        return;
    }

    run_later_ctrl_comp = false;

    instance->events->start_process();

    if (run_later_ctrl_comp) {
        /* Complete the status stage of a no-data control write (e.g.
         * SET_ADDRESS / SET_CONFIGURATION) in hardware. */
        USB_MX->DCPCTR &= (~USB_PID);
        USB_MX->DCPCTR |= USB_PID_BUF;
        USB_MX->DCPCTR |= USB_CCPL;
    }
}

void USBPhyHw::chg_curpipe(uint16_t pipe, uint16_t isel)
{
    uint16_t buf;

    buf  = USB_MX->CFIFOSEL;
    buf &= (uint16_t)(~(USB_RCNT | USB_ISEL | USB_CURPIPE | USB_MBW));
    buf |= (uint16_t)((USB_RCNT | isel | pipe | USB0_FIFO_MBW) & (USB_RCNT | USB_ISEL | USB_CURPIPE | USB_MBW));
    USB_MX->CFIFOSEL = buf;

    do {
        cpu_delay_1us(1);
        buf = USB_MX->CFIFOSEL;
    } while ((buf & (uint16_t)(USB_ISEL | USB_CURPIPE)) != (uint16_t)(isel | pipe));
}

uint16_t USBPhyHw::is_set_frdy(uint16_t pipe, uint16_t isel)
{
    uint16_t buffer;
    int retry_cnt = 0;

    chg_curpipe(pipe, isel);                    /* Changes the FIFO port by the pipe. */
    for (retry_cnt = 0; retry_cnt < 10; retry_cnt++) {
        buffer = USB_MX->CFIFOCTR;
        if ((uint16_t)(buffer & USB_FRDY) == USB_FRDY) {
            return (buffer);
        }
        cpu_delay_1us(1);
    }

    return (USB_FIFOERROR);
}

uint8_t *USBPhyHw::read_fifo(uint16_t pipe, uint16_t count, uint8_t *read_p)
{
    uint16_t even;
    uint16_t odd_byte_data_temp;

    (void)pipe;

    /* The FS module only supports 16-bit FIFO accesses (8-bit is not
     * available on reads), match the FSP usb_pstd_read_fifo IP0 path. */
    for (even = (uint16_t)(count >> 1); (even != 0); --even) {
        /* 16bit FIFO access */
        *((uint16_t *)read_p) = USB_MX->CFIFOL;
        read_p += sizeof(uint16_t);
    }
    if ((count & (uint16_t)0x0001u) != 0u) {
        /* Odd trailing byte: 16bit access, take the low byte (little endian) */
        odd_byte_data_temp = USB_MX->CFIFOL;
        *read_p = (uint8_t)(odd_byte_data_temp & 0x00ffu);
        read_p += sizeof(uint8_t);
    }

    return read_p;
}

uint16_t USBPhyHw::read_data(uint16_t pipe)
{
    uint16_t count;
    uint16_t buffer;
    uint16_t mxps;
    uint16_t dtln;
    uint16_t end_flag;

    /* Changes FIFO port by the pipe. */
    buffer = is_set_frdy(pipe, 0);
    if (buffer == USB_FIFOERROR) {
        return (USB_FIFOERROR);                 /* FIFO access error */
    }
    dtln = (uint16_t)(buffer & USB_DTLN);

    /* Max Packet Size */
    if (pipe == USB_PIPE0) {
        mxps = (uint16_t)(USB_MX->DCPMAXP & USB_MAXP);
    } else {
        USB_MX->PIPESEL = pipe;         /* Pipe select */
        mxps = (uint16_t)(USB_MX->PIPEMAXP & USB_MXPS);
    }

    if (pipe_ctrl[pipe].data_cnt < dtln) {
        /* Buffer Over ? */
        end_flag = USB_READOVER;
        set_pid(pipe, USB_PID_NAK);             /* Set NAK */
        count = (uint16_t)pipe_ctrl[pipe].data_cnt;
        pipe_ctrl[pipe].data_cnt = dtln;
    } else if (pipe_ctrl[pipe].data_cnt == dtln) {
        /* Just Receive Size */
        count = dtln;
        if ((count == 0) || ((dtln % mxps) != 0)) {
            /* Just Receive Size */
            /* Peripheral Function */
            end_flag = USB_READSHRT;
        } else {
            end_flag = USB_READEND;
            set_pid(pipe, USB_PID_NAK);         /* Set NAK */
        }
    } else {
        /* Continuous Receive data */
        count = dtln;
        end_flag = USB_READING;
        if (count == 0) {
            /* Null Packet receive */
            end_flag = USB_READSHRT;
            set_pid(pipe, USB_PID_NAK);         /* Set NAK */
        }
        if ((count % mxps) != 0) {
            /* Short Packet receive */
            end_flag = USB_READSHRT;
            set_pid(pipe, USB_PID_NAK);         /* Set NAK */
        }
    }

    if (dtln == 0) { /* 0 length packet */
        USB_MX->CFIFOCTR = USB_BCLR;                      /* Clear BCLR */
    } else {
        pipe_ctrl[pipe].p_data = read_fifo(pipe, count, pipe_ctrl[pipe].p_data);
    }
    pipe_ctrl[pipe].data_cnt -= count;

    return end_flag;
}

void USBPhyHw::fifo_to_buf(uint16_t pipe)
{
    /* Check FIFO access sequence */
    switch (read_data(pipe)) {
        case USB_READING :                                      /* Continue of data read */
            break;
        case USB_READEND :                                      /* End of data read */
            data_end(pipe, (uint16_t)USB_DATA_OK);
            pipe_ctrl[pipe].req_size -= pipe_ctrl[pipe].data_cnt;
            break;
        case USB_READSHRT :                                     /* End of data read */
            data_end(pipe, (uint16_t)USB_DATA_SHT);
            pipe_ctrl[pipe].req_size -= pipe_ctrl[pipe].data_cnt;
            break;
        case USB_READOVER :                                     /* Buffer over */
            forced_termination(pipe, (uint16_t)USB_DATA_OVR);
            pipe_ctrl[pipe].req_size -= pipe_ctrl[pipe].data_cnt;
            break;
        case USB_FIFOERROR :                                    /* FIFO access error */
        default:
            forced_termination(pipe, (uint16_t)USB_DATA_ERR);
            break;
    }
}

uint8_t *USBPhyHw::write_fifo(uint16_t pipe, uint16_t count, uint8_t *write_p)
{
    uint16_t even;

    set_mbw(pipe, USB0_FIFO_MBW);                             /* 16bit access */
    for (even = (uint16_t)(count >> 1); (even != 0); --even) {
        /* 16bit FIFO access */
        USB_MX->CFIFOL = *((uint16_t *)write_p);
        write_p += sizeof(uint16_t);
    }
    if ((count & (uint16_t)0x0001u) != 0u) {
        /* Odd trailing byte: 8bit access, then restore the width */
        set_mbw(pipe, USB_MBW_8);                             /* 8bit access */
        USB_MX->CFIFOLL = *write_p;
        write_p++;
        set_mbw(pipe, USB0_FIFO_MBW);                         /* 16bit access */
    }

    return write_p;
}

uint16_t USBPhyHw::write_data(uint16_t pipe)
{
    uint16_t size;
    uint16_t count;
    uint16_t mxps;
    uint16_t end_flag;
    uint16_t buffer;

    /* Changes FIFO port by the pipe. */
    if (pipe == USB_PIPE0) {
        buffer = is_set_frdy(pipe, USB_ISEL_WRITE);
    } else {
        buffer = is_set_frdy(pipe, 0);
    }

    if (buffer == USB_FIFOERROR) {
        return (USB_FIFOERROR);
    }

    /* Max Packet Size */
    if (pipe == USB_PIPE0) {
        mxps = (uint16_t)(USB_MX->DCPMAXP & USB_MAXP);
    } else {
        USB_MX->PIPESEL = pipe;    /* Pipe select */
        mxps = (uint16_t)(USB_MX->PIPEMAXP & USB_MXPS);
    }

    /* Data buffer size: with the continuous transfer mode disabled only a
     * single max-packet sized chunk can be written to the pipe buffer at
     * a time (the RA6M3 FS pipes have no PIPEBUF size field). */
    size = mxps;

    /* Data size check */
    if (pipe_ctrl[pipe].data_cnt <= (uint32_t)size) {
        count = (uint16_t)pipe_ctrl[pipe].data_cnt;
        if (count == 0) {
            end_flag = USB_WRITESHRT;                   /* Null Packet is end of write */
        } else if ((count % mxps) != 0) {
            end_flag = USB_WRITESHRT;                   /* Short Packet is end of write */
        } else {
            end_flag = USB_WRITEEND;                    /* Just Send Size */
        }
    } else {
        /* Write continues */
        end_flag = USB_WRITING;
        count = size;
    }

    pipe_ctrl[pipe].p_data = write_fifo(pipe, count, pipe_ctrl[pipe].p_data);

    /* Check data count to remain */
    if (pipe_ctrl[pipe].data_cnt < (uint32_t)size) {
        pipe_ctrl[pipe].data_cnt = 0u;                  /* Clear data count */

        if ((USB_MX->CFIFOCTR & USB_BVAL) == 0u) {                /* Check BVAL */
            USB_MX->CFIFOCTR |= USB_BVAL;                         /* Short Packet */
        }
    } else {
        pipe_ctrl[pipe].data_cnt -= count;              /* Total data count - count */
    }

    return end_flag;
}

void USBPhyHw::buf_to_fifo(uint16_t pipe)
{
    /* Disable Ready Interrupt */
    USB_MX->BRDYENB &= (uint16_t) ~(1 << pipe);

    /* Peripheral control sequence */
    switch (write_data(pipe)) {
        case USB_WRITING:                           /* Continue of data write */
            USB_MX->BRDYENB |= (1 << pipe);         /* Enable Ready Interrupt */
            USB_MX->NRDYENB |= (1 << pipe);         /* Enable Not Ready Interrupt */
            break;
        case USB_WRITEEND:                          /* End of data write */
        case USB_WRITESHRT:                         /* End of data write */
            USB_MX->BEMPENB |= (1 << pipe);         /* Enable Empty Interrupt */
            USB_MX->NRDYENB |= (1 << pipe);         /* Enable Not Ready Interrupt */
            break;
        case USB_FIFOERROR:                         /* FIFO access error */
        default:
            forced_termination(pipe, (uint16_t)USB_DATA_ERR);
            break;
    }
}

volatile uint16_t *USBPhyHw::get_pipectr_reg(uint16_t pipe)
{
    if (pipe == USB_PIPE0) {
        return (volatile uint16_t *) & (USB_MX->DCPCTR);
    } else {
        return (volatile uint16_t *) & (USB_MX->PIPE_CTR[pipe - 1]);
    }
}

volatile uint16_t *USBPhyHw::get_pipetre_reg(uint16_t pipe)
{
    if ((pipe >= 1) && (pipe <= 5)) {
        return (volatile uint16_t *) & (USB_MX->PIPE_TR[pipe - 1].E);
    } else {
        return NULL;
    }
}

volatile uint16_t *USBPhyHw::get_pipetrn_reg(uint16_t pipe)
{
    if ((pipe >= 1) && (pipe <= 5)) {
        return (volatile uint16_t *) & (USB_MX->PIPE_TR[pipe - 1].N);
    } else {
        return NULL;
    }
}

uint16_t USBPhyHw::get_pid(uint16_t pipe)
{
    volatile uint16_t *p_reg;

    p_reg = get_pipectr_reg(pipe);
    return (uint16_t)(*p_reg & USB_PID);
}

void USBPhyHw::set_mbw(uint16_t pipe, uint16_t data)
{
    USB_MX->CFIFOSEL &= (~USB_MBW);
    if (data != 0) {
        USB_MX->CFIFOSEL |= data;
    }
    (void)pipe;
}

void USBPhyHw::set_pid(uint16_t pipe, uint16_t new_pid)
{
    volatile uint16_t *p_reg;
    uint16_t old_pid;

    p_reg = get_pipectr_reg(pipe);
    old_pid = get_pid(pipe);

    switch (new_pid) {
        case USB_PID_STALL:
            if ((old_pid & USB_PID_BUF) == USB_PID_BUF) {
                *p_reg &= (~USB_PID);
                *p_reg |= USB_PID_STALL2;
            } else {
                *p_reg &= (~USB_PID);
                *p_reg |= new_pid;
            }
            break;
        case USB_PID_BUF:
            if (((old_pid & USB_PID_STALL) == USB_PID_STALL) ||
                    ((old_pid & USB_PID_STALL2) == USB_PID_STALL2)) {
                *p_reg &= (~USB_PID);
                *p_reg |= USB_PID_NAK;
            }
            *p_reg &= (~USB_PID);
            *p_reg |= new_pid;
            break;
        case USB_PID_NAK:
            if ((old_pid & USB_PID_STALL2) == USB_PID_STALL2) {
                *p_reg &= (~USB_PID);
                *p_reg |= USB_PID_STALL;
            }
            *p_reg &= (~USB_PID);
            *p_reg |= new_pid;

            do {
                cpu_delay_1us(1);
                p_reg = get_pipectr_reg(pipe);
            } while ((*p_reg & USB_PBUSY) == USB_PBUSY);
            break;
        default:
            *p_reg &= (~USB_PID);
            *p_reg |= new_pid;
            break;
    }
}

void USBPhyHw::cpu_delay_1us(uint16_t time)
{
    R_BSP_SoftwareDelay((uint32_t)time, BSP_DELAY_UNITS_MICROSECONDS);
}

uint16_t USBPhyHw::EP2PIPE(usb_ep_t endpoint)
{
    for (uint16_t pipe = USB_MIN_PIPE_NO; pipe < PIPE_NUM; pipe++) {
        if (pipe2ep[pipe] == endpoint) {
            return pipe;
        }
    }
    return 0;
}

usb_ep_t USBPhyHw::PIPE2EP(uint16_t pipe)
{
    return pipe2ep[pipe];
}

void USBPhyHw::ctrl_end(uint16_t status)
{
    /* Interrupt disable */
    USB_MX->BEMPENB &= (uint16_t) ~(1 << USB_PIPE0);  /* Disable Empty Interrupt */
    USB_MX->BRDYENB &= (uint16_t) ~(1 << USB_PIPE0);  /* Disable Ready Interrupt */
    USB_MX->NRDYENB &= (uint16_t) ~(1 << USB_PIPE0);  /* Disable Not Ready Interrupt */

    set_mbw(USB_PIPE0, USB0_FIFO_MBW);

    if ((status == USB_DATA_ERR) || (status == USB_DATA_OVR)) {
        set_pid(USB_PIPE0, USB_PID_STALL);  /* Request error */
    } else {
        USB_MX->DCPCTR |= USB_CCPL;     /* Set CCPL bit */
    }
}

void USBPhyHw::data_end(uint16_t pipe, uint16_t status)
{
    volatile uint16_t *p_reg;

    /* Disable Interrupt */
    USB_MX->BRDYENB &= (uint16_t) ~(1 << pipe);      /* Disable Ready Interrupt */
    USB_MX->NRDYENB &= (uint16_t) ~(1 << pipe);      /* Disable Not Ready Interrupt */
    USB_MX->BEMPENB &= (uint16_t) ~(1 << pipe);      /* Disable Empty Interrupt */

    set_pid(pipe, USB_PID_NAK);                 /* Set NAK */

    /* Disable Transaction count */
    p_reg = get_pipetre_reg(pipe);
    if (p_reg != NULL) {
        *p_reg &= (~USB_TRENB);
        *p_reg |= USB_TRCLR;
    }

    if (pipe_ctrl[pipe].enable) {
        /* Check PIPE TYPE */
        USB_MX->PIPESEL = pipe;             /* Pipe select */
        if ((USB_MX->PIPECFG & USB_TYPFIELD) != USB_TYPFIELD_ISO) {
            /* Transfer information set */
            pipe_ctrl[pipe].enable = false;
            pipe_ctrl[pipe].status = status;
        } else if ((uint16_t)(USB_MX->PIPECFG & USB_DIRFIELD) == USB_DIR_P_IN) {
            /* ISO IN Transfer (restart) */
            pipe_ctrl[pipe].status = USB_DATA_WRITING;
        } else {
            /* ISO OUT Transfer (restart) */
            pipe_ctrl[pipe].status = USB_DATA_READING;
        }
    }
}

void USBPhyHw::forced_termination(uint16_t pipe, uint16_t status)
{
    volatile uint16_t *p_reg;

    LOG4(0xC8, pipe, status, 0);

    /* Disable Interrupt */
    USB_MX->BRDYENB &= (uint16_t) ~(1 << pipe);      /* Disable Ready Interrupt */
    USB_MX->NRDYENB &= (uint16_t) ~(1 << pipe);      /* Disable Not Ready Interrupt */
    USB_MX->BEMPENB &= (uint16_t) ~(1 << pipe);      /* Disable Empty Interrupt */

    set_pid(pipe, USB_PID_NAK);                 /* Set NAK */

    /* Disable Transaction count */
    p_reg = get_pipetre_reg(pipe);
    if (p_reg != NULL) {
        *p_reg &= (~USB_TRENB);
        *p_reg |= USB_TRCLR;
    }

    set_mbw(pipe, USB0_FIFO_MBW);

    chg_curpipe(pipe, 0);                   /* Changes the FIFO port by the pipe. */

    p_reg = get_pipectr_reg(pipe);
    /* Buffer Clear */
    *p_reg |= USB_ACLRM;
    *p_reg &= ~USB_ACLRM;

    pipe_ctrl[pipe].enable = false;
    pipe_ctrl[pipe].status  = status;
}

#endif /* DEVICE_USBDEVICE */

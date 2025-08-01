/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "can_api.h"
#include "m480_gpio.h"
#include "m480_can.h"

#if DEVICE_CAN
#include <string.h>
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "gpio_api.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"
#include "nu_bitutil.h"
#include "mbed_critical.h"
#include "mbed_error.h"

#define NU_CAN_DEBUG    0
#define CAN_NUM         2

/* Reserve Message Object number 31 for Tx */
#define NU_CAN_MSG_OBJ_NUM_TX   31

/* Max number of message ID filter handle */
#define NU_CAN_MAXNUM_HNDL      NU_CAN_MSG_OBJ_NUM_TX

/* Convert to string literal */
#define NU_STR_(X)  #X
#define NU_STR(X)   NU_STR_(X)

static uintptr_t can_irq_contexts[CAN_NUM] = {0};
static can_irq_handler can0_irq_handler;
static can_irq_handler can1_irq_handler;

extern uint32_t CAN_GetCANBitRate(CAN_T *tCAN);
extern void CAN_EnterInitMode(CAN_T *tCAN, uint8_t u8Mask);
extern void CAN_LeaveInitMode(CAN_T *tCAN);
extern void CAN_LeaveTestMode(CAN_T *tCAN);
extern void CAN_EnterTestMode(CAN_T *tCAN, uint8_t u8TestMask);
extern void CAN_CLR_INT_PENDING_ONLY_BIT(CAN_T *tCAN, uint32_t u32MsgNum);

static const struct nu_modinit_s can_modinit_tab[] = {
    {CAN_0, CAN0_MODULE, 0, 0, CAN0_RST, CAN0_IRQn, NULL},
    {CAN_1, CAN1_MODULE, 0, 0, CAN1_RST, CAN1_IRQn, NULL},

    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

void can_init_freq(can_t *obj, PinName rd, PinName td, int hz)
{
    uint32_t can_td = (CANName)pinmap_peripheral(td, PinMap_CAN_TD);
    uint32_t can_rd = (CANName)pinmap_peripheral(rd, PinMap_CAN_RD);
    obj->can = (CANName)pinmap_merge(can_td, can_rd);
    MBED_ASSERT((int)obj->can != NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    obj->pin_rd = rd;
    obj->pin_td = td;

    pinmap_pinout(td, PinMap_CAN_TD);
    pinmap_pinout(rd, PinMap_CAN_RD);

    // Enable IP clock
    CLK_EnableModuleClock(modinit->clkidx);

    // Reset this module
    SYS_ResetModule(modinit->rsetidx);

    NVIC_DisableIRQ(CAN0_IRQn);
    NVIC_DisableIRQ(CAN1_IRQn);    

    if(obj->can == CAN_1) {
        obj->index = 1;
    } else
        obj->index = 0;

#if 0
    /* TBD: For M487 mbed Board Transmitter Setting (RS Pin) */
    GPIO_SetMode(PA, BIT2| BIT3, GPIO_MODE_OUTPUT);
    PA2 = 0x00;
    PA3 = 0x00;
#endif
    CAN_Open((CAN_T *)NU_MODBASE(obj->can), hz, CAN_NORMAL_MODE);

    can_filter(obj, 0, 0, CANStandard, 0);
}

void can_init(can_t *obj, PinName rd, PinName td)
{
    can_init_freq(obj, rd, td, 500000);
}

void can_free(can_t *obj)
{

    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);

    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    // Reset this module
    SYS_ResetModule(modinit->rsetidx);

    CLK_DisableModuleClock(modinit->clkidx);

    /* Free up pins */
    gpio_set(obj->pin_rd);
    gpio_set(obj->pin_td);
    obj->pin_rd = NC;
    obj->pin_td = NC;
}

int can_frequency(can_t *obj, int hz)
{
    CAN_SetBaudRate((CAN_T *)NU_MODBASE(obj->can), hz);

    return CAN_GetCANBitRate((CAN_T *)NU_MODBASE(obj->can));
}

static void can_irq(CANName name, int id)
{

    CAN_T *can = (CAN_T *)NU_MODBASE(name);
    uint32_t u8IIDRstatus;

    u8IIDRstatus = can->IIDR;

    if(u8IIDRstatus == 0x00008000) {      /* Check Status Interrupt Flag (Error status Int and Status change Int) */
        /**************************/
        /* Status Change interrupt*/
        /**************************/
        if(can->STATUS & CAN_STATUS_RXOK_Msk) {
            can->STATUS &= ~CAN_STATUS_RXOK_Msk;   /* Clear Rx Ok status*/
        }

        if(can->STATUS & CAN_STATUS_TXOK_Msk) {
            can->STATUS &= ~CAN_STATUS_TXOK_Msk;    /* Clear Tx Ok status*/
        }

        /**************************/
        /* Error Status interrupt */
        /**************************/
        if(can->STATUS & CAN_STATUS_EWARN_Msk) {
            if(id)
                can1_irq_handler(can_irq_contexts[id], IRQ_ERROR);
            else
                can0_irq_handler(can_irq_contexts[id], IRQ_ERROR);
        }

        if(can->STATUS & CAN_STATUS_BOFF_Msk) {
            if(id)
                can1_irq_handler(can_irq_contexts[id], IRQ_BUS);
            else
                can0_irq_handler(can_irq_contexts[id], IRQ_BUS);
        }
    } else if (u8IIDRstatus >= 1 && u8IIDRstatus <= 32) {
        if ((u8IIDRstatus - 1) != NU_CAN_MSG_OBJ_NUM_TX) {
            if (id) {
                can1_irq_handler(can_irq_contexts[id], IRQ_RX);
            }
            else {
                can0_irq_handler(can_irq_contexts[id], IRQ_RX);
            }
            CAN_CLR_INT_PENDING_ONLY_BIT(can, (u8IIDRstatus -1));
        } else {
            if (id) {
                can1_irq_handler(can_irq_contexts[id], IRQ_TX);
            }
            else {
                can0_irq_handler(can_irq_contexts[id], IRQ_TX);
            }
            CAN_CLR_INT_PENDING_BIT(can, (u8IIDRstatus -1));
        }
    } else if (u8IIDRstatus!=0) {

        if(id)
            can1_irq_handler(can_irq_contexts[id], IRQ_OVERRUN);
        else
            can0_irq_handler(can_irq_contexts[id], IRQ_OVERRUN);

        CAN_CLR_INT_PENDING_BIT(can, ((can->IIDR) -1));      /* Clear Interrupt Pending */
    } else if(can->WU_STATUS == 1) {

        can->WU_STATUS = 0;                       /* Write '0' to clear */
        if(id)
            can1_irq_handler(can_irq_contexts[id], IRQ_WAKEUP);
        else
            can0_irq_handler(can_irq_contexts[id], IRQ_WAKEUP);
    }
}

void CAN0_IRQHandler(void)
{
    can_irq(CAN_0, 0);
}

void CAN1_IRQHandler(void)
{
    can_irq(CAN_1, 1);
}

void can_irq_init(can_t *obj, can_irq_handler handler, uintptr_t context)
{
    if(obj->index)
        can1_irq_handler = handler;
    else
        can0_irq_handler = handler;
    can_irq_contexts[obj->index] = context;

}

void can_irq_free(can_t *obj)
{
    CAN_DisableInt((CAN_T *)NU_MODBASE(obj->can), (CAN_CON_IE_Msk|CAN_CON_SIE_Msk|CAN_CON_EIE_Msk));

    can_irq_contexts[obj->index] = 0;

    if(!obj->index)
        NVIC_DisableIRQ(CAN0_IRQn);
    else
        NVIC_DisableIRQ(CAN1_IRQn);


}

void can_irq_set(can_t *obj, CanIrqType irq, uint32_t enable)
{
    uint8_t u8Mask;

    u8Mask = ((enable != 0 )? CAN_CON_IE_Msk :0);

    switch (irq) {
    case IRQ_ERROR:
    case IRQ_BUS:
    case IRQ_PASSIVE:
        u8Mask = u8Mask | CAN_CON_EIE_Msk | CAN_CON_SIE_Msk;
        break;

    case IRQ_RX:
    case IRQ_TX:
    case IRQ_OVERRUN:
    case IRQ_WAKEUP:
        u8Mask = u8Mask | CAN_CON_SIE_Msk;
        break;

    default:
        break;

    }
    CAN_EnterInitMode((CAN_T*)NU_MODBASE(obj->can), u8Mask);

    CAN_LeaveInitMode((CAN_T*)NU_MODBASE(obj->can));

    if(!obj->index) {
        NVIC_SetVector(CAN0_IRQn, (uint32_t)&CAN0_IRQHandler);
        NVIC_EnableIRQ(CAN0_IRQn);
    } else {
        NVIC_SetVector(CAN1_IRQn, (uint32_t)&CAN1_IRQHandler);
        NVIC_EnableIRQ(CAN1_IRQn);
    }

}

int can_write(can_t *obj, CAN_Message msg)
{
    STR_CANMSG_T CMsg;

    CMsg.IdType = (uint32_t)msg.format;
    CMsg.FrameType = (uint32_t)!msg.type;
    CMsg.Id = msg.id;
    CMsg.DLC = msg.len;
    memcpy((void *)&CMsg.Data[0],(const void *)&msg.data[0], (unsigned int)8);

    return CAN_Transmit((CAN_T *)(NU_MODBASE(obj->can)), NU_CAN_MSG_OBJ_NUM_TX, &CMsg);
}

int can_read(can_t *obj, CAN_Message *msg, int handle)
{
    STR_CANMSG_T CMsg;

    if(!CAN_Receive((CAN_T *)(NU_MODBASE(obj->can)), handle, &CMsg))
        return 0;

    msg->format = (CANFormat)CMsg.IdType;
    msg->type = (CANType)!CMsg.FrameType;
    msg->id = CMsg.Id;
    msg->len = CMsg.DLC;
    memcpy(&msg->data[0], &CMsg.Data[0], 8);

    return 1;
}

int can_mode(can_t *obj, CanMode mode)
{
    int success = 0;

    switch (mode) {
    case MODE_RESET:
        CAN_LeaveTestMode((CAN_T*)NU_MODBASE(obj->can));
        success = 1;
        break;

    case MODE_NORMAL:
        CAN_EnterTestMode((CAN_T*)NU_MODBASE(obj->can), CAN_TEST_BASIC_Msk);
        success = 1;
        break;

    case MODE_SILENT:
        CAN_EnterTestMode((CAN_T*)NU_MODBASE(obj->can), CAN_TEST_SILENT_Msk);
        success = 1;
        break;

    case MODE_TEST_LOCAL:
    case MODE_TEST_GLOBAL:
        CAN_EnterTestMode((CAN_T*)NU_MODBASE(obj->can), CAN_TEST_LBACK_Msk);
        success = 1;
        break;

    case MODE_TEST_SILENT:
        CAN_EnterTestMode((CAN_T*)NU_MODBASE(obj->can), CAN_TEST_SILENT_Msk | CAN_TEST_LBACK_Msk);
        success = 1;
        break;

    default:
        success = 0;
        break;

    }

    return success;
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
    /* Check validity of filter handle */
    if (handle < 0 || handle >= NU_CAN_MAXNUM_HNDL) {
        /* NOTE: 0 is ambiguous, error or filter handle 0. */
        error("Support max " NU_STR(NU_CAN_MAXNUM_HNDL) " CAN filters");
        return 0;
    }

    uint32_t numask = mask;
    if( format == CANStandard )
    {
      numask = (mask << 18);
    }
    numask = (numask | ((CAN_IF_MASK2_MDIR_Msk | CAN_IF_MASK2_MXTD_Msk) << 16));
    return CAN_SetRxMsgAndMsk((CAN_T *)NU_MODBASE(obj->can), handle, (uint32_t)format, id, numask);
}


void can_reset(can_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);

    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    // Reset this module
    SYS_ResetModule(modinit->rsetidx);

}

unsigned char can_rderror(can_t *obj)
{
    CAN_T *can = (CAN_T *)NU_MODBASE(obj->can);
    return ((can->ERR>>8)&0xFF);
}

unsigned char can_tderror(can_t *obj)
{
    CAN_T *can = (CAN_T *)NU_MODBASE(obj->can);
    return ((can->ERR)&0xFF);
}

void can_monitor(can_t *obj, int silent)
{
    CAN_EnterTestMode((CAN_T *)NU_MODBASE(obj->can), CAN_TEST_SILENT_Msk);
}

const PinMap *can_rd_pinmap()
{
    return PinMap_CAN_TD;
}

const PinMap *can_td_pinmap()
{
    return PinMap_CAN_RD;
}

#endif // DEVICE_CAN

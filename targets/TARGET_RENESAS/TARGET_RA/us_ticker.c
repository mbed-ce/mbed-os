#include "mbed_error.h"
#include "us_ticker_api.h"
#include "us_ticker_defines.h"
#include "r_gpt.h"
#include "r_gpt_cfg.h"

extern const timer_instance_t US_TICKER_VAR;

static int us_ticker_inited = 0;

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        US_TICKER_CLOCK_HZ,
        US_TICKER_CLOCK_BITS
    };
    return &info;
}

static void us_ticker_gpt_init(void)
{
    fsp_err_t err;

    err = US_TICKER_VAR.p_api->open(US_TICKER_VAR.p_ctrl, US_TICKER_VAR.p_cfg);
    if (FSP_SUCCESS != err && FSP_ERR_ALREADY_OPEN != err) {
        mbed_error(MBED_ERROR_CODE_OPEN_FAILED, "Failed to open GPT channel for us ticker", 0, MBED_FILENAME, __LINE__);
        return;
    }

    err = US_TICKER_VAR.p_api->start(US_TICKER_VAR.p_ctrl);
    if (FSP_SUCCESS != err) {
        mbed_error(MBED_ERROR_CODE_OPEN_FAILED, "Failed to start GPT channel for us ticker", 0, MBED_FILENAME, __LINE__);
        return;
    }
}

static void us_ticker_gpt_set_compare(uint32_t timestamp)
{
    US_TICKER_INSTANCE->GTCCR[0] = (uint32_t)timestamp;
    US_TICKER_INSTANCE->GTST = R_GPT0_GTST_TCFA_Msk;
}

static void us_ticker_gpt_disable_irq(void)
{
    R_BSP_IrqDisable(US_TICKER_IRQ);
}

static void us_ticker_gpt_clear_irq_flag(void)
{
    US_TICKER_INSTANCE->GTST = R_GPT0_GTST_TCFA_Msk;
    R_BSP_IrqClearPending(US_TICKER_IRQ);
}

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }

    us_ticker_gpt_init();

    us_ticker_gpt_disable_irq();
    us_ticker_gpt_clear_irq_flag();

    us_ticker_inited = 1;
}

uint32_t (us_ticker_read)()
{
    /* Invoke the macro */
    return us_ticker_read();
}

void us_ticker_fire_interrupt(void)
{
    us_ticker_gpt_clear_irq_flag();
    R_BSP_IrqEnable(US_TICKER_IRQ);
    us_ticker_irq_handler();
}

void us_ticker_disable_interrupt(void)
{
    us_ticker_gpt_disable_irq();
}

void us_ticker_clear_interrupt(void)
{
    us_ticker_gpt_clear_irq_flag();
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    uint32_t now = us_ticker_read();
    uint32_t delta = (uint32_t)(timestamp - now);

    if (delta <= 1) {
        us_ticker_irq_handler();
        return;
    }

    us_ticker_gpt_set_compare(timestamp);

    R_BSP_IrqEnable(US_TICKER_IRQ);
}

void us_ticker_free(void)
{
    if (!us_ticker_inited) {
        return;
    }

    us_ticker_gpt_disable_irq();

    US_TICKER_VAR.p_api->stop(US_TICKER_VAR.p_ctrl);

    us_ticker_inited = 0;
}

void usticker_compare_match_callback(timer_callback_args_t * p_args)
{
    if(p_args->event == TIMER_EVENT_CAPTURE_A || p_args->event == TIMER_EVENT_CYCLE_END)
    {
        us_ticker_gpt_clear_irq_flag();
        us_ticker_irq_handler();
    }
}

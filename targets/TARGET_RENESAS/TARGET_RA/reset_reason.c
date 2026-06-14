#include "reset_reason_api.h"
#include "bsp_api.h"

/*
 * Reset Source Summary
 *
 * RSTSR0:
 *   PORF      Power-on reset
 *   LVD0RF    LVD0 reset
 *   LVD1RF    LVD1 reset
 *   LVD2RF    LVD2 reset
 *   LVD4RF    LVD4 reset
 *   LVD5RF    LVD5 reset
 *   DPSRSTF   Deep Standby reset
 *
 * RSTSR1:
 *   IWDTRF    Independent WDT reset
 *   WDTRF     WDT reset
 *   SWRF      Software reset
 *   RPERF     RAM parity error reset
 *   REERF     RAM ECC error reset
 *   BUSSRF    Bus slave error reset
 *   BUSMRF    Bus master error reset
 *   SPERF     Stack pointer error reset
 *   TZERF     TrustZone error reset
 *   CPERF     CPU exception reset
 *
 * RSTSR2:
 *   CWSF      Cold/Warm start flag (NOT a reset source)
 *
 * RSTSR3:
 *   CVMRF     Clock/Voltage monitor reset
 *   OCPRF     Overcurrent reset
 *   TEMPRF    Temperature reset
 */

static reset_reason_t ra_map_reset_reason(uint16_t rstsr0,
                                          uint16_t rstsr1,
                                          uint16_t rstsr2,
                                          uint16_t rstsr3)
{
    /* --- RSTSR0: Power / LVD / Deep Standby --- */

    if (rstsr0 & R_SYSTEM_RSTSR0_PORF_Msk) {
        return RESET_REASON_POWER_ON;
    }

    if (rstsr0 & (R_SYSTEM_RSTSR0_LVD0RF_Msk
                  | R_SYSTEM_RSTSR0_LVD1RF_Msk
                  | R_SYSTEM_RSTSR0_LVD2RF_Msk
#ifdef R_SYSTEM_RSTSR0_LVD3RF_Msk
                  | R_SYSTEM_RSTSR0_LVD3RF_Msk
                  | R_SYSTEM_RSTSR0_LVD4RF_Msk
                  | R_SYSTEM_RSTSR0_LVD5RF_Msk
#endif
                )) {
        return RESET_REASON_BROWN_OUT;
    }

    if (rstsr0 & R_SYSTEM_RSTSR0_DPSRSTF_Msk) {
        return RESET_REASON_PLATFORM;   /* Deep Standby exit */
    }

    /* --- RSTSR1: Watchdog / Software / Bus / ECC / CPU errors --- */

    if (rstsr1 & (R_SYSTEM_RSTSR1_IWDTRF_Msk |
                  R_SYSTEM_RSTSR1_WDTRF_Msk)) {
        return RESET_REASON_WATCHDOG;
    }

    if (rstsr1 & R_SYSTEM_RSTSR1_SWRF_Msk) {
        return RESET_REASON_SOFTWARE;
    }

    /* All other RSTSR1 error resets -> PLATFORM */
    if (rstsr1 & ~(R_SYSTEM_RSTSR1_IWDTRF_Msk |
                  R_SYSTEM_RSTSR1_WDTRF_Msk |
                  R_SYSTEM_RSTSR1_SWRF_Msk)) {
        return RESET_REASON_PLATFORM;
    }

    /* --- RSTSR3: Voltage / Overcurrent / Temperature --- */
#ifdef R_SYSTEM_RSTSR3_CVMRF_Msk
    if (rstsr3 & (R_SYSTEM_RSTSR3_CVMRF_Msk |
                  R_SYSTEM_RSTSR3_OCPRF_Msk |
                  R_SYSTEM_RSTSR3_TEMPRF_Msk)) {
        return RESET_REASON_BROWN_OUT;  /* Broadly: power/environment abnormal */
    }
#endif

    if (rstsr0 == 0 && rstsr1 == 0 && rstsr2 == 0 && rstsr3 == 0) {
        return RESET_REASON_PIN_RESET;
    }

    return RESET_REASON_UNKNOWN;
}

reset_reason_t hal_reset_reason_get(void)
{
    uint16_t rstsr0 = R_SYSTEM->RSTSR0;
    uint16_t rstsr1 = R_SYSTEM->RSTSR1;
    uint16_t rstsr2 = R_SYSTEM->RSTSR2;
#ifdef R_SYSTEM_RSTSR3_CVMRF_Msk
    uint16_t rstsr3 = R_SYSTEM->RSTSR3;
#else
    uint16_t rstsr3 = 0;
#endif
    return ra_map_reset_reason(rstsr0, rstsr1, rstsr2, rstsr3);
}

uint32_t hal_reset_reason_get_raw(void)
{
    return ((uint32_t)R_SYSTEM->RSTSR0)        |
           ((uint32_t)R_SYSTEM->RSTSR1 << 8)   |
           ((uint32_t)R_SYSTEM->RSTSR2 << 16)  |
#ifdef R_SYSTEM_RSTSR3_CVMRF_Msk
           ((uint32_t)R_SYSTEM->RSTSR3 << 24)
#else
           0
#endif
    ;
}

void hal_reset_reason_clear(void)
{
    /* RA: write 0 to clear writable flags */
    R_SYSTEM->RSTSR0 = 0;
    R_SYSTEM->RSTSR1 = 0;
    R_SYSTEM->RSTSR2 = 0;   /* CWSF is read-only, writing 0 is harmless */
#ifdef R_SYSTEM_RSTSR3_CVMRF_Msk
    R_SYSTEM->RSTSR3 = 0;
#endif
}

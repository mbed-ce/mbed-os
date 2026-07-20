/* Copyright (c) 2024 Renesas Electronics Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* generated common source file - do not edit */
#include "common_data.h"
icu_instance_ctrl_t g_external_irq14_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq14_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq14_cfg =
{
    .channel             = 14,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq14_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ14)
    .irq                 = VECTOR_NUMBER_ICU_IRQ14,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq14 =
{
    .p_ctrl        = &g_external_irq14_ctrl,
    .p_cfg         = &g_external_irq14_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq13_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq13_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq13_cfg =
{
    .channel             = 13,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq13_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ13)
    .irq                 = VECTOR_NUMBER_ICU_IRQ13,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq13 =
{
    .p_ctrl        = &g_external_irq13_ctrl,
    .p_cfg         = &g_external_irq13_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq12_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq12_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq12_cfg =
{
    .channel             = 12,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq12_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ12)
    .irq                 = VECTOR_NUMBER_ICU_IRQ12,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq12 =
{
    .p_ctrl        = &g_external_irq12_ctrl,
    .p_cfg         = &g_external_irq12_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq11_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq11_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq11_cfg =
{
    .channel             = 11,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq11_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ11)
    .irq                 = VECTOR_NUMBER_ICU_IRQ11,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq11 =
{
    .p_ctrl        = &g_external_irq11_ctrl,
    .p_cfg         = &g_external_irq11_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq10_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq10_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq10_cfg =
{
    .channel             = 10,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq10_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ10)
    .irq                 = VECTOR_NUMBER_ICU_IRQ10,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq10 =
{
    .p_ctrl        = &g_external_irq10_ctrl,
    .p_cfg         = &g_external_irq10_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq9_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq9_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq9_cfg =
{
    .channel             = 9,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq9_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ9)
    .irq                 = VECTOR_NUMBER_ICU_IRQ9,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq9 =
{
    .p_ctrl        = &g_external_irq9_ctrl,
    .p_cfg         = &g_external_irq9_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq8_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq8_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq8_cfg =
{
    .channel             = 8,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq8_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ8)
    .irq                 = VECTOR_NUMBER_ICU_IRQ8,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq8 =
{
    .p_ctrl        = &g_external_irq8_ctrl,
    .p_cfg         = &g_external_irq8_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq7_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq7_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq7_cfg =
{
    .channel             = 7,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq7_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ7)
    .irq                 = VECTOR_NUMBER_ICU_IRQ7,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq7 =
{
    .p_ctrl        = &g_external_irq7_ctrl,
    .p_cfg         = &g_external_irq7_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq6_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq6_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq6_cfg =
{
    .channel             = 6,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq6_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ6)
    .irq                 = VECTOR_NUMBER_ICU_IRQ6,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq6 =
{
    .p_ctrl        = &g_external_irq6_ctrl,
    .p_cfg         = &g_external_irq6_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq5_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq5_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq5_cfg =
{
    .channel             = 5,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq5_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ5)
    .irq                 = VECTOR_NUMBER_ICU_IRQ5,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq5 =
{
    .p_ctrl        = &g_external_irq5_ctrl,
    .p_cfg         = &g_external_irq5_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq4_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq4_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq4_cfg =
{
    .channel             = 4,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq4_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ4)
    .irq                 = VECTOR_NUMBER_ICU_IRQ4,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq4 =
{
    .p_ctrl        = &g_external_irq4_ctrl,
    .p_cfg         = &g_external_irq4_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq3_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq3_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq3_cfg =
{
    .channel             = 3,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq3_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ3)
    .irq                 = VECTOR_NUMBER_ICU_IRQ3,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq3 =
{
    .p_ctrl        = &g_external_irq3_ctrl,
    .p_cfg         = &g_external_irq3_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq2_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq2_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq2_cfg =
{
    .channel             = 2,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq2_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ2)
    .irq                 = VECTOR_NUMBER_ICU_IRQ2,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq2 =
{
    .p_ctrl        = &g_external_irq2_ctrl,
    .p_cfg         = &g_external_irq2_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq1_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq1_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq1_cfg =
{
    .channel             = 1,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq1_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ1)
    .irq                 = VECTOR_NUMBER_ICU_IRQ1,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq1 =
{
    .p_ctrl        = &g_external_irq1_ctrl,
    .p_cfg         = &g_external_irq1_cfg,
    .p_api         = &g_external_irq_on_icu
};
icu_instance_ctrl_t g_external_irq0_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq0_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_external_irq0_cfg =
{
    .channel             = 0,
    .trigger             = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = external_irq_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_external_irq0_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ0)
    .irq                 = VECTOR_NUMBER_ICU_IRQ0,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq0 =
{
    .p_ctrl        = &g_external_irq0_ctrl,
    .p_cfg         = &g_external_irq0_cfg,
    .p_api         = &g_external_irq_on_icu
};
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
        {
            .p_api = &g_ioport_on_ioport,
            .p_ctrl = &g_ioport_ctrl,
            .p_cfg = &g_bsp_pin_cfg,
        };
void g_common_init(void) {
}

/* generated configuration header file - do not edit */
#ifndef BSP_MCU_OFS_CFG_H_
#define BSP_MCU_OFS_CFG_H_
#ifndef BSP_CFG_OPTION_SETTING_OFS0
#define OFS_IWDT (0xA001A001 | 1 << 1 | 3 << 2 | 15 << 4 | 3 << 8 | 3 << 10 | 1 << 12 | 1 << 14)
#define OFS_WDT  (1 << 17 | 3 << 18 | 15 << 20 | 3 << 24 | 3 << 26 | 1 << 28 | 1 << 30)
#define BSP_CFG_OPTION_SETTING_OFS0  (OFS_IWDT | OFS_WDT)
#endif
#ifndef BSP_CFG_OPTION_SETTING_OFS2
#define BSP_CFG_OPTION_SETTING_OFS2  ((1 << 0) | (0xFFFFFFFE))
#endif
#ifndef BSP_CFG_OPTION_SETTING_OFS1_SEC
#define BSP_CFG_OPTION_SETTING_OFS1_SEC_NO_HOCOFRQ (0xFCFFF0D0 | 1 <<3 | 7 | 1 << 5 | 1 << 8 | 1 << 24 | 0 << 25)

#define BSP_CFG_OPTION_SETTING_OFS1_SEC  ((uint32_t) BSP_CFG_OPTION_SETTING_OFS1_SEC_NO_HOCOFRQ | ((uint32_t) BSP_CFG_HOCO_FREQUENCY << BSP_FEATURE_BSP_OFS1_HOCOFRQ_OFFSET))
#endif
#ifndef BSP_CFG_OPTION_SETTING_OFS1_SEL
#if defined(_RA_TZ_SECURE) || defined(_RA_TZ_NONSECURE)
  #define BSP_CFG_OPTION_SETTING_OFS1_SEL  (0 | ((0U << 0U)) | ((0U << 3U)) | ((0U << 5U)) | ((BSP_CFG_CLOCKS_SECURE == 0) ? 0xF00 : 0U) | ((0U << 24U)) | ((0U << 25U)))
#else
  #define BSP_CFG_OPTION_SETTING_OFS1_SEL  (0)
#endif
#endif
#endif /* BSP_MCU_OFS_CFG_H_ */

#ifndef FLASH_DEVICE_H
#define FLASH_DEVICE_H

/* RA6E2: 512KB Code Flash, 8KB Data Flash */
#ifndef RA_CF_START
#define RA_CF_START      (0x00000000u)
#endif
#ifndef RA_CF_SIZE
#define RA_CF_SIZE       (512u * 1024u)
#endif
#ifndef RA_CF_SECTOR_SIZE
#define RA_CF_SECTOR_SIZE (2048u)      /* 2KB */
#endif
#ifndef RA_CF_WRITE_SIZE
#define RA_CF_WRITE_SIZE  (128u)       /* 128B */
#endif

#ifndef RA_DF_START
#define RA_DF_START      (0x08000000u)
#endif
#ifndef RA_DF_SIZE
#define RA_DF_SIZE       (8u * 1024u)
#endif
#ifndef RA_DF_SECTOR_SIZE
#define RA_DF_SECTOR_SIZE (64u)        /* 64B */
#endif
#ifndef RA_DF_WRITE_SIZE
#define RA_DF_WRITE_SIZE  (4u)
#endif

#endif

#ifndef FLASH_DEVICE_H
#define FLASH_DEVICE_H

/* RA8E1: 1MB Code Flash, 32KB Data Flash */
#ifndef RA_CF_START
#define RA_CF_START      (0x02000000u)
#endif
#ifndef RA_CF_SIZE
#define RA_CF_SIZE       (1024u * 1024u)
#endif
#ifndef RA_CF_SECTOR_SIZE
#define RA_CF_SECTOR_SIZE (8192u)      /* 8KB */
#endif
#ifndef RA_CF_WRITE_SIZE
#define RA_CF_WRITE_SIZE  (256u)       /* 256B */
#endif

#ifndef RA_DF_START
#define RA_DF_START      (0x37000000u)
#endif
#ifndef RA_DF_SIZE
#define RA_DF_SIZE       (32u * 1024u)
#endif
#ifndef RA_DF_SECTOR_SIZE
#define RA_DF_SECTOR_SIZE (128u)       /* 128B */
#endif
#ifndef RA_DF_WRITE_SIZE
#define RA_DF_WRITE_SIZE  (8u)
#endif

#endif

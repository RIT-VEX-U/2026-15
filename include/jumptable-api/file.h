#pragma once

#include <stdint.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

typedef enum {
  FR_OK                  = 0,
  FR_DISK_ERR            = 1,
  FR_INT_ERR             = 2,
  FR_NOT_READY           = 3,
  FR_NO_FILE             = 4,
  FR_NO_PATH             = 5,
  FR_INVALID_NAME        = 6,
  FR_DENIED              = 7,
  FR_EXIST               = 8,
  FR_INVALID_OBJECT      = 9,
  FR_WRITE_PROTECTED     = 10,
  FR_INVALID_DRIVE       = 11,
  FR_NOT_ENABLED         = 12,
  FR_NO_FILESYSTEM       = 13,
  FR_MKFS_ABORTED        = 14,
  FR_TIMEOUT             = 15,
  FR_LOCKED              = 16,
  FR_NOT_ENOUGH_CORE     = 17,
  FR_TOO_MANY_OPEN_FILES = 18,
  FR_INVALID_PARAMETER   = 19,
} FRESULT;

typedef void FIL;

JUMPTABLE_FUNC(vexFileMountSD, FRESULT, (void), (), 0x7d0)
JUMPTABLE_FUNC(vexFileDirectoryGet, FRESULT, (const char *path, char *buffer, uint32_t len), (path, buffer, len), 0x7d4)
JUMPTABLE_FUNC(vexFileOpen, FIL *, (const char *filename, const char *mode), (filename, mode), 0x7d8)
JUMPTABLE_FUNC(vexFileOpenWrite, FIL *, (const char *filename), (filename), 0x7dc)
JUMPTABLE_FUNC(vexFileOpenCreate, FIL *, (const char *filename), (filename), 0x7e0)
JUMPTABLE_FUNC(vexFileClose, void, (FIL *fdp), (fdp), 0x7e4)
JUMPTABLE_FUNC(vexFileWrite, int32_t, (char *buf, uint32_t size, uint32_t nItems, FIL *fdp), (buf, size, nItems, fdp), 0x7ec)
JUMPTABLE_FUNC(vexFileSize, int32_t, (FIL *fdp), (fdp), 0x7f0)
JUMPTABLE_FUNC(vexFileSeek, FRESULT, (FIL *fdp, uint32_t offset, int32_t whence), (fdp, offset, whence), 0x7f4)
JUMPTABLE_FUNC(vexFileRead, int32_t, (char *buf, uint32_t size, uint32_t nItems, FIL *fdp), (buf, size, nItems, fdp), 0x7f8)
JUMPTABLE_FUNC(vexFileDriveStatus, bool, (uint32_t drive), (drive), 0x7fc)
JUMPTABLE_FUNC(vexFileTell, int32_t, (FIL *fdp), (fdp), 0x800)
JUMPTABLE_FUNC(vexFileSync, void, (FIL *fdp), (fdp), 0x804)
JUMPTABLE_FUNC(vexFileStatus, uint32_t, (const char *filename), (filename), 0x808)

#ifdef __cplusplus
}
#endif


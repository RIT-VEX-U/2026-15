#pragma once

#include <stdint.h>
#include <stdarg.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

#pragma pack(push,1)
typedef struct {
  uint16_t width;
  uint16_t height;
  uint32_t *data; // buffer of rgb8 pixels, it must be set before the read operation as a pointer to the pre-allocated pixel buffer, after an image read, the pixels are written here
  uint32_t *p;    // points to the first pixel of the second row in the buffer, only set after vexImageBmpRead
} v5_image;
#pragma pack(pop)

JUMPTABLE_FUNC(vexDisplayForegroundColor, void, (uint32_t col), (col), 0x640)
JUMPTABLE_FUNC(vexDisplayBackgroundColor, void, (uint32_t col), (col), 0x644)
JUMPTABLE_FUNC(vexDisplayErase, void, (void), (), 0x648)
JUMPTABLE_FUNC(vexDisplayScroll, void, (int32_t nStartLine, int32_t nLines), (nStartLine, nLines), 0x64c)
JUMPTABLE_FUNC(vexDisplayScrollRect, void, (int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t nLines), (x1, y1, x2, y2, nLines), 0x650)
JUMPTABLE_FUNC(vexDisplayCopyRect, void, (int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t *pSrc, int32_t srcStride), (x1, y1, x2, y2, pSrc, srcStride), 0x654)
JUMPTABLE_FUNC(vexDisplayPixelSet, void, (uint32_t x, uint32_t y), (x, y), 0x658)
JUMPTABLE_FUNC(vexDisplayPixelClear, void, (uint32_t x, uint32_t y), (x, y), 0x65c)
JUMPTABLE_FUNC(vexDisplayLineDraw, void, (int32_t x1, int32_t y1, int32_t x2, int32_t y2), (x1, y1, x2, y2), 0x660)
JUMPTABLE_FUNC(vexDisplayLineClear, void, (int32_t x1, int32_t y1, int32_t x2, int32_t y2), (x1, y1, x2, y2), 0x664)
JUMPTABLE_FUNC(vexDisplayRectDraw, void, (int32_t x1, int32_t y1, int32_t x2, int32_t y2), (x1, y1, x2, y2), 0x668)
JUMPTABLE_FUNC(vexDisplayRectClear, void, (int32_t x1, int32_t y1, int32_t x2, int32_t y2), (x1, y1, x2, y2), 0x66c)
JUMPTABLE_FUNC(vexDisplayRectFill, void, (int32_t x1, int32_t y1, int32_t x2, int32_t y2), (x1, y1, x2, y2), 0x670)
JUMPTABLE_FUNC(vexDisplayCircleDraw, void, (int32_t xc, int32_t yc, int32_t radius), (xc, yc, radius), 0x674)
JUMPTABLE_FUNC(vexDisplayCircleClear, void, (int32_t xc, int32_t yc, int32_t radius), (xc, yc, radius), 0x678)
JUMPTABLE_FUNC(vexDisplayCircleFill, void, (int32_t xc, int32_t yc, int32_t radius), (xc, yc, radius), 0x67c)
JUMPTABLE_FUNC(vexDisplayTextSize, void, (uint32_t n, uint32_t d), (n, d), 0x6a8)
JUMPTABLE_FUNC(vexDisplayFontNamedSet, void, (const char *pFontName), (pFontName), 0x6b4)
JUMPTABLE_FUNC(vexDisplayForegroundColorGet, uint32_t, (void), (), 0x6b8)
JUMPTABLE_FUNC(vexDisplayBackgroundColorGet, uint32_t, (void), (), 0x6bc)
JUMPTABLE_FUNC(vexDisplayStringWidthGet, int32_t, (const char *pString), (pString), 0x6c0)
JUMPTABLE_FUNC(vexDisplayStringHeightGet, int32_t, (const char *pString), (pString), 0x6c4)
JUMPTABLE_FUNC(vexDisplayClipRegionSet, void, (int32_t x1, int32_t y1, int32_t x2, int32_t y2), (x1, y1, x2, y2), 0x794)
JUMPTABLE_FUNC(vexDisplayRender, void, (bool bVsyncWait, bool bRunScheduler), (bVsyncWait, bRunScheduler), 0x7a0)
JUMPTABLE_FUNC(vexDisplayDoubleBufferDisable, void, (void), (), 0x7a4)
JUMPTABLE_FUNC(vexDisplayClipRegionSetWithIndex, void, (int32_t index, int32_t x1, int32_t y1, int32_t x2, int32_t y2), (index, x1, y1, x2, y2), 0x7a8)
JUMPTABLE_FUNC(vexImageBmpRead, uint32_t, (const uint8_t *ibuf, v5_image *oBuf, uint32_t maxw, uint32_t maxh), (ibuf, oBuf, maxw, maxh), 0x990)
JUMPTABLE_FUNC(vexImagePngRead, uint32_t, (const uint8_t *ibuf, v5_image *oBuf, uint32_t maxw, uint32_t maxh, uint32_t ibuflen), (ibuf, oBuf, maxw, maxh, ibuflen), 0x994)
JUMPTABLE_FUNC(vexDisplayVPrintf, void, (int32_t xpos, int32_t ypos, int32_t bOpaque, const char *format, va_list args), (xpos, ypos, bOpaque, format, args), 0x680)
JUMPTABLE_FUNC(vexDisplayVString, void, (int32_t nLineNumber, const char *format, va_list args), (nLineNumber, format, args), 0x684)
JUMPTABLE_FUNC(vexDisplayVStringAt, void, (int32_t xpos, int32_t ypos, const char *format, va_list args), (xpos, ypos, format, args), 0x688)
JUMPTABLE_FUNC(vexDisplayVBigString, void, (int32_t nLineNumber, const char *format, va_list args), (nLineNumber, format, args), 0x68c)
JUMPTABLE_FUNC(vexDisplayVBigStringAt, void, (int32_t xpos, int32_t ypos, const char *format, va_list args), (xpos, ypos, format, args), 0x690)
JUMPTABLE_FUNC(vexDisplayVSmallStringAt, void, (int32_t xpos, int32_t ypos, const char *format, va_list args), (xpos, ypos, format, args), 0x6b0)
JUMPTABLE_FUNC(vexDisplayVCenteredString, void, (int32_t nLineNumber, const char *format, va_list args), (nLineNumber, format, args), 0x694)
JUMPTABLE_FUNC(vexDisplayVBigCenteredString, void, (int32_t nLineNumber, const char *format, va_list args), (nLineNumber, format, args), 0x698)

void vexDisplayPrintf(int32_t xpos, int32_t ypos, int32_t bOpaque, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vexDisplayVPrintf(xpos, ypos, bOpaque, format, args);
    va_end(args);
}

void vexDisplayString(int32_t nLineNumber, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vexDisplayVString(nLineNumber, format, args);
    va_end(args);
}

void vexDisplayStringAt(int32_t xpos, int32_t ypos, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vexDisplayVStringAt(xpos, ypos, format, args);
    va_end(args);
}

void vexDisplayBigString(int32_t nLineNumber, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vexDisplayVBigString(nLineNumber, format, args);
    va_end(args);
}

void vexDisplayBigStringAt(int32_t xpos, int32_t ypos, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vexDisplayVBigStringAt(xpos, ypos, format, args);
    va_end(args);
}

void vexDisplaySmallStringAt(int32_t xpos, int32_t ypos, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vexDisplayVSmallStringAt(xpos, ypos, format, args);
    va_end(args);
}

void vexDisplayCenteredString(int32_t nLineNumber, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vexDisplayVCenteredString(nLineNumber, format, args);
    va_end(args);
}

void vexDisplayBigCenteredString(int32_t nLineNumber, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vexDisplayVBigCenteredString(nLineNumber, format, args);
    va_end(args);
}

#ifdef __cplusplus
}
#endif



/*
 * SVGA
 *
 */

#ifdef USE_VESA

#include "./svga.h"

#include "Machine.h"
#include "PageManager.h"

namespace video {
namespace svga {


int8_t* screen;
int8_t bytesPerPixel;
VbeModeInfo* modeInfo;
int32_t videoMemSize;
int32_t vMemPages;

void init(VbeModeInfo* modeInfo) {
    svga::modeInfo = modeInfo;
    svga::screen = (int8_t*) (VESA_SCREEN_VADDR);
    bytesPerPixel = modeInfo->bpp / 8;
    videoMemSize = bytesPerPixel * modeInfo->width * modeInfo->height;
    vMemPages = (videoMemSize + PageManager::PAGE_SIZE - 1) / PageManager::PAGE_SIZE;
}


void clear(int32_t color) {
    for (int i = 0; i < modeInfo->pitch / bytesPerPixel * modeInfo->height; i++) {
        int8_t* pixel = screen + i * bytesPerPixel;
        pixel[0] = color & 0xff;
        pixel[1] = (color >> 8) & 0xff;
        pixel[2] = color >> 16;
    }
}




}  // namespace svga
}  // namespace video

#endif

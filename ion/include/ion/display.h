#ifndef ION_DISPLAY_H
#define ION_DISPLAY_H

/* ION abstracts pushing pixels to the screen.
 *
 * There could be a single entry point, set_pixel, but setting pixels one by one
 * incurs quite a large overhead because you need to send the coordinate of each
 * pixel to the screen.
 *
 * Many displays support sending contiguous pixels without having to repeat the
 * pixel coordinates every time. We're therefore leveraging this capability
 * which results in a very consequent speedup (up to ~10x faster). */

#include <kandinsky/rect.h>
#include <kandinsky/color.h>

namespace Ion {
namespace Display {

void pushRect(KDRect r, const KDColor * pixels);
void pushRectUniform(KDRect r, KDColor c);
void pullRect(KDRect r, KDColor * pixels);

bool waitForVBlank();

constexpr int Width = 320;
constexpr int Height = 240;
constexpr int WidthInTenthOfMillimeter = 576;
constexpr int HeightInTenthOfMillimeter = 432;

// For Power On Self tests
int displayUniformTilingSize10(KDColor c);
int displayColoredTilingSize10();
void POSTPushMulticolor(int rootNumberTiles, int tileSize);

}
}

#endif

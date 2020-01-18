#ifndef SHARED_STORE_TITLE_CELL_H
#define SHARED_STORE_TITLE_CELL_H

#include "buffer_function_title_cell.h"
#include "separable.h"

namespace Shared {

class StoreTitleCell : public BufferFunctionTitleCell, public Separable {
public:
  StoreTitleCell() :
    BufferFunctionTitleCell(Orientation::HorizontalIndicator, KDFont::SmallFont),
    Separable()
  {}
  void drawRect(KDContext * ctx, KDRect rect) const override;
  void layoutSubviews() override;
private:
  void didSetSeparator() override;
};

}

#endif

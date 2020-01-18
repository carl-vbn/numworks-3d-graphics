#ifndef APPS_MATH_TOOLBOX_H
#define APPS_MATH_TOOLBOX_H

#include <escher.h>
#include <apps/i18n.h>

class MathToolbox : public Toolbox {
public:
  MathToolbox();
protected:
  bool selectLeaf(int selectedRow) override;
  const ToolboxMessageTree * rootModel() const override;
  MessageTableCellWithMessage * leafCellAtIndex(int index) override;
  MessageTableCellWithChevron* nodeCellAtIndex(int index) override;
  int maxNumberOfDisplayedRows() override;
  constexpr static int k_maxNumberOfDisplayedRows = 6; // = 240/40
private:
  MessageTableCellWithMessage m_leafCells[k_maxNumberOfDisplayedRows];
  MessageTableCellWithChevron m_nodeCells[k_maxNumberOfDisplayedRows];
};

#endif

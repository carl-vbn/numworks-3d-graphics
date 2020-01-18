#ifndef ESCHER_EXPRESSION_TABLE_CELL_H
#define ESCHER_EXPRESSION_TABLE_CELL_H

#include <escher/expression_view.h>
#include <escher/table_cell.h>

class ExpressionTableCell : public TableCell {
public:
  ExpressionTableCell(Layout layout = Layout::Horizontal);
  View * labelView() const override;
  void setHighlighted(bool highlight) override;
  void setLayout(Poincare::Layout layout);
  Poincare::Layout layout() const override { return m_labelExpressionView.layout(); }
private:
  ExpressionView m_labelExpressionView;
};

#endif

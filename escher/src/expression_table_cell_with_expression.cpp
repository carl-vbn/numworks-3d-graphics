#include <escher/expression_table_cell_with_expression.h>
#include <escher/palette.h>
#include <assert.h>

ExpressionTableCellWithExpression::ExpressionTableCellWithExpression() :
  ExpressionTableCell(Layout::Horizontal),
  m_accessoryExpressionView(1.0f, 0.5f, Palette::GreyDark, KDColorWhite)
{}

View * ExpressionTableCellWithExpression::accessoryView() const {
  return (View *)&m_accessoryExpressionView;
}

void ExpressionTableCellWithExpression::setHighlighted(bool highlight) {
  ExpressionTableCell::setHighlighted(highlight);
  KDColor backgroundColor = highlight? Palette::Select : KDColorWhite;
  m_accessoryExpressionView.setBackgroundColor(backgroundColor);
}

void ExpressionTableCellWithExpression::setAccessoryLayout(Poincare::Layout l) {
  m_accessoryExpressionView.setLayout(l);
  layoutSubviews();
}

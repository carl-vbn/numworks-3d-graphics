#ifndef APPS_STATISTICS_CALCULATION_SELECTABLE_TABLE_VIEW_H
#define APPS_STATISTICS_CALCULATION_SELECTABLE_TABLE_VIEW_H

#include <escher/selectable_table_view.h>

namespace Statistics {

class CalculationSelectableTableView : public SelectableTableView {
public:
  using SelectableTableView::SelectableTableView;
  bool handleEvent(Ion::Events::Event event) override;
};

}

#endif

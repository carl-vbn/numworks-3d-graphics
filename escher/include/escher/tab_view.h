#ifndef ESCHER_TAB_VIEW_H
#define ESCHER_TAB_VIEW_H

#include <escher/view.h>
#include <escher/i18n.h>
#include <escher/tab_view_cell.h>
#include <escher/view_controller.h>

class TabViewController;

class TabView : public View {
public:
  TabView();
  int numberOfTabs() const;
  void drawRect(KDContext * ctx, KDRect rect) const override;

  void addTab(ViewController * controller);
  //TODO: void removeLastTab();
  void setActiveIndex(int index);
  void setSelectedIndex(int index);
protected:
#if ESCHER_VIEW_LOGGING
  const char * className() const override;
  void logAttributes(std::ostream &os) const override;
#endif
private:
  constexpr static KDCoordinate k_activeTabHeight = 5;
  int numberOfSubviews() const override;
  View * subviewAtIndex(int index) override;
  void layoutSubviews() override;

  constexpr static uint8_t k_maxNumberOfTabs = 4;
  TabViewCell m_cells[k_maxNumberOfTabs];
  uint8_t m_numberOfTabs;
  int8_t m_activeTabIndex;
  int8_t m_selectedTabIndex;
};

#endif

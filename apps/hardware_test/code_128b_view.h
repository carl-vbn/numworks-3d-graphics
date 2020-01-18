#ifndef HARDWARE_TEST_CODE_128B_VIEW_H
#define HARDWARE_TEST_CODE_128B_VIEW_H

#include <escher.h>

namespace HardwareTest {

class Code128BView : public View {
public:
  Code128BView();
  void drawRect(KDContext * ctx, KDRect rect) const override;
  void setData(const char * data);
  void layoutSubviews() override;
private:
  static constexpr KDCoordinate k_outlineThickness = 1;
  static constexpr KDCoordinate k_charPatternWidth = 11;
  static constexpr KDCoordinate k_stringOffset = 3;
  static constexpr KDColor k_borderColor = KDColorRed;
  void updateModuleWidth();
  int checksum() const;
  void drawQuietZoneAt(KDContext * ctx, KDCoordinate * x) const;
  void drawPatternAt(KDContext * ctx, uint16_t pattern, KDCoordinate * x, KDCoordinate width = k_charPatternWidth) const;
  void drawCharAt(KDContext * ctx, char c, KDCoordinate * x) const;
  int m_moduleWidth;
  const char * m_data;
};

}

#endif


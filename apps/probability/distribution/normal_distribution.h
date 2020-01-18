#ifndef PROBABILITE_NORMAL_DISTRIBUTION_H
#define PROBABILITE_NORMAL_DISTRIBUTION_H

#include "two_parameter_distribution.h"

namespace Probability {

class NormalDistribution final : public TwoParameterDistribution {
public:
  NormalDistribution() : TwoParameterDistribution(0.0f, 1.0f) {}
  I18n::Message title() override { return I18n::Message::NormalDistribution; }
  Type type() const override { return Type::Normal; }
  bool isContinuous() const override { return true; }
  float xMin() const override { return xExtremum(true); }
  float xMax() const override { return xExtremum(false); }
  float yMax() const override;
  I18n::Message parameterNameAtIndex(int index) override;
  I18n::Message parameterDefinitionAtIndex(int index) override;
  float evaluateAtAbscissa(float x) const override;
  bool authorizedValueAtIndex(float x, int index) const override;
  void setParameterAtIndex(float f, int index) override;
  double cumulativeDistributiveFunctionAtAbscissa(double x) const override;
  double cumulativeDistributiveInverseForProbability(double * probability) override;
private:
  constexpr static double k_maxRatioMuSigma = 1000000.0f;
  float xExtremum(bool min) const;
};

}

#endif

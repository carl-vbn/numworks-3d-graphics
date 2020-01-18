#ifndef PROBABILITY_STUDENT_DISTRIBUTION_H
#define PROBABILITY_STUDENT_DISTRIBUTION_H

#include "one_parameter_distribution.h"
#include <float.h>

namespace Probability {

class StudentDistribution : public OneParameterDistribution {
public:
  StudentDistribution() : OneParameterDistribution(1.0f) {}
  I18n::Message title() override { return I18n::Message::StudentDistribution; }
  Type type() const override { return Type::Student; }
  bool isContinuous() const override { return true; }
  float xMin() const override;
  float xMax() const override;
  float yMax() const override;
  I18n::Message parameterNameAtIndex(int index) override {
    assert(index == 0);
    return I18n::Message::K;
  }
  I18n::Message parameterDefinitionAtIndex(int index) override {
    assert(index == 0);
    return I18n::Message::DegreesOfFreedomDefinition;
  }
  float evaluateAtAbscissa(float x) const override;
  bool authorizedValueAtIndex(float x, int index) const override;
  double cumulativeDistributiveFunctionAtAbscissa(double x) const override;
  double cumulativeDistributiveInverseForProbability(double * probability) override;
private:
  float lnCoefficient() const;
};

}

#endif

#ifndef REGRESSION_LOGISTIC_MODEL_H
#define REGRESSION_LOGISTIC_MODEL_H

#include "model.h"

namespace Regression {

class LogisticModel : public Model {
public:
  using Model::Model;
  Poincare::Layout layout() override;
  I18n::Message formulaMessage() const override { return I18n::Message::LogisticRegressionFormula; }
  double evaluate(double * modelCoefficients, double x) const override;
  double levelSet(double * modelCoefficients, double xMin, double step, double xMax, double y, Poincare::Context * context) override;
  double partialDerivate(double * modelCoefficients, int derivateCoefficientIndex, double x) const override;
  int numberOfCoefficients() const override { return 3; }
  int bannerLinesCount() const override { return 3; }
};

}


#endif

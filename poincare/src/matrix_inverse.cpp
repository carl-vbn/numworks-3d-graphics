#include <poincare/matrix_inverse.h>
#include <poincare/division.h>
#include <poincare/layout_helper.h>
#include <poincare/matrix.h>
#include <poincare/power.h>
#include <poincare/rational.h>
#include <poincare/serialization_helper.h>
#include <poincare/undefined.h>
#include <cmath>

namespace Poincare {

constexpr Expression::FunctionHelper MatrixInverse::s_functionHelper;

int MatrixInverseNode::numberOfChildren() const { return MatrixInverse::s_functionHelper.numberOfChildren(); }

Expression MatrixInverseNode::shallowReduce(ReductionContext reductionContext) {
  return MatrixInverse(this).shallowReduce(reductionContext);
}

Layout MatrixInverseNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return LayoutHelper::Prefix(MatrixInverse(this), floatDisplayMode, numberOfSignificantDigits, MatrixInverse::s_functionHelper.name());
}

int MatrixInverseNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, MatrixInverse::s_functionHelper.name());
}

template<typename T>
Evaluation<T> MatrixInverseNode::templatedApproximate(Context * context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) const {
  Evaluation<T> input = childAtIndex(0)->approximate(T(), context, complexFormat, angleUnit);
  Evaluation<T> inverse;
  if (input.type() == EvaluationNode<T>::Type::MatrixComplex) {
    inverse = static_cast<MatrixComplex<T>&>(input).inverse();
  } else if (input.type() == EvaluationNode<T>::Type::Complex) {
    inverse = Complex<T>::Builder(std::complex<T>(1)/(static_cast<Complex<T>&>(input).stdComplex()));
  }
  if (inverse.isUninitialized()) {
    inverse = Complex<T>::Undefined();
  }
  return inverse;
}


Expression MatrixInverse::shallowReduce(ExpressionNode::ReductionContext reductionContext) {
  {
    Expression e = Expression::defaultShallowReduce();
    if (e.isUndefined()) {
      return e;
    }
  }
  Expression c = childAtIndex(0);
  if (c.type() == ExpressionNode::Type::Matrix) {
    /* Power(matrix, -n) creates a matrixInverse, so the simplification must be
     * done here and not in power. */
    bool couldComputeInverse = false;
    Expression result = static_cast<Matrix&>(c).createInverse(reductionContext, &couldComputeInverse);
    if (couldComputeInverse) {
      replaceWithInPlace(result);
      return result.shallowReduce(reductionContext);
    }
    // The matrix could not be inverted exactly
    // TODO Poincare error?
    return *this;
  }
  Expression result = Power::Builder(c, Rational::Builder(-1));
  replaceWithInPlace(result);
  return result.shallowReduce(reductionContext);
}

}

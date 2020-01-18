#include <poincare/matrix_identity.h>
#include <poincare/integer.h>
#include <poincare/layout_helper.h>
#include <poincare/matrix.h>
#include <poincare/matrix_complex.h>
#include <poincare/rational.h>
#include <poincare/serialization_helper.h>
#include <poincare/undefined.h>
#include <cmath>
#include <limits.h>

namespace Poincare {

constexpr Expression::FunctionHelper MatrixIdentity::s_functionHelper;

int MatrixIdentityNode::numberOfChildren() const { return MatrixIdentity::s_functionHelper.numberOfChildren(); }

Expression MatrixIdentityNode::shallowReduce(ReductionContext reductionContext) {
  return MatrixIdentity(this).shallowReduce(reductionContext);
}

Layout MatrixIdentityNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return LayoutHelper::Prefix(MatrixIdentity(this), floatDisplayMode, numberOfSignificantDigits, MatrixIdentity::s_functionHelper.name());
}

int MatrixIdentityNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, MatrixIdentity::s_functionHelper.name());
}

template<typename T>
Evaluation<T> MatrixIdentityNode::templatedApproximate(Context * context, Preferences::ComplexFormat complexFormat, Preferences::AngleUnit angleUnit) const {
  Evaluation<T> input = childAtIndex(0)->approximate(T(), context, complexFormat, angleUnit);
  T r = input.toScalar(); // Undefined if the child is not real
  if (!std::isnan(r) && !std::isinf(r) && r > (T)0.0 // The child is defined and positive
      && std::ceil(r) == std::floor(r) // The child is an integer
      &&  r < ((float) INT_MAX)) // The child is not too big
  {
    return MatrixComplex<T>::CreateIdentity((int)r);
  }
  return Complex<T>::Undefined();
}


Expression MatrixIdentity::shallowReduce(ExpressionNode::ReductionContext reductionContext) {
  {
    Expression e = Expression::defaultShallowReduce();
    if (e.isUndefined()) {
      return e;
    }
  }
  Expression c = childAtIndex(0);
  if (c.type() != ExpressionNode::Type::Rational
      || !static_cast<Rational&>(c).isInteger())
  {
    return *this;
  }
  Integer dimension = static_cast<Rational &>(c).signedIntegerNumerator();
  if (dimension.isNegative() || dimension.isZero()) {
    return replaceWithUndefinedInPlace();
  }
  if (Integer::NaturalOrder(dimension, Integer(15)) > 0) { // For now, after 15 it would fill the pool
    return *this;
  }
  int dim = dimension.extractedInt();
  assert(dim != 0);
  Expression result = Matrix::CreateIdentity(dim);
  replaceWithInPlace(result);
  return result;
}

}

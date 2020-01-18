#include <poincare/hyperbolic_trigonometric_function.h>
#include <poincare/multiplication.h>
#include <poincare/rational.h>

namespace Poincare {

Expression HyperbolicTrigonometricFunctionNode::shallowReduce(ReductionContext reductionContext) {
  return HyperbolicTrigonometricFunction(this).shallowReduce(reductionContext);
}

Expression HyperbolicTrigonometricFunction::shallowReduce(ExpressionNode::ReductionContext reductionContext) {
  {
    Expression e = Expression::defaultShallowReduce();
    if (e.isUndefined()) {
      return e;
    }
  }

  Expression thisExpression = *this;
  Expression c = childAtIndex(0);

  // Step 0. Map on matrix child if possible
  if (c.type() == ExpressionNode::Type::Matrix) {
    return mapOnMatrixFirstChild(reductionContext);
  }

  // Step 1. Notable values
  if (node()->isNotableValue(c)) {
    Expression result = node()->imageOfNotableValue();
    replaceWithInPlace(result);
    return result;
  }

  // Step 2. Look for an expression of type "cosh(acosh(z))", return z
  ExpressionNode::Type t = type();
  ExpressionNode::Type childT = c.type();
  {
    Expression result;
    if (t == ExpressionNode::Type::HyperbolicCosine) {
      if (childT == ExpressionNode::Type::HyperbolicArcCosine) {
        Expression e = childAtIndex(0).childAtIndex(0);
        if (reductionContext.complexFormat() != Preferences::ComplexFormat::Real
            || (e.isReal(reductionContext.context())
              && e.approximateToScalar<double>(
                reductionContext.context(),
                reductionContext.complexFormat(),
                reductionContext.angleUnit()) >= 1.0))
        {
          result = e;
        }
      }
    } else if (t == ExpressionNode::Type::HyperbolicArcCosine) {
      if (childT == ExpressionNode::Type::HyperbolicCosine) {
        Expression e = childAtIndex(0).childAtIndex(0);
        if (reductionContext.complexFormat() != Preferences::ComplexFormat::Real
            || (e.isReal(reductionContext.context())
              && e.approximateToScalar<double>(
                reductionContext.context(),
                reductionContext.complexFormat(),
                reductionContext.angleUnit()) >= 0.0))
        {
          result = e;
        }
      }
    } else if (t == ExpressionNode::Type::HyperbolicSine) {
      if (childT == ExpressionNode::Type::HyperbolicArcSine) {
        result = childAtIndex(0).childAtIndex(0);
      }
    } else if (t == ExpressionNode::Type::HyperbolicArcSine) {
      if (childT == ExpressionNode::Type::HyperbolicSine) {
        result = childAtIndex(0).childAtIndex(0);
      }
    } else if (t == ExpressionNode::Type::HyperbolicTangent) {
      if (childT == ExpressionNode::Type::HyperbolicArcTangent) {
        Expression e = childAtIndex(0).childAtIndex(0);
        if (reductionContext.complexFormat() != Preferences::ComplexFormat::Real
            || (e.isReal(reductionContext.context())
              && std::fabs(e.approximateToScalar<double>(
                reductionContext.context(),
                reductionContext.complexFormat(),
                reductionContext.angleUnit())) < 1.0))
        {
          result = e;
        }
      }
    } else {
      assert(t == ExpressionNode::Type::HyperbolicArcTangent);
      if (childT == ExpressionNode::Type::HyperbolicTangent) {
        result = childAtIndex(0).childAtIndex(0);
      }
    }
    if (!result.isUninitialized()) {
      replaceWithInPlace(result);
      return result;
    }
  }

  // Step 3. Look for an expression of type "cosh(-x)", return "+/-cosh(x)"
  if (t != ExpressionNode::Type::HyperbolicArcCosine) {
    Expression positiveArg = childAtIndex(0).makePositiveAnyNegativeNumeralFactor(reductionContext);
    if (!positiveArg.isUninitialized()) {
      // The argument was of form cosh(-a)
      if (t == ExpressionNode::Type::HyperbolicCosine) {
        // cosh(-a) = cosh(a)
        return shallowReduce(reductionContext);
      } else {
        /* sinh(-a) = -sinh(a) or tanh(-a) = -tanh(a) or arcsinh(-a) = -arcsinh(a)
         * or arctanh(-a) = -arctanh(a) */
        assert(t == ExpressionNode::Type::HyperbolicSine || t == ExpressionNode::Type::HyperbolicArcSine || t == ExpressionNode::Type::HyperbolicTangent || t == ExpressionNode::Type::HyperbolicArcTangent);
        Multiplication m = Multiplication::Builder(Rational::Builder(-1));
        thisExpression.replaceWithInPlace(m);
        m.addChildAtIndexInPlace(thisExpression, 1, 1);
        thisExpression.shallowReduce(reductionContext);
        return m.shallowReduce(reductionContext);
      }
    }
  }
  return *this;
}

}

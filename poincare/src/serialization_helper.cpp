#include <poincare/serialization_helper.h>
#include <ion/unicode/utf8_decoder.h>
#include <ion/unicode/utf8_helper.h>
#include <string.h>
#include <assert.h>

namespace Poincare {

void replaceOneCharSizedCodePointWith(char * buffer, CodePoint searchedCodePoint, CodePoint newCodePoint) {
  assert(UTF8Decoder::CharSizeOfCodePoint(searchedCodePoint == 1));
  assert(UTF8Decoder::CharSizeOfCodePoint(newCodePoint == 1));
  UTF8Helper::PerformAtCodePoints(
      buffer,
      searchedCodePoint,
      [](int codePointOffset, void * text, int newCodePoint, int bufferLength) {
        *((char *)text+codePointOffset) = (char)newCodePoint;
      },
      [](int c1, void * c2, int c3, int c4) {},
      (void *)buffer,
      newCodePoint);
}

void SerializationHelper::ReplaceSystemParenthesesByUserParentheses(char * buffer) {
  replaceOneCharSizedCodePointWith(buffer, UCodePointLeftSystemParenthesis, '(');
  replaceOneCharSizedCodePointWith(buffer, UCodePointRightSystemParenthesis, ')');
}

static bool checkBufferSize(char * buffer, int bufferSize, int * result) {
  // If buffer has size 0 or 1, put a zero if it fits and return
  if (bufferSize == 0) {
    *result = -1;
    return true;
  }

  buffer[bufferSize-1] = 0; // Null-terminate the buffer
  if (bufferSize == 1) {
    *result = 0;
    return true;
  }
  return false;
}

int SerializationHelper::SerializeChild(
    const TreeNode * childNode,
    const TreeNode * parentNode,
    char * buffer,
    int bufferSize,
    Preferences::PrintFloatMode floatDisplayMode,
    int numberOfDigits)
{
  {
    int result = 0;
    if (checkBufferSize(buffer, bufferSize, &result)) {
      return result;
    }
  }

  int numberOfChar = 0;
  // Write the child with parentheses if needed
  bool addParentheses = parentNode->childNeedsSystemParenthesesAtSerialization(childNode);
  if (addParentheses) {
    numberOfChar += SerializationHelper::CodePoint(buffer+numberOfChar, bufferSize - numberOfChar, UCodePointLeftSystemParenthesis);
    if (numberOfChar >= bufferSize-1) {
      return bufferSize-1;
    }
  }
  numberOfChar+= childNode->serialize(buffer + numberOfChar, bufferSize - numberOfChar, floatDisplayMode, numberOfDigits);
  if (numberOfChar >= bufferSize-1) {
    assert(buffer[bufferSize - 1] == 0);
    return bufferSize-1;
  }
  if (addParentheses) {
    numberOfChar += SerializationHelper::CodePoint(buffer+numberOfChar, bufferSize - numberOfChar, UCodePointRightSystemParenthesis);
    if (numberOfChar >= bufferSize-1) {
      assert(buffer[bufferSize - 1] == 0);
      return bufferSize-1;
    }
  }
  assert(buffer[numberOfChar] == 0);
  return numberOfChar;
}

int InfixPrefix(
    bool prefix,
    const TreeNode * node,
    char * buffer,
    int bufferSize,
    Preferences::PrintFloatMode floatDisplayMode,
    int numberOfDigits,
    const char * operatorName,
    bool needsSystemParentheses,
    int firstChildIndex,
    int lastChildIndex)
{
  {
    int result = 0;
    if (checkBufferSize(buffer, bufferSize, &result)) {
      return result;
    }
  }

  int numberOfChar = 0;

  /* For Prefix, we use system parentheses so that, for instance, |3)+(1| is not
   * parsable after serialization.*/

  if (prefix) {
    // Prefix: Copy the operator name
    numberOfChar = strlcpy(buffer, operatorName, bufferSize);
    if (numberOfChar >= bufferSize-1) {
      assert(buffer[bufferSize - 1] == 0);
      return bufferSize-1;
    }
    // Add the opening (system or user) parenthesis
    numberOfChar += SerializationHelper::CodePoint(buffer+numberOfChar, bufferSize - numberOfChar, needsSystemParentheses ? UCodePointLeftSystemParenthesis : CodePoint('('));
    if (numberOfChar >= bufferSize-1) {
      assert(buffer[bufferSize - 1] == 0);
      return bufferSize - 1;
    }
  }

  int childrenCount = node->numberOfChildren();
  assert(prefix || childrenCount > 0);

  if (childrenCount > 0) {
    int lastIndex = lastChildIndex < 0 ? childrenCount - 1 : lastChildIndex;
    assert(firstChildIndex <= lastIndex);

    // Write the children, separated with commas or the operator
    for (int i = firstChildIndex; i <= lastIndex; i++) {
      if (i != firstChildIndex) {
        // Write the operator or the comma
        numberOfChar += prefix ?
          SerializationHelper::CodePoint(buffer+numberOfChar, bufferSize - numberOfChar, ',') :
          strlcpy(buffer+numberOfChar, operatorName, bufferSize-numberOfChar);
        if (numberOfChar >= bufferSize-1) {
          assert(buffer[bufferSize - 1] == 0);
          return bufferSize - 1;
        }
      }
      // Write the child, with or without parentheses if needed
      if (prefix) {
        if (needsSystemParentheses && (childrenCount > 1)) {
          numberOfChar += SerializationHelper::CodePoint(buffer+numberOfChar, bufferSize - numberOfChar, UCodePointLeftSystemParenthesis);
          if (numberOfChar >= bufferSize-1) {
            assert(buffer[bufferSize - 1] == 0);
            return bufferSize - 1;
          }
        }
        numberOfChar += node->childAtIndex(i)->serialize(buffer+numberOfChar, bufferSize-numberOfChar, floatDisplayMode, numberOfDigits);
        if (numberOfChar >= bufferSize-1) {
          assert(buffer[bufferSize - 1] == 0);
          return bufferSize - 1;
        }
        if (needsSystemParentheses && (childrenCount > 1)) {
          numberOfChar += SerializationHelper::CodePoint(buffer+numberOfChar, bufferSize - numberOfChar, UCodePointRightSystemParenthesis);
          if (numberOfChar >= bufferSize-1) {
            assert(buffer[bufferSize - 1] == 0);
            return bufferSize - 1;
          }
        }
      } else {
        numberOfChar += SerializationHelper::SerializeChild(node->childAtIndex(i), node, buffer + numberOfChar, bufferSize - numberOfChar, floatDisplayMode, numberOfDigits);
        if (numberOfChar >= bufferSize-1) {
          assert(buffer[bufferSize - 1] == 0);
          return bufferSize - 1;
        }
      }
    }
  }

  if (prefix) {
    // Add the closing system parenthesis
    numberOfChar += SerializationHelper::CodePoint(buffer+numberOfChar, bufferSize - numberOfChar, needsSystemParentheses ? UCodePointRightSystemParenthesis : CodePoint(')'));
    if (numberOfChar >= bufferSize-1) {
       assert(buffer[bufferSize - 1] == 0);
       return bufferSize - 1;
    }
  }

  assert(buffer[numberOfChar] == 0);
  return numberOfChar;
}

int SerializationHelper::Infix(
    const TreeNode * node,
    char * buffer,
    int bufferSize,
    Preferences::PrintFloatMode floatDisplayMode,
    int numberOfDigits,
    const char * operatorName,
    int firstChildIndex,
    int lastChildIndex)
{
  return InfixPrefix(false, node, buffer, bufferSize, floatDisplayMode, numberOfDigits, operatorName, false, firstChildIndex, lastChildIndex);
}

int SerializationHelper::Prefix(
    const TreeNode * node,
    char * buffer,
    int bufferSize,
    Preferences::PrintFloatMode floatDisplayMode,
    int numberOfDigits,
    const char * operatorName,
    bool needsSystemParentheses,
    int lastChildIndex)
{
  return InfixPrefix(true, node, buffer, bufferSize, floatDisplayMode, numberOfDigits, operatorName, needsSystemParentheses, 0, lastChildIndex);
}

int SerializationHelper::CodePoint(char * buffer, int bufferSize, class CodePoint c) {
  {
    int result = 0;
    if (checkBufferSize(buffer, bufferSize, &result)) {
      return result;
    }
  }
  int size = UTF8Decoder::CodePointToChars(c, buffer, bufferSize);
  if (size <= bufferSize - 1) {
    buffer[size] = 0;
  } else {
    assert(size - 1 == bufferSize - 1);
    buffer[--size] = 0;
  }
  return size;
}

}

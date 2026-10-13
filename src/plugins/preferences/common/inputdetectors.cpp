#include "inputdetectors.h"

namespace preferences {

bool WhitespaceDetector::detect(const QString &input)
{
    return input.trimmed() != input;
}

bool EmptyDetector::detect(const QString &input)
{
    return input.length() == 0;
}

} // namespace preferences

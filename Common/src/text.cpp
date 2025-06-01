#include "QtNoidCommon/text.h"
#include <QCoreApplication>

namespace QtNoid {
namespace Common {

bool Text::isUrl(const QString &text)
{
    if(text.startsWith("http"))
        return true;
    if(text.startsWith("www"))
        return true;
    return false;
}

} // namespace Common
} // namespace QtNoid

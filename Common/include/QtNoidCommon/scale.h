#ifndef QTNOID_SCALE_H
#define QTNOID_SCALE_H

#include "global.h"
#include <QObject>
#include <QString>

namespace QtNoid {
namespace Common {

class QTNOIDCOMMON_EXPORT Scale
{
public:
    static QStringList prefixList(){return {"k","M","G","T","P","E"};} // -> we need 128 bit for this << "Z" << "Y";
    static qint64 multiplier(const QString &prefix);
    static QString multiplierPrefix(qint64 value);
    static QString autoUpToExaByte(qint64 value, const QString& unitOfMeasure = "Byte");
    static QString autoUpToExa(qint64 value, const QString& unitOfMeasure);
    static QString nanoSecsUpToDays(quint64 ns);
    static QString nanoSecsUpToDays(qint64 ns);

private:
    static QString removeBracketsAndSpacesFromUnitOfMeasure(const QString& unitOfMeasure);
};

} // namespace QtCommon
} // namespace QtNoid

#endif // QTNOID_SCALE_H

#include "QtNoidCommon/scale.h"
#include <QCoreApplication>
#include <QTime>
#include <QUrl>

namespace QtNoid {
namespace Common {

qint64 Scale::multiplier(const QString &prefix)
{
    QString filteredPrefix = prefix.trimmed();
    filteredPrefix.truncate(1);
    auto index = prefixList().indexOf(filteredPrefix);
    if(index < 0)
        return 0;
    return static_cast<qint64>(qPow(10.0, 3.0*(static_cast<qreal>(index+1))));
}

/**
 * @brief Scale::multiplierPrefix return scale prefix given the input value
 * @param value: it could be any value not only a multiple of 1000
 * @return
 */
QString Scale::multiplierPrefix(qint64 value)
{
    if(value<=0)
        return QString();

    double magnitude = floor(log10(value)/3);
    int index = static_cast<int>(magnitude) - 1;
    if(index<0){
        return QString();
    }
    return prefixList().at(index);
}

QString Scale::autoUpToExaByte(qint64 value, const QString &unitOfMeasure)
{
    // log2(1024) = 10
    qint64 absValue = abs(value);
    double scaledVal = 0.0;
    double scale = 0.0;
    if(absValue != 0) {
        double magnitude = round(log2(absValue)/10.0);
        scale = pow(2, magnitude*10);
        scaledVal = static_cast<double>(value)/scale;
    }
    return QString("%1 [%2%3]").arg(scaledVal, 0, 'f', 2)
        .arg(multiplierPrefix(scale), removeBracketsAndSpacesFromUnitOfMeasure(unitOfMeasure));

}

QString Scale::autoUpToExa(qint64 value, const QString &unitOfMeasure)
{
    // log10(1000) = 3
    qint64 absValue = abs(value);
    double scaledVal = 0.0;
    double scale = 0.0;
    if(absValue != 0) {
        double magnitude = round(log10(absValue)/3.0);
        scale = pow(10, magnitude*3);
        scaledVal = static_cast<double>(value)/scale;
    }
    return QString("%1 [%2%3]").arg(scaledVal, 0, 'f', 2)
        .arg(multiplierPrefix(scale), removeBracketsAndSpacesFromUnitOfMeasure(unitOfMeasure));
}

QString Scale::nanoSecsUpToDays(quint64 ns)
{
    if(ns < 1000) {
        return QString("%1 [ns]").arg(ns);
    }
    if(ns < 100*1000) {
        return QString("%1 [µs]").arg(ns/1000.0, 0, 'f', 2);
    }
    if(ns < 1000*1000) {
        return QString("%1 [µs]").arg(round(ns/1000.0));
    }
    if(ns < 100*1000*1000) {
        return QString("%1 [ms]").arg(ns/1000000.0, 0, 'f', 2);
    }

    quint64 ms = ns/1000000;
    if(ns < 1000*1000*1000) {
        return QString("%1 [ms]").arg(ms);
    }

    // More than 1 seconds
    if (ns < 60LL*1000*1000*1000){
        return QString("%1 [s]").arg(ms/1000.0, 0, 'f', 2);
    }
    quint64 nsPerDay = 24LL*3600*1000*1000*1000;
    if (ns < nsPerDay) {
        QTime  elapsedTime = QTime::fromMSecsSinceStartOfDay(static_cast<int>(ms));
        return elapsedTime.toString("HH:mm:ss");
    }

    // More than 1 day
    quint64 days = ns / nsPerDay;
    QTime  remainderHrs = QTime::fromMSecsSinceStartOfDay(static_cast<int>((ms)%(24LL*3600*1000)));
    return QString("%1 %2")
        .arg(days, 3, 10, QChar('0'))
        .arg(remainderHrs.toString("HH:mm:ss"));
}

QString Scale::nanoSecsUpToDays(qint64 ns)
{
    return nanoSecsUpToDays(static_cast<quint64>(ns));
}

QString Scale::removeBracketsAndSpacesFromUnitOfMeasure(const QString &unitOfMeasure)
{
    // Check and clean-up measuring unit
    QString bareUnitOfMeasure=unitOfMeasure;
    bareUnitOfMeasure.replace("[", "");
    bareUnitOfMeasure.replace("]", "");
    bareUnitOfMeasure = bareUnitOfMeasure.trimmed();
    return bareUnitOfMeasure;
}



} // namespace Common
} // namespace QtNoid

#ifndef QTNOID_TXT2JSON_H
#define QTNOID_TXT2JSON_H

#include "global.h"
#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class TestQtNoidJsonTxt2Json;

namespace QtNoid {
namespace Json {


class QTNOIDJSON_EXPORT Txt2Json
{
    friend class ::TestQtNoidJsonTxt2Json;

public:
    Txt2Json() = default;
    static QJsonObject plainTextToJson(const QStringList &plainText);
    static QStringList plainTextFromJson(const QJsonObject &json);

private:
    static bool plainTextIsString(const QString &val);
    static bool plainTextIsNumber(const QString &val);
    static bool plainTextIsArray(const QString &val);
    static bool plainTextIsKeyValuePair(const QString key, const QString &val);

    static QStringList textArrayToJson(const QString &val);
    static QString textNumberToJson(const QString &val);
    static QString textStringToJson(const QString &val);

    static QString plainTextJsonValToString(const QJsonValue &jVal);

};

} // namespace Json
} // namespace QtNoid

#endif // QTNOID_TXT2JSON_H

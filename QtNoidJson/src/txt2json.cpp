#include "QtNoidJson/txt2json.h"
#include <QCoreApplication>
#include <QDebug>

namespace QtNoid {
namespace Json {

QJsonObject Txt2Json::plainTextToJson(const QStringList &plainText)
{
    QStringList jStrList;
    for(const QString &line : plainText){
        // Dismantle
        auto key = line.section(':',0,0).trimmed();
        QString val = line.section(':', 1).trimmed();
        if (plainTextIsString(val)) {
            val = QString("\"%1\"").arg(val);
        } else if (plainTextIsNumber(val)) {
            if (val.startsWith("+")) {
                val = val.remove(0, 1);
            }
        } else if (auto list = plainTextIsArray(val); list.size() > 0) {
            QStringList newArrayList;
            for (const auto &e : list) {
                QString element = e.trimmed();
                if (plainTextIsString(element)) {
                    element = QString("\"%1\"").arg(element);
                } else if (plainTextIsNumber(element)) {
                    if (element.startsWith("+")) {
                        element = element.remove(0, 1);
                    }
                }
                newArrayList << element;
            }
            val = QString("[%1]").arg(newArrayList.join(","));
        }
        // Rebuild
        QString newLine = QString("\"%1\": %2").arg(key, val);
        jStrList.append(newLine);
    }

    QString jsonDef = "{" + jStrList.join(",") + "}";
    QJsonParseError errors;
    QJsonDocument doc = QJsonDocument::fromJson(jsonDef.toUtf8(), &errors);

    return doc.object();
}

QStringList Txt2Json::plainTextFromJson(const QJsonObject &json)
{
    QStringList res;
    for(const QString &key : json.keys()){
        const QJsonValue val = json[key];
        res << QString("%1: %2").arg(key, plainTextJsonValToString(val));
    }
    return res;
}

bool Txt2Json::plainTextIsString(const QString &val)
{
    auto txt = val.trimmed();
    if (txt == "true") {
        return false;
    }
    if (txt == "false") {
        return false;
    }
    if (txt == "null") {
        return false;
    }
    if (txt.startsWith("["))
        return false;

    if(plainTextIsNumber(txt))
        return false;

    return true;
}

bool Txt2Json::plainTextIsNumber(const QString &val)
{
    auto txt = val.trimmed();

    bool isIntNumber = false;
    txt.toInt(&isIntNumber, 10);
    if(isIntNumber)
        return true;

    bool isRealNumber = false;
    txt.toDouble(&isRealNumber);
    if(isRealNumber)
        return true;

    return false;
}

QStringList Txt2Json::plainTextIsArray(const QString &val)
{
    auto txt = val.trimmed();
    if (!txt.startsWith("["))
        return QStringList();
    if(!txt.endsWith("]"))
        return QStringList();
    txt = txt.remove(0,1);
    txt.chop(1);
    QStringList res;
    for(QString &line : txt.split(",")) {
        line = line.replace("\"", "");
        res.append(line.trimmed());
    }

    return res;
}

QString Txt2Json::plainTextJsonValToString(const QJsonValue &jVal)
{
    if(jVal.isString()){
        return jVal.toString();
    }
    else if(jVal.isBool()) {
        return jVal.toBool()? "true" : "false";
    }
    else if(jVal.isDouble()) {
        // QString("%1: %2").arg(val.toDouble(), 0, 'g');
        return QString::number(jVal.toDouble(), 'g', -1);
    }
    else if(jVal.isArray()){
        QStringList aStrList;
        const QJsonArray jValArray = jVal.toArray();
        for(const QJsonValue &aVal : jValArray) {
            aStrList << plainTextJsonValToString(aVal);
        }
        return QString("[%1]").arg(aStrList.join(", "));
    }

    return QString();
}


} // namespace Json
} // namespace QtNoid

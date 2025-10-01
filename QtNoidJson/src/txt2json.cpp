#include "QtNoidJson/txt2json.h"
#include "QtNoidCommon/QtNoidCommon"
#include <QCoreApplication>
#include <QDebug>

namespace QtNoid {
namespace Json {

QJsonObject Txt2Json::plainTextToJson(const QStringList &plainText)
{
    // Translate from text to JSON syntax
    QStringList jStrList;
    for(const QString &line : plainText){
        // Dismantle
        QString key = line.section(':',0,0).trimmed();
        QString val = line.section(':', 1).trimmed();
        if (plainTextIsString(val)) {
            val = QString("\"%1\"").arg(val);
        } else if (plainTextIsNumber(val)) {
            if (val.startsWith('+')) {
                val.remove('+');
            }
        } else if (plainTextIsArray(val)) {
            if(val.contains("}")) {
                val = QString("[%1] }").arg(textArrayToJson(val).join(","));
            }
            else {
                val = QString("[%1]").arg(textArrayToJson(val).join(","));
            }
        }
        // Rebuild
        if(!key.isEmpty()) {
            QString newLine;
            if(key.startsWith("{")) {
                key.remove(0,1);
                newLine = "{ ";
            }
            newLine += QString("\"%1\":").arg(key);
            if(!val.isEmpty()) {
                newLine += QString("%1,").arg(val);
            }
            jStrList.append(newLine);
        }
    }

    // Remove the excessive , at the end of the last item
    if(!jStrList.isEmpty() && jStrList.last().endsWith(',')) {
        jStrList.last().chop(1);
    }
    QString jsonDef = "{" + jStrList.join("\n") + "}";
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
    if(txt.isEmpty()){
        return false;
    }
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

bool Txt2Json::plainTextIsArray(const QString &val)
{
    auto txt = val.trimmed();
    if (!txt.startsWith("["))
        return false;

    if (!txt.contains("]"))
        return false;

    return true;
}

QStringList Txt2Json::textArrayToJson(const QString &val)
{
    QString txt = val.section('[', 1).section(']', 0, 0);

    QStringList res;
    QStringList elements = QtNoid::Common::Text::tokenize(txt, ", ", false, 0);
    for(QString &element : elements){
        if (plainTextIsNumber(element) && element.startsWith('+')) {
            element.remove('+');
        }
        res.append(element.trimmed());
    }

    return res;

    // txt = txt.remove(0,1);
    // txt.chop(1);
    // auto list = texArrayToJson(val);
    // // list.size() > 0
    // QStringList jArrayList;
    // for (QString &element : list) {
    //     if (plainTextIsNumber(element) && element.startsWith("+")) {
    //         element.remove("+");
    //     }
    //     jArrayList << element;
    // }
    // val = QString("[%1]").arg(jArrayList.join(","));
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

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
    int openBrace = 0;
    /* A new line after an objec name can mark a child object start
    so another brace need to be cancelled since we have a new object name */
    bool braceStartMarker = false;
    bool arrayStartMarker = false;
    for(const QString &line : plainText){
        // Dismantle
        QString key = line.section(':',0,0).trimmed();
        QString val = line.section(':', 1).trimmed();
        if (plainTextIsString(val)) {
            if(val.endsWith("]")) {
                val.chop(1);
                val = textStringToJson(val);
                val.append(']');
            }
            else {
                val = textStringToJson(val);
            }
        } else if (plainTextIsNumber(val)) {
            if(val.endsWith("}")) {
                val = textNumberToJson(val) + "}";
                --openBrace;
            }
            else {
                val = textNumberToJson(val);
            }
        } else if (plainTextIsArray(val)) {
            if(val.contains("}")) {
                val = QString("[%1] }").arg(textArrayToJson(val).join(","));
                --openBrace;
            }
            else {
                val = QString("[%1]").arg(textArrayToJson(val).join(","));
            }
        }
        // Rebuild
        if(!key.isEmpty()) {
            QString newLine;
            if(plainTextIsKeyValuePair(key, val) && arrayStartMarker ){
                newLine.append('{');
                ++openBrace;
                braceStartMarker = true;
            }
            if(key.startsWith("{")) {
                key.remove(0,1);
                key = key.trimmed();
                if(!braceStartMarker) {
                    newLine = "{ ";
                    ++openBrace;
                    braceStartMarker = true;
                }
            }

            // Filter double ""
            newLine += textStringToJson(key).append(':');

            if(val.isEmpty()) {
                // val empty Mark the start of a child object
                if(!braceStartMarker) {
                    newLine += "{";
                    ++openBrace;
                    braceStartMarker = true;
                }
            }
            else if(val.startsWith('{')) {
                newLine += QString("%1").arg(val);
                ++openBrace;
                braceStartMarker = true;
            }
            else if(val.startsWith('[') && !val.contains(']')) {
                newLine += QString("%1").arg(val);
                arrayStartMarker = true;
            }
            else if(val.endsWith(']') && !val.contains('[')) {
                val.chop(1);
                newLine += QString("%1").arg(val);
                newLine.append('}');
                newLine.append(']');
                arrayStartMarker = false;
            }
            else {
                if(openBrace > 0 && arrayStartMarker) {
                    newLine += QString("%1},").arg(val);
                    openBrace--;
                }
                else{
                    newLine += QString("%1,").arg(val);
                }
            }
            jStrList.append(newLine);
        }
        else if(openBrace > 0){
            if(!jStrList.isEmpty() && jStrList.last().endsWith(',')) {
                jStrList.last().chop(1);
            }
            jStrList.append("}");
            --openBrace;
        }
    }

    // Remove the excessive , at the end of the last item
    if(!jStrList.isEmpty() && jStrList.last().endsWith(',')) {
        jStrList.last().chop(1);
    }
    QString jsonDef = "{" + jStrList.join("") + "}";
    // Clean double {{ and }}
    // jsonDef.replace("{{", "{").replace("}}", "}");
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
    if (txt.startsWith("{"))
        return false;

    if(plainTextIsNumber(txt))
        return false;

    return true;
}

bool Txt2Json::plainTextIsNumber(const QString &val)
{
    auto txt = val.trimmed();
    if(txt.endsWith('}')) {
        txt.chop(1);
    }

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

bool Txt2Json::plainTextIsKeyValuePair(const QString key, const QString &val)
{
    if(!plainTextIsString(val)) {
        return false;
    }
    return true;
}

QStringList Txt2Json::textArrayToJson(const QString &val)
{
    QStringList res;
    QStringList elements = QtNoid::Common::Text::tokenize(val, "[], }", false, 0);
    for(QString &element : elements){
        if (plainTextIsNumber(element)) {
            element.remove('+');
        }
        else if(plainTextIsString(element)) {
            if(!element.startsWith('"')) {
                element = R"(")" + element;
            }
            if(!element.endsWith('"')) {
                element += R"(")";
            }
        }
        res.append(element);
    }

    return res;
}

QString Txt2Json::textNumberToJson(const QString &val)
{
    QString res = val;
    bool isNegative = false;

    // Remove + / -
    if (res.startsWith('+')) {
        res.remove('+');
    }

    if (res.startsWith('-')) {
        res.remove('-');
        isNegative = true;
    }

    if (res.startsWith('0') && !res.startsWith("0.") && res != "0") {
        int ii = 0;
        for (const QChar& c : std::as_const(res)) {
            if (c != '0') break;
            ii++;
        }
        res = res.mid(ii);
        if(res.startsWith('.')) {
            res = "0"+res;
        }
    }

    if(isNegative) {
        res = "-" + res;
    }

    if(res.endsWith('}')) {
        res.chop(1);
        res = res.trimmed();
    }

    return res;
}

QString Txt2Json::textStringToJson(const QString &val)
{
    QString res;
    if(!val.startsWith('"')) {
        res += QString('"');
    }
    res += val;

    if(!val.endsWith('"')) {
        res += QString('"');
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

#include "QtNoidJson/yaml2json.h"
#include "QtNoidCommon/QtNoidCommon"
#include <QCoreApplication>
#include <QDebug>

namespace QtNoid {
namespace Json {

Yaml2Json::Yaml2Json(const QString yaml, QObject *parent)
    : QObject(parent), m_yaml(yaml)
{
}

QString Yaml2Json::yaml() const
{
    return m_yaml;
}

void Yaml2Json::setYaml(const QString &newYaml)
{
    m_yaml = newYaml;
}

QJsonObject Yaml2Json::json() const
{
    return m_json;
}

bool Yaml2Json::isValid() const
{
    return m_isValid;
}




} // namespace Json
} // namespace QtNoid

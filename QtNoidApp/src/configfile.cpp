//=============================================================================
// configfile.cpp
//=============================================================================
#include "QtNoidApp/configfile.h"
#include "QtNoidApp/settings.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

namespace QtNoid {
namespace App {

ConfigFile::ConfigFile(QObject *parent)
    : Config(parent)
{
    qDebug() << __func__ ;
}


ConfigFile::ConfigFile(const QString &fileName, QObject *parent)
    : Config(parent)
{
    qDebug() << __func__ << this;
    initFileName(fileName);
    load();
}


ConfigFile::~ConfigFile()
{
    qDebug() << __func__;
    save();
}


QString ConfigFile::fileName() const
{
    return m_fileName.value();
}


void ConfigFile::setFileName(const QString &value)
{
    m_fileName = value;
}


QBindable<QString> ConfigFile::bindableFileName()
{
    return QBindable<QString>(&m_fileName);
}


bool ConfigFile::isValid()
{
    QString name = m_fileName;
    if(name.isEmpty()) return false;

    return true;
}


bool ConfigFile::load()
{
    // qDebug() << __func__ ;
    if(!isValid())
        return false;

    QFile file(m_fileName);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }

    QJsonParseError m_jsonError;
    auto doc = QJsonDocument::fromJson(file.readAll(), &m_jsonError) ;
    if(!doc.isObject())
        return false;

    auto res = valuesFromJson(doc.object());
    if(!res) {
        return false;
    }

    emit fileLoaded(m_fileName);
    return true;
}


bool ConfigFile::save()
{
    if(!isValid())
        return false;
    QFile file(m_fileName);
    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }

    if(isEmpty())
        return false;

    QJsonDocument doc = QJsonDocument(toJsonValues());
    QTextStream text(&file);
    text << doc.toJson();

    // qDebug() << __func__;
    return true;
}


void ConfigFile::initFileName(const QString &fileName)
{
    if(fileName.isEmpty()) {
        m_fileName = Settings::filePathAsAppSibling();
    }
    else {
        m_fileName = fileName;
    }
}



} // namespace App
} // namespace QtNoid

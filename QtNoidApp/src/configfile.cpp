//=============================================================================
// configfile.cpp
//=============================================================================
#include "QtNoidApp/configfile.h"
#include "QtNoidApp/core.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

namespace QtNoid {
namespace App {

ConfigFile::ConfigFile(QObject* parent)
    : Config(parent)
{
    // qDebug() << Q_FUNC_INFO;
}


ConfigFile::ConfigFile(const QString& fileName, QObject* parent)
    : Config(parent)
{
    // qDebug() << Q_FUNC_INFO << "|" << this;
    initFileName(fileName);
    load();
}


ConfigFile::~ConfigFile()
{
    // qDebug() << Q_FUNC_INFO;
    save();
}


QString ConfigFile::fileName() const
{
    return m_fileName.value();
}


void ConfigFile::setFileName(const QString& value)
{
    m_fileName = value;
}


QBindable<QString> ConfigFile::bindableFileName()
{
    return QBindable<QString>(&m_fileName);
}


bool ConfigFile::isValid() const
{
    QString name = m_fileName;
    if(name.isEmpty()) return false;

    return true;
}


bool ConfigFile::load()
{
    QJsonParseError jsonError;

    // qDebug() << Q_FUNC_INFO;
    if(!isValid()) {
        emit fileLoadError(m_fileName, "Invalid fileName");
        return false;
    }

    QFile file(m_fileName);
    if(!file.open(QIODevice::ReadOnly)){
        emit fileLoadError(m_fileName, "Failed to open the file");
        return false;
    }

    auto doc = QJsonDocument::fromJson(file.readAll(), &jsonError);
    if(jsonError.error != QJsonParseError::NoError) {
        emit fileLoadError(m_fileName, jsonError.errorString());
        return false;
    }

    if(!doc.isObject()) {
        emit fileLoadError(m_fileName, "File content is not a JSON object");
        return false;
    }

    auto json = doc.object();
    const bool nameWillBeAdoptedFromJson = name().isEmpty() && (json.count() == 1);
    if(!nameWillBeAdoptedFromJson && !json.contains(name())) {
        // This is not the correct JSON model
        emit fileLoadError(m_fileName, "JSON content doesn't match the expected Config structure");
        return false;
    }

    clear(); // Remove previous content
    valuesFromJson(json);
    emit fileLoaded(m_fileName);
    return true;
}


bool ConfigFile::save()
{
    if(!isValid()) {
        emit fileSaveError(m_fileName, "Invalid fileName");
        return false;
    }

    QFile file(m_fileName);
    if(!file.open(QIODevice::WriteOnly)){
        emit fileSaveError(m_fileName, file.errorString());
        return false;
    }

    QJsonDocument doc = QJsonDocument(toJsonValues());
    QTextStream text(&file);
    text << doc.toJson();

    // qDebug() << Q_FUNC_INFO;
    return true;
}


void ConfigFile::initFileName(const QString& fileName)
{
    if(fileName.isEmpty()) {
        m_fileName = Core::filePathAsAppSibling();
    }
    else {
        m_fileName = fileName;
    }
}



} // namespace App
} // namespace QtNoid

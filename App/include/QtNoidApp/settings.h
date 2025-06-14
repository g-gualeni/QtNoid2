
#ifndef QTNOID_SETTINGS_H
#define QTNOID_SETTINGS_H

#include "global.h"
#include <QObject>
#include <QString>


namespace QtNoid {
namespace App {


class QTNOIDAPP_EXPORT Settings
{
public:
    static QString fileAsAppSibling();
    static QString appExeOrAppBundlePath();
private:
};



/**
 *

    static QWidget *mainWindowsFromWidget(QWidget *ref);

    static QString iniGroupNameFromClass(const QObject* ref);
    static QString iniGroupNameFromObjectName(const QObject* ref);

 *
*/




/**

    bool save();
    bool load();
    static bool isValid(const QJsonDocument& jsonDoc);

    bool isEmpty();
    void setValue(const QString& path, const QVariant& value);
    void setValue(const QObject *sender, const QString& path, const QVariant& value);

    QString value(const QObject *sender, const QString &path, const QString &defaultValue) const;
    QString value(const QString &path, const QString &defaultValue) const;
    QVariant valueAsVariant(const QString& path, const QVariant& defaultValue) const;
    QVariant valueAsVariant(const QObject* sender, const QString& path, const QVariant& defaultValue) const;
    QByteArray valueAsByteArray(const QString& path, const QByteArray& defaultValue) const;
    QByteArray valueAsByteArray(const QObject* sender, const QString& path, const QByteArray& defaultValue) const;

    std::shared_ptr<PreferencesModel> preferencesModel() const;

protected:
    QString senderName(const QObject *sender) const;
    QJsonValue findValue(const QObject* sender, const QString& path) const;
    QJsonObject findGroup(const QObject* sender) const;

private:
    QJsonObject m_configurations;
    QJsonParseError m_jsonError;
    QString m_appSettingsFilePath;
    std::shared_ptr<PreferencesModel> m_preferencesModel;



 */


} // namespace App
} // namespace QtNoid

#endif // QTNOID_SETTINGS_H

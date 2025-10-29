#ifndef QTNOID_APP_CONFIGFILE_H
#define QTNOID_APP_CONFIGFILE_H

#include "config.h"

namespace QtNoid {
namespace App {

class QTNOIDAPP_EXPORT ConfigFile : public Config
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName BINDABLE bindableFileName NOTIFY fileNameChanged FINAL)

public:
    explicit ConfigFile(QObject *parent = nullptr);
    explicit ConfigFile(const QString &fileName, QObject *parent = nullptr);
    ~ConfigFile();

    // FileName
    QString fileName() const;
    void setFileName(const QString& value);
    QBindable<QString> bindableFileName();
    bool isValid();
    bool load();
    bool save();

signals:
    void fileNameChanged(const QString& value);

private:
    void initFileName(const QString& fileName);

private:
    Q_OBJECT_BINDABLE_PROPERTY(ConfigFile, QString, m_fileName, &ConfigFile::fileNameChanged)
};

} // namespace App
} // namespace QtNoid


inline QDebug operator<<(QDebug debug, const QtNoid::App::ConfigFile &configFile)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "ConfigFile(" << configFile.fileName() << "->";

    auto config = qobject_cast<const QtNoid::App::Config*>(&configFile);
    debug.nospace() << QDebug::toString(config);
    return debug;
}

inline QDebug operator<<(QDebug debug, const QtNoid::App::ConfigFile *configFile)
{
    if (configFile) {
        return ::operator<<(debug, *configFile);
    } else {
        return debug << "ConfigFile(nullptr)";
    }
}

#endif // QTNOID_APP_CONFIGFILE_H

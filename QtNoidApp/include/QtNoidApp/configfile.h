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

#endif // QTNOID_APP_CONFIGFILE_H

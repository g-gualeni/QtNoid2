#ifndef QTNOID_UICOMMON_CORE_H
#define QTNOID_UICOMMON_CORE_H

#include "global.h"
#include <QJsonObject>
#include <QString>
#include <QStringList>

class QComboBox;

namespace QtNoid {
namespace UiCommon {


class QTNOID_UICOMMON_EXPORT Core
{
public:
    static bool restoreComboBoxTextItems(QComboBox *cbo, const QJsonObject &cboConfig);
    static QJsonObject saveComboBoxTextItems(QComboBox *cbo);
};




} // namespace App
} // namespace UiCommon

#endif // QTNOID_UICOMMON_CORE_H

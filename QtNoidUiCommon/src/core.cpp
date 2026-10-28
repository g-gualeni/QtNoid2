#include "QtNoidUiCommon/core.h"
#include "QtNoidApp/QtNoidApp"

#include <QComboBox>
#include <QJsonArray>

namespace QtNoid {
namespace UiCommon {



bool Core::restoreComboBoxTextItems(QComboBox *cbo, const QJsonObject &cboConfig)
{
    bool finalResult = false;
    do {
        if(cbo == nullptr) {
            break;
        }

        if(!cboConfig.contains("items")) {
            break;
        }

        if(!cboConfig.value("items").isArray()) {
            break;
        }

        if(!cboConfig.contains("currentText")) {
            break;
        }

        QStringList items;
        const QJsonArray itemsArray = cboConfig.value("items").toArray();
        for (const QJsonValue &val : itemsArray) {
            items << val.toString();
        }
        items.removeDuplicates();

        QString currentItem;
        currentItem = cboConfig.value("currentText").toString();

        // All test ok we can touch cbo
        cbo->clear();
        cbo->addItems(items);
        cbo->setCurrentText(currentItem);

        // If a current item not in the list return false
        if(!items.contains(currentItem)) {
            break;
        }
        finalResult = true;

    } while(0);

    return finalResult;
}

QJsonObject Core::saveComboBoxTextItems(QComboBox *cbo)
{
    if(cbo == nullptr)
        return {};

    QJsonObject cboJson;

    QStringList items;
    for(int ii=0; ii<cbo->count(); ii++) {
        items << cbo->itemText(ii);
    }
    cboJson["items"] = QJsonArray::fromStringList(items);
    cboJson["currentText"] = cbo->currentText();

    return cboJson;
}


}   // namespace UiCommon
}   // namespace QtNoid

# Class: Core
This class contains a set of helper static methods to save and restore the content of UI widgets to and from a `QJsonObject`, so that a widget's state (for example a combo box's items and current selection) can be persisted as part of an application's configuration, for example through [QtNoid::App::Config](../../QtNoidApp/doc/QtNoidAppConfig.md) or the global object `appConfig`.

## Static Methods

- `bool restoreComboBoxTextItems(QComboBox *cbo, const QJsonObject &cboConfig)`: restores the text items and the current selection of a combo box from a JSON object with the same shape produced by `saveComboBoxTextItems()`, using the keys `"items"` (a JSON array of strings) and `"currentText"` (a string). Duplicate entries in `"items"` are removed, keeping the first occurrence of each and preserving the original relative order. Returns `false`, leaving `cbo` untouched, if `cbo` is `nullptr`, or if `cboConfig` does not contain `"items"` as a JSON array, or does not contain `"currentText"`. If `cboConfig` has that shape, `cbo` is cleared and repopulated with the (deduplicated) items and `setCurrentText()` is called with the value of `"currentText"`; this happens even if `"currentText"` does not match any of the restored items, in which case the method still returns `false`, even though the items were applied to `cbo` (the combo box ends up with whatever item Qt selects by default, typically the first one).

- `QJsonObject saveComboBoxTextItems(QComboBox *cbo)`: saves a combo box's text items and current selection into a JSON object, under the keys `"items"` (a JSON array built from `QComboBox::itemText()` for every item, in order) and `"currentText"` (the combo box's `QComboBox::currentText()`). Returns an empty `QJsonObject` if `cbo` is `nullptr`. The result can be stored and later passed to `restoreComboBoxTextItems()`.

## Non Static Methods
* None

[⬆ Back to QtNoidUiCommon](QtNoidUiCommon.md)
[← Back to README](../../README.md)

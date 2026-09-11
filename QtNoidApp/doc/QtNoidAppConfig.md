# Class: Config

    Q_PROPERTY(QString name READ name WRITE setName BINDABLE bindableName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription BINDABLE bindableDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString tooltip READ tooltip WRITE setTooltip BINDABLE bindableTooltip NOTIFY tooltipChanged FINAL)
    Q_PROPERTY(int count READ count BINDABLE bindableCount NOTIFY countChanged FINAL)


signals:
    void nameChanged(const QString& value);
    void descriptionChanged(const QString& value);
    void tooltipChanged(const QString& value);
    void countChanged(int count);
    void pageAdded(const QtNoid::App::ParameterList* parameterList);
    void pageRemoved(QtNoid::App::ParameterList* parameterList);
    void pageRenameError(const QString& oldName, const QString& newName);

[⬆ Back to QtNoidApp](QtNoidApp.md)

&nbsp;

[← Back to README](../../README.md)

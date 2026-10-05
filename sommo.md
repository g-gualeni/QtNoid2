Ecco la lista completa dei metodi toccati nei tre file di test, divisi per file e per tipo di modifica.

**test_app_parameter.cpp**

Rimossi (dichiarazione + corpo, legati al vecchio `changed()`):

- [x] testParameterChangedSignalEmittedOnValueChange()
- [x] testParameterChangedSignalNotEmittedWhenValueUnchanged()
- [x] testParameterChangedSignalEmittedForEachPropertyType()
- [x] testParameterChangedSignalCoalescesMultiplePropertyChanges()
- [x] testParameterChangedSignalNotEmittedWhenSignalsBlocked()

Modificati (stessa firma, corpo riscritto per il nuovo `aboutToBeDestroyed(Parameter*)`):

- [x] testAboutToBeDestroyedSignalWhenValueUnchanged()
- [x] testAboutToBeDestroyedSignalWhenValueChanged()

Nuovi:

- [x] testAboutToBeDestroyedSignalWhenSchemaChanged()
- [ ] testBindableIsSchemaChanged()
- [ ] testParameterIsSchemaChangedShouldBeFalseAfterConstructor()
- [ ] testParameterIsSchemaChangedOnlyAfterSchemaPropertyChanged()
- [ ] testParameterIsSchemaChangedNotAffectedByValueChange()
- [ ] testParameterIsSchemaChangedShouldBeFalseAfterLoadingFromJson()
- [ ] testParameterResetValueChange()
- [ ] testParameterResetValueChangeDoesNotAffectSchema()
- [ ] testParameterResetSchemaChange()
- [ ] testParameterResetSchemaChangeDoesNotAffectValue()
- [ ] testParameterResetSchemaChangeMinMaxCanClampCurrentValueAsSideEffect()

**test_app_parameterspage.cpp**

Rimossi:

- [ ] testParametersPageChangedSignalEmittedForEachOwnPropertyType()
- [ ] testParametersPageChangedSignalCoalescesMultiplePropertyChanges()
- [ ] testParametersPageChangedSignalNotEmittedWhenSignalsBlocked()
- [ ] testParametersPageChangedSignalEmittedOnceWhenContainedParametersChanges()
- [ ] testParametersPageChangedSignalNotEmittedWhenContainedParameterValueUnchanged
- [ ] testParametersPageChangedSignalNotEmittedAfterParameterRemoved()

Modificati:

- [x] testAboutToBeDestroyedSignalWhenValueUnchanged()
- [x] testAboutToBeDestroyedSignalWhenValueChanged()

Nuovi:

- [x] testAboutToBeDestroyedSignalWhenSchemaChanged()
- [ ] testParametersPageResetAllValues()
- [ ] testParametersPageResetAllValuesDoesNotAffectSchema()
- [ ] testParametersPageIsSchemaChanged()
- [ ] testBindableIsSchemaChangedProperty()
- [ ] testParametersPageResetSchemaChange()
- [ ] testParametersPageResetSchemaChangeDoesNotAffectValues()

**test_app_config.cpp**

Rimossi:

- [ ] testConfigChangedSignalEmittedForEachOwnPropertyType()
- [ ] testConfigChangedSignalCoalescesMultiplePropertyChanges()
- [ ] testConfigChangedSignalNotEmittedWhenSignalsBlocked()
- [ ] testConfigChangedSignalEmittedOnceWhenContainedPagesChanges()
- [ ] testConfigChangedSignalNotEmittedWhenContainedPagePropertyUnchanged() (conteneva anche un `QVERIFY(false);` residuo a fine corpo, quindi sarebbe comunque fallito)
- [ ] testConfigChangedSignalNotEmittedAfterPageRemoved()
- [ ] testConfigChangedSignalEmittedForDeeplyNestedParameterChange()

Modificati:

- [x] testAboutToBeDestroyedSignalWhenValueUnchanged()
- [x] testAboutToBeDestroyedSignalWhenValueChanged()

Nuovi:

- [x] testAboutToBeDestroyedSignalWhenSchemaChanged()
- [ ] testConfigResetAllValues()
- [ ] testConfigResetAllValuesDoesNotAffectSchema()
- [ ] testConfigIsSchemaChanged()
- [ ] testConfigBindableIsSchemaChangedProperty()
- [ ] testConfigResetSchemaChange()
- [ ] testConfigResetSchemaChangeDoesNotAffectValues()

In tutti e tre i file le dichiarazioni nella sezione `private slots:` sono state aggiornate di conseguenza (rimosse quelle dei test cancellati, aggiunte quelle dei nuovi).
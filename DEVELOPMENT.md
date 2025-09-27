# QtNoid Development Status

## Current Sprint (Sprint-003)

### Sprint-003 Goals
- Implementation of the QtNoid::App library and release of QtNoid2 version 2.1.0



### Completed
- [x] QtNoid:::App::Settings::fullDialogGrab() ✅ (commit: 9adac1b, 2025-08-15)
- [x] Parameter class foundation ✅ (commit: 7ebdc55, 2025-08-15)
- [x] Parameter class tests ✅ (commit: c464f08, 2025-08-16)
- [x] ParameterList implementation ✅ (commit: 2efbc1e, 2025-08-17)
- [x] Parameter class - readOnly and Measure Unit support ✅ (commit: 4ca2472, 2025-08-17)
- [x] Just testing and doc ✅ (commit: 86946ba, 2025-08-23)
- [x] Clean up on parameter and test completion ✅ (commit: faf83c2, 2025-08-30)
- [x] Binding test ✅ (commit: 8d3764b, 2025-09-09)
- [x] Test and Review of Parameter List ✅ (commit: 180c86f, 2025-09-11)
- [x] Parameter Serialization ✅ (commit: 92c9638, 2025-09-12)

### In Progress


### Todo Parameter
- [x] Manca il test per Min / Max
- [x] Manca il test per Preset
- [x] Manca il test per tutti i bindable
- [x] Add serialization to JSON
- [x] Add JSON de serialization
- [x] Parameter::isValid() how can I say that the parameter is invalid? 
      No Name, No Value, Bad Range and so on
- [x] Presets should be enter in the schema      
- [x] Presets should be created from schema
- [x] Add test for UniqueID
- [x] uniqueId should not be saved to JSON
- [x] Check the min and max are correct when we apply range
- [x] Check the min and max are correct when we apply schema
- [x] Test Parameter constructor using bad JSON objects
- [x] Add tooltip parameter (remember test and schema)
- [x] Add the visible parameter (remember test and schema)
- [x] Parameter::nameEdited to track oldName and NewName
- [x] Change argument position for the constructor because they are confusing
- [x] Add the QDebug << operator to simplify debug
- [x] Add a slot onChanging value so it is simpler to connect to GUI
- [x] Add rangeIsValid and his test
- [x] Add the isChanged flag 
- [x] rename isChanged to isValueChanged
- [x] handle the case where the value is back to default
      without triggering a preset or loading from json
- [ ] Update documentation 

      

### Todo ParameterList
- [x] Rivedere la gestione della lista e degli eventi
- [x] creare i test
- [x] Manca il test per tutti i bindable
- [x] Add JSON serialization 
- [x] Add JSON deserialization
- [x] Add Element as JSON Schema / Value
- [x] Emplace method to add new values 
- [x] Rename addParameter to append to be consistent
- [x] Fare il test per ParameterList::append(nullptr)
- [x] check that if the paramter name change, the list automatically update
- [x] test the first element in the list has index 0.
- [x] addParameter(Parameter *parameter) should fail with same object renamed
- [x] Add a description property
- [x] Add a tooltip property
- [x] Add a test for parameterRenameError
- [x] Add a test for isEmpty()
- [x] Add the method applyPreset()
- [x] Add test for the method applyPreset()
- [x] Add the QDebug << operator to simplify debug
- [x] Test the ownership and the destruction
- [x] Test the update values from JSON capability

- [ ] Create iterators
- [ ] Create Reverse Iterators

- [ ] Update documentation 


### Test AppParameterBasicUsage
- [x] Application that shows all elements and some feature for Paramter
- [x] Add tooltip parameter
- [x] Add the visible parameter
- [x] Add droplist to show how applyPreset()

### Test AppParameterListBasicUsage
- [x] Create the example with a list of 2 or more parameters
- [x] Add an example of the QDebug << operator 
- [x] Add a save to JSON command
- [x] Add a load from JSON command
- [x] Set bold for modified paramters

### Test AppParameterListBenchmark
- [x] Performance for Parameter and ParameterList using new and using JSON
- [ ] Add a test for linking 2 paramters using binding. Consider also biderectional
      linking.
- [ ] Add a test for linking 2 paramters using signals and slot. Consider also bidirectional
      linking.
- [ ] Test performance using bindings and using signal/slots


&nbsp;
## Next Sprint Ideas

### Text2Json:
https://jsonstudio.io/convert/TEXT-to-JSON-Converter
Create a library to convert structured text with a symple syntax to JSON
And the opposite.

From QtNoidCommonText.h
static QJsonObject jsonPlainTextToJson(const QStringList &plainText);
static bool jsonPlainTextIsString(const QString &val);
static bool jsonPlainTextIsNumber(const QString &val);
static QStringList jsonPlainTextIsArray(const QString &val);
static QStringList jsonPlainTextFromJson(const QJsonObject &json);

### Upgrade QtNoid::Common::File
- [ ] Massive File Copy / Rename / Move could became Copy Move Marshal
      Si trova in Deep  
- [ ] Massive File Rename: replace token in huge file list above multipe folders
      Forse è solo in DeepImageLoader
    


# QtNoid Development Status

## Current Sprint (Sprint-003)

### Sprint-003 Goals
- Implementation of the QtNoid::App library and release of QtNoid2 version 2.0.2



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

- [ ] Add tooltip parameter (remember test and schema)
- [ ] Add the visible parameter (remember test and schema)

- [ ] Test performance using bindings and using signal/slots
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

- [ ] Fare il test per ParameterList::append(nullptr)
- [ ] check that if the paramter name change, so it should be
  in the list or I cannot find it by name

- [ ] Add a description property
- [ ] Add a tooltip property

- [ ] fare il test con addParameter(Parameter *parameter) in cui aggiungo
lo stesso oggetto con 2 nomi diversi.


-> Gestione dei preset di tutta la pagina
  Raccolgo i nomi e poi faccio una lista per applicarli?
  
- [ ] test the first element in the list has index 0.
    --> come si comporta m_parametersByIndex.insert(m_nextParameterIndex++, parameter)?
    il primo elemento è lo 0?
   
- [ ] Update documentation 



--- TEST COVERAGE REPORT
  ParameterList header file (parameterlist.h:21-57) and the existing tests
  (test_parameterlist.cpp), here are the methods that are NOT covered by tests:

  Untested ParameterList Methods:

  Constructors:
  1. ParameterList(const QJsonObject &schemaList, const QJsonObject& valueList, QObject *parent = nullptr) - The
  constructor that takes both schema and value JSON objects is not directly tested (only individual schemaFromJson
  and valuesFromJson are tested)

  Access Methods:

  2. indexOf(Parameter* parameter) - Line 47 in header - Only indexOf(const QString& name) is tested
  3. contains(Parameter* parameter) - Line 49 in header - Only contains(const QString& name) is tested

  Overloaded Methods:

  4. removeParameter(Parameter* parameter) - Line 40 in header - Only removeParameter(const QString& name) is tested
  5. addParameter(const QJsonObject& schema, const QJsonObject& value) - Line 39 in header - This method is tested,
  but could use more edge cases

  Signal Testing:

  While signals are tested in some cases, there could be more comprehensive signal testing for:
  - countChanged signal in edge cases
  - parameterAdded signal with different scenarios
  - parameterRemoved signal with different scenarios

  Edge Cases Not Fully Covered:

  6. Boundary testing for parameter(int index) with invalid indices (partially tested)
  7. Memory management scenarios when parameters are deleted externally
  8. JSON constructor edge cases with malformed or empty JSON objects

  Summary: The main untested methods are the overloaded versions of indexOf, contains, removeParameter that take
  Parameter* objects instead of strings, and the JSON constructor. The core functionality is well-tested, but these
  specific method overloads need coverage.


### Test AppParameterBasicUsage
Application that shows all elements and some feature for Paramter

### Test AppParameterListBenchmark
- [x] Performance for Parameter and ParameterList using new and using JSON




&nbsp;
## Notes & Remarks

### Recent Development Notes
- ParameterList implementation completed
- Parameter class now supports readOnly and Measure Unit features
- Extended test coverage for Parameter class
- QtNoid App Settings fullDialogGrab functionality is complete



### Technical Decisions
 

### Known Issues


## Backlog

### Features


### Bugs


### Improvements





## Next Sprint Ideas

Text2Json:
https://jsonstudio.io/convert/TEXT-to-JSON-Converter
Create a library to convert structured text with a symple syntax to JSON
And the opposite.

From QtNoidCommonText.h
static QJsonObject jsonPlainTextToJson(const QStringList &plainText);
static bool jsonPlainTextIsString(const QString &val);
static bool jsonPlainTextIsNumber(const QString &val);
static QStringList jsonPlainTextIsArray(const QString &val);
static QStringList jsonPlainTextFromJson(const QJsonObject &json);


---> Recuperare il codice di Massive File Copy / Rename / Move
Si trova in Deep
Potrebbe essere: Copy Move Marshal

Poi c'è anche la funzione che rinomina file in massa
--> Forse è solo in DeepImageLoader


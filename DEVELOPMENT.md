# QtNoid Development Status

## Current Sprint (Sprint-003)

### Sprint-003 Goals
- Implementation of the QtNoid::App library



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
Serialization / deserialization
create and example


### Todo Parameter
- [x] Manca il test per Min / Max
- [x] Manca il test per Preset
- [x] Manca il test per tutti i bindable
- [x] Add serialization to JSON
- [x] Add JSON de serialization
- [ ] Test performance using bindings and using signal/slots
- [ ] Add tooltip parameter (remember test and schema)
- [ ] Add the visible parameter (remember test and schema)
- [ ] Add index parameter (remember test and schema)
- [ ] Update documentation 

### Todo ParameterList
- [x] Rivedere la gestione della lista e degli eventi
- [x] creare i test
- [ ] Manca il test per tutti i bindable
- [ ] Add JSON serialization 
- [ ] Add JSON deserialization
- [ ] Update documentation 


### Todo App -> Examples
- [ ] Mettere un esempio in cui si definisce un oggetto parameterList e lo si usa
per creare una pagina con alcuni oggetti dinamicamente, con valore, max, min descrizione, un pop-up
quando si supera il valore massimo / minimo del range
Ad esempio impostare la dimensione massima di log file?
Ad esempio una casella di testo + una spinbox con unità di misura max e min e pop-up 




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

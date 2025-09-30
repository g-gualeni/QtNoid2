# QtNoid Development Status

## Current Sprint (Sprint-004)

### Sprint Goals
- Implementation of the QtNoid::Txt2Json library and release of QtNoid2 version 2.2.0


### Progress Status
new project scaffolding


### Release Drafting
- [ ] Run the CLOC-Report.bat and update CLOC-REPORT.md
- [ ] Update CLOC Report
- [ ] Update the ReleaseNotes.txt




### Doc
- [ ] Doc update


### Todo Txt2Json
- [ ] placeholder



### Test Txt2Json
- [ ] placeholder



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
    


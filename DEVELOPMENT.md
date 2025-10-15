# QtNoid Development Status
&nbsp;
## Current Sprint (Sprint-004)

### Sprint Goals
- Implementation of the QtNoid::Yaml2Json library 
- Implementaton o QtNoid::App::Config class
- Release of QtNoid2 version 2.2.0

&nbsp;

### Progress Status
Implementation of Yaml2Json library 
Is there a reason to keep Txt2Json? Maybe it is faster?

Finire
void TestQtNoidJsonYaml2Json::testMixedStructures()
Fare un metodo statico per la conversione YAML to JSON


# QtNoidApp
- [x] appExeOrAppBundleDirPath();
- [x] appExeOrAppBundleFilePath();
- [x] doc appExeOrAppBundleDirPath
- [x] doc update for appExeOrAppBundleFilePath
- [x] test for appExeOrAppBundleDirPath
- [x] test update for appExeOrAppBundleFilePath
- [ ] class Config as a container for ParamterList
- [ ] class Config Test
- [ ] class Config Documentation
- [x] class GlobalConfig as static wrapper for a singletone Config
- [x] appConfig macro for easy access
- [x] class GlobalConfig test
- [ ] class GlobalConfig Documentation
- [ ] appConfig load / save configuration of current application
- [ ] Settings: appConfigFileToProjectRes(): load current configuration file into 
          project resources
- [ ] appConfigFileFromRes: init application config file from project resources
- [ ] QtNoid::App::Config create some specific save and restore method for:
     * [ ] Windows position
     * [ ] Splitter configuration



# Older Examples
- [x] AppSettingsBasicUsage update to support the new appExeOrAppBundleDirPath API
- [x] AppSettingsBasicUsage update labels using bold 
- [ ] AppSettingsBasicUsage Win11 screenshot update AppSettingsBasicUsage-Win11.png
- [ ] AppSettingsBasicUsage macOS screenshot update AppSettingsBasicUsage-macOS.png


# YAML to JSON test suite example
- [x] Scaffolding
- [x] Store all the yaml suite in a config file and add a method to
          generate in the installation folder
- [ ] App Configuration management using QtNoid::App::Config (appConfig)
- [ ] Add the debug menu that can save current config to app resources?
- [ ] Restore app config file from resources, the first time.


# App Config



### Release Drafting
- [ ] Run the CLOC-Report.bat and update CLOC-REPORT.md
- [ ] Update CLOC Report
- [ ] Update the ReleaseNotes.txt




### Doc
- [ ] Doc update

### Todo Txt2Json Parser
these are the use case

Key-value
key: value

Key-child objects
key:
        obj1:value
        obj2:value

Key-child array
key:
        obj1:[value1 value2]
        obj2:[value1 value2]

Key-array of objects
key: [
        obj1:value
        obj2:value]



### Todo Txt2Json
- [x] textNumberToJson: to parse numbers and convert to Json valid numbers
- [x] testTextNumberToJson: test for textNumberToJson
- [x] testTextArrayToJson: test the case with [AA BB]}
- [ ] test for plainTextIsKeyValuePair
- [ ] test for QString Txt2Json::textStringToJson(const QString &val)
- [ ] testPlainTextToJsonFromListOfObjects: finire con gli array

- [ ] Trigger the sub-object using key+emptyValue or {}
- [ ] Conversion of an objet with multiple sub-object
     S:
       A: Home
       B: Work
     -> { "H": {"A": "Home", "B": "Work"  }}

- [ ] Conversion of a object with multiple Arrays
     ObjName:
          Array01: [ 1  2  3 ]
          Array02: [ 10  20  30 ]          
     --> {"ObjName": {"Array01": [ 1, 2, 3], "Array02": [10, 20, 30]}}

- [ ] Conversion of an array of multiple objects with a mix of arrays and values
     Readings: [
         {CodeName: Code128             
          CodeSize: [ "5 Mils" "8 Mils" "10 Mils" "12 Mils" "15 Mils"  "20 Mils"]
          WDLimit: [382  611 763 916 1144 1525]}
     
          {CodeName: "ECC 200"
            CodeSize: [ "5 Mils" "8 Mils" "10 Mils" "12 Mils" "15 Mils" "20 Mils"]
            WDLimit: [ 230 367 458 550 687 916  ]}

Caso che funziona
ObjName:
    {Array01: [ 1  2  3 ]
    Array02: [ 10  20  30 ]}
    
Casi da provare e far funzionare:
il problema sono le ""
"H F O V": 
     {"F O V": [ 0  36  900 ]
      "W D" : [0  80  1700 ]}

HFOV: 
     {"FOV": [ 0  36  900 ]
      "WD": [0  80  1700 ]}


Readings: [
    {CodeName: Code128             
     CodeSize: [ "5 Mils" "8 Mils" "10 Mils" "12 Mils" "15 Mils"  "20 Mils"]
     WDLimit: [382  611 763 916 1144 1525]}

     {CodeName: "ECC 200"
       CodeSize: [ "5 Mils" "8 Mils" "10 Mils" "12 Mils" "15 Mils" "20 Mils"]
       WDLimit: [ 230 367 458 550 687 916  ]}


### Test Common Text
- [x] check the TestQtNoidCommonText::testTokenizeJsonArray() addining
     more test cases with string or a mixture of strings and numbers (
     mixed types array)


### Test Txt2Json
- [x] update and verify void testPlainTextIsArray();
- [x] update testTextArrayToJson()
- [ ] update testPlainTextToJsonFromListOfObjects();


&nbsp;
## Next Sprint Ideas

### Text2Json:
https://jsonstudio.io/convert/TEXT-to-JSON-Converter


### Upgrade QtNoid::Common::File
- [ ] Massive File Copy / Rename / Move could became Copy Move Marshal
      Si trova in Deep  
- [ ] Massive File Rename: replace token in huge file list above multipe folders
      Forse è solo in DeepImageLoader
    


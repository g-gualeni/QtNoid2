# QtNoid Development Status
&nbsp;
## Current Sprint (Sprint-004)

### Sprint Goals
- Implementation of the QtNoid::Yaml2Json library 
- Implementaton of QtNoid::App::Config class from 
- Implementaton of appConfig global instance 
- Review of Parameter / ParameterList after 
  deriving Config / ConfigFile / ConfigGlobal
- Release of QtNoid2 version 2.2.0

&nbsp;

### Progress Status

salvare questo trucco
QString output2 = QDebug::toString(&param2);

Implementation of Yaml2Json library 
Is there a reason to keep Txt2Json? Maybe it is faster?

Finire
void TestQtNoidJsonYaml2Json::testMixedStructures()
Fare un metodo statico per la conversione YAML to JSON

revisione di Config
fino a void Config::remove(ParameterList *page)

test di ParameterList::nameEdited(const QString &oldName, const QString &newName);

test che Parameter::nameChanged() venga emesso
test che Parameter::nameEdited() venga emesso



# QtNoidApp
- [x] appExeOrAppBundleDirPath();
- [x] appExeOrAppBundleFilePath();
- [x] doc appExeOrAppBundleDirPath
- [x] doc update for appExeOrAppBundleFilePath
- [x] test for appExeOrAppBundleDirPath
- [x] test update for appExeOrAppBundleFilePath
- [ ] class **Parameter** update
     - [ ] check Parameter Documentation
     - [x] all json names should be camelCase lowercase
     - [x] check all properties are in the JsonSchema
     - [x] check all properties are in the << Debug operator
     
- [ ] class **ParameterList** update
     - [ ] check ParameterList Documentation
     - [x] add the visible flag
     - [x] test description in schema
     - [x] test description from schema
     - [x] test tooltip in schema
     - [x] test tooltip from schema
     - [x] all json names should be camelCase lowercase
     - [x] add uniqueId
     - [x] test uniqueId
     - [x] test for QDebug operator
     - [x] test tooltip and all parameters are in Debug()
     - [ ] count should be bindable

- [ ] class **Config** as a container for ParamterList
     - [ ] class Config Documentation
     - [x] uniqueId for Parameter starts from 1000
     - [x] Add valueAsInt
     - [x] Add valueAsDouble
     - [x] Add valueAsString
     - [x] Add valueAsStringList
     - [x] Add valueAsBool
     - [x] change value to restore (restoreAsBool) 
     - [x] addRecentFile
     - [x] clearRecentFiles
     - [x] restoreRecentFiles
     - [x] remove s_nextUniqueId because it is in ParameterList
     - [x] add tooltip
     - [x] all json names should be camelCase lowercase
     - [x] test Config Debug Output 
     - [ ] check implementation of class Config
     
     - [ ] add iterator and reverse iterator for Config
     - [ ] cound should be bindable
     
     - [ ] class Config Test
     - [ ] test tooltip in schema
     - [ ] test description in schema



- [x] class **ConfigFile** as an extension of Config
     - [ ] class ConfigFile documentation         
     - [x] load / save configuration 
     - [x] class ConfigFile test     
     - [ ] test Debug() function for ConfigFile (to be implemented)


- [x] class **GlobalConfig** as static wrapper for a singletone Config
     - [x] appConfig macro for easy access
     - [x] class GlobalConfig test
     - [ ] class GlobalConfig Documentation


More:

- [ ] Settings::initFullDialogGrabShortcut fix keySequence usage
     - [ ] Add a test that verify the keysequence are correct
     - [x] Add -windows and -macOs to the screenshot and update doc
     
- [ ] Settings: appConfigFileToProjectRes(): load current configuration file into 
          project resources
- [ ] appConfigFileFromRes: init application config file from project resources


# Older Examples
- [x] AppSettingsBasicUsage update to support the new appExeOrAppBundleDirPath API
- [x] AppSettingsBasicUsage update labels using bold 
- [ ] Update all examples with the new auto identification of os in the screenshot name
     - [ ] CoreBasicUsage
     - [ ] CommonBasicUsage
     - [ ] AppSettingsBasicUsage
     - [ ] AppParameterBasicUsage
     - [ ] AppParameterListBasicUsage
     - [ ] AppParameterListBenchmark
     - [ ] JsonTxt2JsonBasicUsage
     - [ ] YAMLtoJsonTestSuite
     

# YAML to JSON test suite example
- [x] Scaffolding
- [x] Store all the yaml suite in a config file and add a method to
          generate in the installation folder
- [x] App Configuration management using QtNoid::App::Config (appConfig)
     - [x] Save/Restore Geometry
     - [x] Save restore recent file
     
- [x] Add the screenshot command
- [ ] Add the debug menu that can save current config to app resources?
- [ ] Restore app config file from resources, the first time.
- [ ] Add a run feature that run through all examples and generate a report



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

### New QtNoid::AppUI library 
 - Generate recent files menu
 - Generate About dialog
 - Generate Config UI from Config class
 
### New QtNoid::CmdUI library 
 - File Move/Copy/Rename/Marshal using pattern matching
 - File preview as tiles

### New QtNoid::DispUI library 
 - ImageViewer with all functionality
     Zoom, Pan, Keystone, Pixel value zoom
     Histogram
     Image Component
     Histogram Stretching using CLAHE

### New QtNoid::JsonUI library 
- Yaml / JSON UI editor

### Upgrade QtNoid::Common::File
- [ ] Massive File Copy / Rename / Move could became Copy Move Marshal
      Si trova in Deep  
- [ ] Massive File Rename: replace token in huge file list above multipe folders
      Forse è solo in DeepImageLoader
    

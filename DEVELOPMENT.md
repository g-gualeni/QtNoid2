# QtNoid Development Status
&nbsp;
## Current Sprint (Sprint-004)

### Sprint Goals
- Implementation of the QtNoid::Yaml2Json library: 
     - this the first implementation
     - Main goal is to handle all YAML Suite test without crash.     
- Implementaton of QtNoid::App::Config class from Parameters
- Implementaton of appConfig global instance 
- Implementation of QtNoid::App::Development class to help the application 
     development
- Review of Parameter / ParameterList after 
  deriving Config / ConfigFile / ConfigGlobal

- Release of QtNoid2 version 2.2.0

&nbsp;

### Progress Status
Rimuovere Txt2Json
Rimuovere JSON Txt2Json Basic Usage example
Avanti con YamlExample


Fare il salvataggio dei vari casi: serve una form
 - serve il percorso root
 - devo recuperare i dati dalla dialog
 - devo sapere se ho premuto Yes o Cancel
 - creare un metodo nella classe principale
 

 




&nbsp;

# QtNoidJson
finire implementation Yaml2Json library 
Finire
void TestQtNoidJsonYaml2Json::testMixedStructures()


&nbsp;

# QtNoidApp
- [x] Split the documentation into 1 file for each class
- [ ] class **Settings** update
     - [x] appExeOrAppBundleDirPath();
     - [x] appExeOrAppBundleFilePath();
     - [x] filePathAsAppSibling() not correct under macOs
     - [x] doc appExeOrAppBundleDirPath
     - [x] doc update for appExeOrAppBundleFilePath
     - [x] test for appExeOrAppBundleDirPath
     - [x] test update for appExeOrAppBundleFilePath
     - [x] fullDialogGrab: now returns QImage and fix the QColorSpace          
     - [ ] check Settings Documentation
     
- [ ] class **Parameter** update
     - [x] all json names should be camelCase lowercase
     - [x] check all properties are in the JsonSchema
     - [x] check all properties are in the << Debug operator
     - [ ] check Parameter Documentation
        
- [ ] class **ParameterList** update
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
     - [x] test signal nameEdited(old, new)
     - [x] test signal visibleChanged
     - [x] count should be bindable
     - [ ] check ParameterList Documentation

- [ ] class **Config** as a container for ParamterList
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
     - [x] check implementation of class Config
     - [x] class Config Test
     - [x] test tooltip in schema
     - [x] test description in schema
     - [x] test for Config &operator<<(ParameterList& page)
     - [x] test Parameter::nameEdited / ParameterList::nameEdited to keep names in sync
     - [x] count should be bindable
     - [x] add iterator and reverse iterator for Config
     - [x] restoreComboBoxTextItems
     - [x] saveComboBoxTextItems
     - [ ] class Config Documentation


- [ ] class **ConfigFile** as an extension of Config
     - [x] load / save configuration 
     - [x] class ConfigFile test     
     - [x] test Debug() function for ConfigFile (to be implemented)
     - [x] new event on a new file loaded (fileLoaded())
     - [ ] class ConfigFile documentation         


- [ ] class **GlobalConfig** as static wrapper for a singletone Config
     - [x] appConfig macro for easy access
     - [x] class GlobalConfig test
     - [ ] class GlobalConfig Documentation


- [ ] class **Development** as a class to support application development
     - [x] move initFullDialogGrabShortcut to Development     
     - [x] initFullDialogGrabShortcut(): 
          - [x] Add -windows and -macOs to the screenshot and update doc
          - [x] fix keySequence usage
          - [x] Add a test that verify the keysequence are correct
          - [x] update to handle also a filepath     
          
     - [ ] saveConfigToProject() -> Copy current configuration file into resources 
          - [x] Method implementation
          - [ ] Method test
          - [x] Method doc
          
     - [ ] initConfigFromResources -> init application config file from resources
          - [x] Method implementation
          - [ ] Method test
          - [x] Method doc     
          
     - [ ] class Development Documentation


&nbsp;
# QtNoidCommon
- [ ] class **Text** 
     - [x] TestQtNoidCommonText::testTokenizeJsonArray() adding
          more test cases with string or a mixture of strings and numbers 
          (mixed types array)
     

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




# Release Drafting
- [ ] Doc update
- [ ] Run the CLOC-Report.bat and update CLOC-REPORT.md
- [ ] Update CLOC Report
- [ ] Update the ReleaseNotes.txt
- [ ] Cancel the build folder and run the build from scratch for both Debug and 
      Release




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



### Test Txt2Json - ELIMINARE
- [x] update and verify void testPlainTextIsArray();
- [x] update testTextArrayToJson()
- [ ] update testPlainTextToJsonFromListOfObjects();









&nbsp;

&nbsp;

# Next Sprint Ideas

Yaml to Json validation test
- [ ] Add a run feature that run through all examples and generate a report

## New QtNoid::AppUI library 
 - Generate recent files menu
 - Generate About dialog
 - Generate Config UI from Config class
 
## New QtNoid::CmdUI library 
 - File Move/Copy/Rename/Marshal using pattern matching
 - File preview as tiles

## New QtNoid::DispUI library 
 - ImageViewer with all functionality
     Zoom, Pan, Keystone, Pixel value zoom
     Histogram
     Image Component
     Histogram Stretching using CLAHE

## New QtNoid::JsonUI library 
- Yaml / JSON UI editor

## Upgrade QtNoid::Common::File
- [ ] Massive File Copy / Rename / Move could became Copy Move Marshal
      Si trova in Deep  
- [ ] Massive File Rename: replace token in huge file list above multipe folders
      Forse è solo in DeepImageLoader
    

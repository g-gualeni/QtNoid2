# QtNoid Development Status

# Current Sprint (Sprint-004)

# Sprint Goals
- Implementation of the QtNoid::Yaml2Json library: 
     - this the first implementation
     - Main goal is to handle all YAML Suite test without crash.     
- Implementation of QtNoid::App::Config class from Parameters
- Implementation of appConfig global instance 
- Implementation of QtNoid::App::Development class to help the application    development
- Implementation of `QtNoid::UiCommon::Core` class with just 2 methods: `restoreComboBoxTextItems() saveComboBoxTextItems()`
- Review of Parameter / ParametersPage after deriving Config / ConfigFile / ConfigGlobal
- New release using Qt 5.12
- Release of QtNoid2 version 2.2.0

# Library Hierarchy
* QtNoidCore
* QtNoidCommon
* QtNoidApp
* QtNoidUiCommon



# Da fare
	-> finisci esempio QtNoid App Parameter Basic Usage
	-> continua il controllo delle classi partendo da ConfigFile (implementazione, test, documentazione)

# Progress Status

- [x] Controllo di tutti i progetti Documentazione
- [x] Revisione QtNoid::App::Parameter
- [x] Rinominare la classe ParameterList in ParametersPage per renderla più comprensibile

- [ ] Esempio **QtNoid Core Basic Usage**
	- [ ] Aggiornare screenshot macOS
- [ ] Esempio **QtNoid::Common Basic Usage**
	- [x] Dialog review with testing for relative and absolute path
	- [x] Form Geometry stored in JSON
	- [x] Recent folder list stored
	- [x] Example documentation
	- [x] Screenshot Win11
	- [x] Inserire save restore combo box
	- [ ] Aggiornare screenshot macOS
- [ ] Esempio **QtNoid Common Text Basic Usage**
	- [x] Creare esempio per QtNoid::Common::Text Basic Usage
	- [x] `tokenize()`
	- [x] `convertToCamelCase()`
	- [x] `convertToSnakeCase()`
	- [x] Inserire Screenshot Win11
	- [x] Aggiornare Doc
	- [ ] Inserire screenshot macOS
- [ ] Aggiornare esempio **QtNoid App Core Basic Usage**
	- [x] update to support the new `appExeOrAppBundleDirPath()` API
	- [x] Aggiornare UI
	- [x] Update labels using bold 
	- [x] Update Screenshot win11
	- [x] Aggiornare Doc
	- [ ] Aggiornare screenshot macOS


- [ ] Esempio **QtNoid App Parameter Basic Usage**
	- ==controllare non va bene -> Sistemare preset il resto è ok==
	- [ ] Modificare la funzione updateStatusBar() per gestire messaggio e value
	- [ ] Inserire  il testo in italics per evidenziare il campo value
	- [x] Creare il test per segnale changed()
	- [x] automatizzareJSON
	- [x] Documentare changed() e spiegare il consolidamento dei messaggi in uno solo
	- [x] Aggiornare UI
	- [x] rendere automatica generazione del JSON usando un nuovo evento della classe Parameter che mi notifica quando qualcosa cambia.
	- [x] Aggiornare Doc
	- [ ] Aggiornare screenshot macOS
- [ ] Esempio **QtNoid App ParametersPage Basic Usage**
	- [x] Aggiornare UI
	- [x] Aggiornare documentazione
	- [x] Aggiornare screenshot Win11
	- [ ] Aggiornare screenshot macOS
- [ ] Esempio **QtNoid::App::ParametersPage Benchmark**
	- [x] Aggiornare UI
	- [x] Scrivere la documentazione
	- [x] Screenshot Win11
	- [ ] Screenshot masOs

- [x] Classe **`QtNoid::Common::Text`** 
     - [x] `TestQtNoidCommonText::testTokenizeJsonArray()` adding more test cases with string or a mixture of strings and numbers  (mixed types array)
- [x] class **`QtNoid::Common::File`** 
     - [x] listPathRecursively() converted to static to simplify usage
     - [x] listPathRecursively(): optimization for speed
     - [x] create listSubPathRecursively() to return relative path
     - [x] Doc for listSubPathRecursively()
     - [x] Test for listSubPathRecursively()

- [x] Libreria **`QtNoidApp`**
	- [ ] Aggiungere la dipendenza da `QtNoidCommon`
- [x] Classe **`QtNoid::App::Core`**
   - [x] test di `mainWindowFromWidget()`: reference instead of pointer version
   - [x] test di `groupNameFromObjectOrClass()`: reference instead of pointer version
- [x] Classe **`QtNoid::App::ParamtersPage`**
	- [x] Inserire segnale `changed()`
	- [x] Inserire segnale `aboutToBeDestroyed()`
- [x] Classe **`QtNoid::App::Config`**
	- [x] Studiare la classe Config
	- [x] Inserire Label
	- [x] Inseire UniqueID
	- [x] isValueChanged
	- [x] Inserire changed()
	- [x] aboutToBeDestroyed()
	- [x] Aggiornare la documentazione di Config
	- [x] C'è un test che fallisce
	- [x] Non mi piace changed(). Cambiare la gestione usando un contatore di modifiche
	- [x] Chi lo resetta questo contatore? Posso evitare di mantenere il valore di default? Oppure avrebbe senso dato che potrei gestire il comando Annulla e aggiungere un metodo tipo ResetToDefault?
	- [x] Paramter
	- [x] ParamtersPage
	- [x] Config
	- [x] Non vedo il meccanismo con il contatore delle modifiche

- [x] classe **`QtNoid::UiCommon::Core`** 
	- [x] Documentare `restoreComboBoxTextItems()` 
	- [x] Documentare `saveComboBoxTextItems()` 
	* [x] `restoreComboBoxTextItems()` clean for duplicated items





Rimuovere Txt2Json
Rimuovere JSON Txt2Json Basic Usage example
Avanti con YamlExample
* Sistemare i campi nella finestra di dialogo principale
 - Creare il dataset per i numeri
 - Aggiornare anche qrc: 
	 - creo un nuovo file e faccio in modo di inserirlo dentro alle risorse in modo che basta ricompilare e il nuovo file è presente
- Aggiungere override (magari chiedere conferma se la cartella esiste) per la creazione di un dataset locale. Al momento fallisce e basta
-  Sistemare il caricamento dei dati con le frecce nella form principale
	- Nel compare gestire il caso cancellazione ovvero dal lato del file esistente orfano mettere ->x
	- Gestire il file compare nel caso in cui il file sia solo da un lato.   

 

# QtNoidJson
finire implementation Yaml2Json library 
Finire
void TestQtNoidJsonYaml2Json::testMixedStructures()




# Project Documentation check status
* [x] QtNoidCore
	* [x] Check again
* [x] QtNoidCommon
	* [x] File
	* [x] Scale
	* [x] Text
* [x] QtNoidApp
	* [x] Split the documentation into 1 file for each class
	* [x] QtNoid::App::Core
	* [x] QtNoid::App::Parameter
	* [x] QtNoid::App::ParameterList
	* [x] QtNoid::App::Config: completata
	* [ ] QtNoid::App::ConfigFIle è abbozzata
	* [ ] QtNoid::App::ConfigGlobal è abbozzata
	* [ ] QtNoid::App::Development è da finire di controllare
* [ ] QtNoidJson: - è ancora in sviluppo
* [ ] Examples ancora in sviluppo
	* [ ] JsonTxt2JsonBasicUsage
	* [ ] JsonYamlToJsonTestSuite


# QtNoidApp
- [x] class **Core** update
	 - [x] Rename QtNoid::App::Settings to QtNoid::App::Core
     - [x] appExeOrAppBundleDirPath();
     - [x] appExeOrAppBundleFilePath();
     - [x] filePathAsAppSibling() not correct under macOs
     - [x] doc appExeOrAppBundleDirPath
     - [x] doc update for appExeOrAppBundleFilePath
     - [x] test for appExeOrAppBundleDirPath
     - [x] test update for appExeOrAppBundleFilePath
     - [x] fullDialogGrab: now returns QImage and fix the QColorSpace          
     - [x] mainWindowFromWidget(): reference instead of pointer version
     - [x] groupNameFromObjectOrClass(): reference instead of pointer version
     - [x] check Core Documentation
     
- [ ] class **Parameter** update
     - [x] all json names should be camelCase lowercase
     - [x] check all properties are in the JsonSchema
     - [x] check all properties are in the << Debug operator
     - [x] check Parameter Documentation
        
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
     - [x] check ParameterList Documentation

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
     - [x] class Config Documentation


- [ ] class **ConfigFile** as an extension of Config
     - [x] load / save configuration 
     - [x] class ConfigFile test     
     - [x] test Debug() function for ConfigFile (to be implemented)
     - [x] new event on a new file loaded (fileLoaded())
     - [ ] class ConfigFile documentation         


- [ ] class **GlobalConfig** as static wrapper for a singleton Config
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
          - [x] update handling for macOs destination path
          - [x] update handling multiple screenshot using autoNaming().
     - [ ] saveConfigToProject() -> Copy current configuration file into resources 
          - [x] Method implementation
          - [ ] Method test
          - [x] Method doc
     - [ ] initConfigFromResources -> init application config file from resources
          - [x] Method implementation
          - [ ] Method test
          - [x] Method doc     
     - [ ] class Development Documentation








---
# YAML to JSON test suite example
- [x] Scaffolding
- [x] Store all the yaml suite in a config file and add a method to
          generate in the installation folder
- [x] App Configuration management using QtNoid::App::Config (appConfig)
     - [x] Save/Restore Geometry
     - [x] Save restore recent file
     
- [x] Add the screenshot command
- [x] Add the debug menu that can save current config to app resources?
- [x] Restore app config file from resources, the first time.
- [x] Save Dataset command and dialog
- [x] Ctrl+S to save
- [x] Ctrl+J to convert to Json
- [x] Create new Dataset command and dialog
- [ ] Add automation to init new folder and description
- [ ] Create a resource manager to copy files into resouces end generate list
     - [ ] File Copy to prject folder
     - [ ] Scroll of all prefix
     - [ ] Update of prefix content
     




---


# Release Drafting
- [ ] Doc update
- [ ] Run the CLOC-Report.bat and update CLOC-REPORT.md
- [ ] Update CLOC Report
- [ ] Update the ReleaseNotes.txt
- [ ] Cancel the build folder and run the build from scratch for both Debug and 
      Release











---
---
---

This is planned to be removed
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








---
# Next Sprint Ideas

# Qt Noid User Interface Library

**QtNoid::UiCommon**: this is a collection of simple functionality that all desktop application need.
- [ ] QtNoidUiCommon::Core
	- [x] ComboBox config Save/Restore from settings
	- [ ] Recent Files Menu from settings
	- [ ] QtNoidUiCommon::Combo: Folder comboBox Widget control with button to open file from folder and keep recent list using appConfig file.
	- [ ] QtNoid::UiCommon::Core::About
		- [ ] About Dialog with library information, application folder access, configuration file access
	- [ ] QtNoid::UiCommon::Log
		- [ ] UI for log display (Clear / Save to file / load from file)
	- [ ] QtNoid::UiCommon::Doc
		- [ ] Mark Down viewer for documentation
	- [ ] QtNoid::UiCommon::Settings: this dialog merge schema and value into an automatically generated dialog.
	- [ ] QtNoid::UiCommon::Yaml: this dialog help the creation of a YAML script.

**QtNoid::UiLogger**: this is a library with all you need to get logger from a device, save them, merge, display, rotate log files, search and so on

**QtNoid::UiDisplay**: this is a library that handle image display:  ImageViewer with all functionality: Zoom, Pan, Keystone, Pixel value zoom, Histogram, Image Component, Histogram Stretching using CLAHE
	 **Keystone**: When a projector is tilted up, down, or sideways relative to the wall, the image stops being a perfect rectangle and becomes a **trapezoid** (narrower at one end, wider at the other). The term comes from its resemblance to the wedge-shaped keystone in stone arches.
	
**QtNoid::UiCommander:** this is a library with all widget and class necessary to play with files with a strong commitments on images
	- [ ] File Move/Copy/Rename/Marshal using pattern matching
	- [ ] File preview as tiles

## Yaml to Json validation test
- [ ] Add a run feature that run through all examples and generate a report

## New QtNoid::JsonUI library 
- Yaml / JSON UI editor

## Upgrade QtNoid::Common::File
- [ ] Massive File Copy / Rename / Move could became Copy Move Marshal 
- [ ] Massive File Rename: replace token in huge file list in multiple folders

# Known Issue
* [ ] fullDialogGrab()  wrong display captured on macOs. It take a screenshot from the main display of the computer.

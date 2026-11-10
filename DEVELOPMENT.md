# QtNoid Development Status

> See also **[UserStories/RoadMap.md](UserStories/RoadMap.md)**
>
> See also **[QtNoidJson - DEV](#qtnoidjson-dev)**
> 



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
Passa a QtNoidJson - Yaml2Json

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
- [ ] Esempio **QtNoid App Core Basic Usage** 
	- [x] update to support the new `appExeOrAppBundleDirPath()` API
	- [x] Aggiornare UI
	- [x] Update labels using bold 
	- [x] Update Screenshot win11
	- [x] Aggiornare Doc
	- [ ] Aggiornare screenshot macOS


- [ ] Esempio **QtNoid App Parameter Basic Usage**
	- [x] Fare in modo che la modifica al preset salvi la configurazione nel json
	- [x] Modificare la funzione updateStatusBar() per gestire messaggio e value
	- [x] Log rivisto per renderlo più intelleggibile
	- [x] Creare il test per segnale changed()
	- [x] automatizzareJSON
	- [x] Documentare changed() e spiegare il consolidamento dei messaggi in uno solo
	- [x] Aggiornare UI
	- [x] rendere automatica generazione del JSON usando un nuovo evento della classe Parameter che mi notifica quando qualcosa cambia.
	- [x] Aggiornare Doc
	- [x] Aggiornare screenshot macOS
- [ ] Esempio **QtNoid App ParametersPage Basic Usage**
	- [x] Aggiornare UI
	- [x] Aggiornare documentazione
	- [x] Aggiornare screenshot Win11
	- [ ] Aggiornare screenshot macOS
- [ ] Esempio **QtNoid App ParametersPage Benchmark**
	- [x] Aggiornare UI
	- [x] Scrivere la documentazione
	- [x] Screenshot Win11
	- [ ] Screenshot macOS

* [x] Libreria **`QtNoidCore`**
	* [x] Check again

* [x] Libreria **`QtNoidCommon`**
	- [x] Classe **`QtNoid::Common::Text`** 
	     - [x] `TestQtNoidCommonText::testTokenizeJsonArray()` adding more test cases with string or a mixture of strings and numbers  (mixed types array)
	- [x] class **`QtNoid::Common::File`** 
	     - [x] listPathRecursively() converted to static to simplify usage
	     - [x] listPathRecursively(): optimization for speed
	     - [x] create listSubPathRecursively() to return relative path
	     - [x] Doc for listSubPathRecursively()
	     - [x] Test for listSubPathRecursively()
	     - [x] readAsString() to simplify reading a text file and feed it into a QPlainText
	     - [x] readAsString() -> Scrivere doc
	     - [x] readAsString() -> Scrivere test
	     - [x] resolveAndPrepareFilePath nuova funzione pubblica
	     - [x] resolveAndPrepareFilePath documentare
	     - [x] resolveAndPrepareFilePath testare
	     - [x] riscrivi saveAsTextFile usando QSaveFile file(fileName); per usare file temporaneo e poi commit.

- [x] Libreria **`QtNoidApp`**
	- [x] Aggiungere la dipendenza da `QtNoidCommon`
	- [x] Classe **`QtNoid::App::Core`**
		- [ ] Rename QtNoid::App::Settings to QtNoid::App::Core
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
		 - [x] test di `mainWindowFromWidget()`: reference instead of pointer version
		 - [x] test di `groupNameFromObjectOrClass()`: reference instead of pointer version
	- [x] Classe  **`QtNoid::App::Paramter`** 
		- [x] all json names should be camelCase lowercase
		- [x] check all properties are in the JsonSchema
		- [x] check all properties are in the << Debug operator
		- [x] check Parameter Documentation
	- [x] Classe **`QtNoid::App::ParamtersPage`**
		- [x] Rename from **`ParameterList`** to **`ParametersPage`** 
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
		- [x] Inserire segnale `changed()`
		- [x] Inserire segnale `aboutToBeDestroyed()`
	- [x] Classe **`QtNoid::App::Config`**
		- [x] uniqueId for Parameter starts from 20
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
		- [x] saveValue (qsizetype)  / restoreAsQSizeType()
			- [x] Implementazione
			- [x] sistemare i test per la coppia
			- [x] sistemare la documentazione per la coppia
		- [x] eliminare `restoreRecentFiles()`
		- [x] eliminare `clearRecentFiles()`
		- [x] eliminare `addRecentFile()`
	
	- [x] Classe **`QtNoid::App::ConfigFile`**
		- [x] class **ConfigFile** as an extension of Config
		- [x] load / save configuration 
		- [x] class ConfigFile test     
		- [x] test Debug() function for ConfigFile (to be implemented) 
		- [x] new event on a new file loaded (fileLoaded())
		- [x] Inserire signal per il caso errore di caricamento file
		- [x] Creare il test per fileLoadError()
		- [x] Sistemare la documentazione
		- [x] Creare il test per bindableFileName()
		- [x] Controllo generale
		- [x] Controllo dei test
		- [x] Controllo documentazione
	- [x] Classe **`QtNoid::App::ConfigGlobal`**: static wrapper for a singleton Config
		- [x] appConfig macro for easy access
		- [x] class GlobalConfig test
		- [x] class GlobalConfig Documentation
		- [x] Controllo generale
		- [x] Controllo dei test
		- [x] Controllo documentazione
	- [ ] Classe **`QtNoid::App::Development`**
		- [x] move initFullDialogGrabShortcut to Development  
		- [x] initFullDialogGrabShortcut(): 
			- [x] Add -windows and -macOs to the screenshot and update doc
	        - [x] fix keySequence usage
	        - [x] Add a test that verify the keysequence are correct
	        - [x] update to handle also a filepath     
	        - [x] update handling for macOs destination path
	        - [x] update handling multiple screenshot using autoNaming().
		- [x] initFullDialogGrabShortcut() - codice
		- [x] initFullDialogGrabShortcut() - test
		- [x] initFullDialogGrabShortcut() - documentazione
		- [ ] saveConfigToProject() -> Copy current configuration file into resources 
	        - [x] Method implementation
		    - [ ] Method test
			- [ ] Method doc
		 - [ ] initConfigFromResources -> init application config file from resources
			 - [x] Method implementation
			 - [ ] Aggiornamento implementazione
	         - [ ] Method test
             - [ ] Method doc     

- [ ] Libreria **`QtNoidUiCommon`**
	- [x] classe **`QtNoid::UiCommon::Core`** 
		- [x] Documentare `restoreComboBoxTextItems()` 
		- [x] Documentare `saveComboBoxTextItems()` 
		* [x] `restoreComboBoxTextItems()` clean for duplicated items
	* [x] classe **`QtNoid::UiCommon::RecentFiles`** 
		* [x] Creo progetto
		* [x] Creto test
		* [x] Creo pagina documentazione
		* [x] Costruttore che parte da una QMainWindow
		* [x] Inserisce il menù RecentFiles
		* [x] Poi legge la lista da un json
		* [x] Scrive la lista in un json
		* [x] emette un signal quando un elemento viene selezionato 
		* [x] aggiorna la lista quando viene inserito un nuovo file, che diventa il primo della lista e poi controlla che il file non fosse già presente e controlla che la lunghezza della lista sia di ... 10 elementi
		* [x] inserire setter/getter per numero di elementi della lista

- [ ]  Libreria **`QtNoidJson`**
	- [x] Rimuovere Txt2Json
	- [x] Rimuovere JSON Txt2Json Basic Usage example
	- [x] QtNoidJson.md - aggiungere pagina per Yaml2Json
	- [x] Incorporare rapidyaml come libreria
	- [ ] Creare un metodo per fare da json a yaml
	- [ ] Provare a compilare su macOS
	- [ ] ==🟡Come faccio a rendere esplicita la dipendenza da rapidyaml?==
	- [ ] Non mi piace dipendere da GitHub per compilare, meglio qualcosa di statico


 

# QtNoidJson-DEV
finire implementation Yaml2Json library 
Finire
void TestQtNoidJsonYaml2Json::testMixedStructures()
- [ ] Esempio **QtNoid::Json::Yaml2Json JsonYamlToJsonTestSuite**
	- [x] Sistemare save / restore dei parametri iniziali
	- [x] Inserire Tab per selezionale modalità file o collection
		- [x] Crea il tab
		- [x] Salva / recupera la configurazione
		- [x] Attiva il salvataggio del file singolo
		- [x] Cambia menu da load Yaml a load single file
		- [x] Sincronizza contenuto pannelli con contenuto tab source
		- [x] sistemare l'attivazione del file usato l'ultima volta
		- [x] Mostrare il tab di errore se la conversione è fallita o il tab JSON se ha successo
	- [x] Sistemare la gestione della libreria di test
		- [x] Gestire il caricamento in fase di startup
		- [x] Quando clicco su collection carico il test selezionato
		- [x] Mostrare expected error se il file è presente nel test
		- [x] updateUI_loadYamlFile: divedere in due parti con updateUI_loadExpectedFiles
		- [x] updateUI_yamlInput(loadSingleFile(path)); servono o posso collassare su updateUI_loadYamlFile()?
		- [x] Sistema file->Load single file
		- [x] updateUI_statusBar() semplificare e rimuovere il calcolo del percorso spostandolo la responsabilità sul chiamanante
		- [x] yamlStatusBarMsgFromCollectionList sbaglia il calcolo
		- [x] C'è un problema con il marker del file modificato `[*] [*]`
		- [x] Gestire il salvataggio del file yaml usando saveAsTextFile(...)
		- [x] Gestire il caso del YAML modificato, con un pop-up per fare save delle modifiche
			- [x] Per il cambio tab
			- [x] per il caso file-openYaml
			- [x] per il caso doppi click sulla cartella
			- [x] per il caso prev / next
			- [x] per la selezione di un file dalla lista dei files
			- [x] per la selezione di un recent file
	- [x] Usare la nuova classe QtNoid::UiCommon::RecentFiles

		
	- [ ] Mostrare un messaggio se il file atteso e quello caricato non sono gli stessi, usando confronto tra modelli json
				
	- [ ] Migliorare il calcolo della cartella base della libreria rendendola invariante rispetto alla selezione. (la prima cartella che ne contiene più di una, oppure una cartella sopra i singoli file)
	- [ ] Testare i metodi development per gestire il file di configurazione
	- [ ] Devo anche creare un file esempio e metterlo nelle risorse
	- [ ] Migliorare estrazione risorse mettendo magari una dialog per mostrare i progressi
 
* Creare il dataset per i numeri
* Rinominare il dataset QtNoid e farlo a 3 livelli
* 
 - Aggiornare anche qrc: 
	 - creo un nuovo file e faccio in modo di inserirlo dentro alle risorse in modo che basta ricompilare e il nuovo file è presente
- Aggiungere override (magari chiedere conferma se la cartella esiste) per la creazione di un dataset locale. Al momento fallisce e basta
-  Sistemare il caricamento dei dati con le frecce nella form principale
	- Nel compare gestire il caso cancellazione ovvero dal lato del file esistente orfano mettere ->x
	- Gestire il file compare nel caso in cui il file sia solo da un lato.   







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
     
## Yaml to Json validation test
- [ ] Add a run feature that run through all examples and generate a report




---


# Release Drafting
- [ ] Doc update
- [ ] Run the CLOC-Report.bat and update CLOC-REPORT.md
- [ ] Update CLOC Report
- [ ] Update the ReleaseNotes.txt
- [ ] Cancel the build folder and run the build from scratch for both Debug and 
      Release











---
# Known Issue
* [ ] fullDialogGrab()  wrong display captured on macOs. It take a screenshot from the main display of the computer.

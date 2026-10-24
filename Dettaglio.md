# Qt Noid User Interface Library

- [ ] **QtNoid::UiCommon**: this is a collection of simple functionality that all desktop application need.
	- [ ] QtNoid::UiCommon::Core
		- [ ] ComboBox config Save/Restore from settings
		- [ ] Recent Files Menu from settings
	- [ ] QtNoid::UiCommon::Combo: Folder comboBox Widget control with button to open file from folder and keep recent list using appConfig file.
	- [ ] QtNoid::UiCommon::Core::About
		- [ ] About Dialog with library information, application folder access, configuration file access
	- [ ] QtNoid::UiCommon::Log
		- [ ] UI for log display (Clear / Save to file / load from file)
	- [ ] QtNoid::UiCommon::Doc
		- [ ] Mark Down viewer for documentation
	- [ ] QtNoid::UiCommon::Settings: this dialog merge schema and value into an automatically generated dialog.
	- [ ] QtNoid::UiCommon::Yaml: this dialog help the creation of a YAML script.

- [ ] **QtNoid::UiLogger**: this is a library with all you need to get logger from a device, save them, merge, display, rotate log files, search and so on

- [ ] **QtNoid::UiDisplay**: this is a library that handle image display:  ImageViewer with all functionality: Zoom, Pan, Keystone, Pixel value zoom, Histogram, Image Component, Histogram Stretching using CLAHE
	 **Keystone**: When a projector is tilted up, down, or sideways relative to the wall, the image stops being a perfect rectangle and becomes a **trapezoid** (narrower at one end, wider at the other). The term comes from its resemblance to the wedge-shaped keystone in stone arches.
- [ ] **QtNoid::UiCommander:** this is a library with all widget and class necessary to play with files with a strong commitments on images
	- [ ] File Move/Copy/Rename/Marshal using pattern matching
	- [ ] File preview as tiles


### togliere questi metodi dalla classe config. 
- [ ] void restoreComboBoxTextItems(QComboBox* cbo, const QString& paramName, const QStringList& defaultValue, const QString& pageName = "Settings") const;
- [ ] void saveComboBoxTextItems(QComboBox* cbo, const QString& paramName, const QString& pageName = "Settings");
==🟡-> Vanno messi nella futura classe QtNoid::UiCore==



# Next Sprint Ideas / Roadmap

> Back to [DEVELOPMENT.md](../DEVELOPMENT.md)

## Library hierarchy
* [QtNoid::Core library](#QtNoid%20Core%20library)
* [QtNoid::Common library](#QtNoid%20Common%20library)
* [QtNoidApp library](#QtNoidApp%20library)
* [QtNoid::Json library](#QtNoid%20Json%20library)
* [QtNoid::UiCommon:](#QtNoid%20UiCommon)
* **[New Libraries](#New%20Libraries)**


## QtNoid::Core library
-> No planned updates

## QtNoid::Common library
In the QtNoid::Common::File class:
This is just a library, without UI
- [ ] Massive File Copy / Rename / Move could became Copy Move Marshal
- [ ] Massive File Rename: replace token in huge file list in multiple folders

## QtNoidApp library
-> No planned updates

## QtNoid::Json library
-> No planned updates

## QtNoid::UiCommon:
this is a collection of simple functionality that all desktop application need.

- [ ] **QtNoidUiCommon::Core**

- [ ] **QtNoidUiCommon::ComboFile:** 
	- Combo box wrapper to manage a file name
	- With a rightClick menu:
		- Show File in OS Browser (File Explorer)

- [ ] **QtNoidUiCommon::ComboFolder:** 
	- Based on FolderComboBox from JsonYamlToJsonTestSuite
	- QComboBox control used control folder selection from filesystem
	- With a rightClick menu
		- Reset recent file list
		- Open Folder
		- Show Folder in OS Browser (File Explorer)
	- Storing recent path list in the combo box drop-down
	- Double click to open the openFolder QFileDialog
	- Automatically select just the folder even if we select a file

- [ ] **QtNoid::UiCommon::About**
	- About Dialog that displays:
		- library information, 
		- application folder access, 
		- configuration file access
	- Add a static method to create the About menu

- [ ] **QtNoid::UiCommon::Log**
	- UI for log display (Clear / Save to file / load from file)

- [ ] **QtNoid::UiCommon::Doc**
	- Mark Down viewer for documentation with hyperlink triggering web browser

- [ ] **QtNoid::UiCommon::Settings:**
	- merge schema and value into an automatically generated dialog.

- [ ] **QtNoid::UiCommon::Yaml**: 
	- help the creation of a YAML script.



---
# New Libraries
## QtNoid::UiLogger:
this is a library with all you need to get logger from a device, save them, merge, display, rotate log files, search and so on

## QtNoid::UiDisplay:
this is a library that handle image display:  ImageViewer with all functionality: Zoom, Pan, Keystone, Pixel value zoom, Histogram, Image Component, Histogram Stretching using CLAHE
	 **Keystone**: When a projector is tilted up, down, or sideways relative to the wall, the image stops being a perfect rectangle and becomes a **trapezoid** (narrower at one end, wider at the other). The term comes from its resemblance to the wedge-shaped keystone in stone arches.

## QtNoid::UiCommander:
this is a library with all widget and class necessary to play with files with a strong commitments on images:
- [ ] File Move/Copy/Rename/Marshal using pattern matching
- [ ] File preview as tiles
- [ ] File selection

## QtNoid::ImgCollection library
Library to handle images as a collection creating a database of thumbnails, smart thumbnails, pre calculated preview
The resulting collection can be feed into UICommander or UI Display for a fast image browsing

## QtNoid::JsonUI library
- [ ] Yaml / JSON UI editor
- [ ] Apply syntax highlighting
- [ ] Simplify creation using snippets



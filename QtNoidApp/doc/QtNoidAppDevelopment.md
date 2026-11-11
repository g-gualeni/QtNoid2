# Class: Development
This is a collection of internal API that facilitate application development by automating common integration tasks. These APIs are for development use only and are not intended for customer-facing implementations.
The design prioritizes ease of integration through automation, though this approach may limit flexibility in certain scenarios.

## Static Methods

- `QShortcut *initFullDialogGrabShortcut(QWidget *parent, const QString &keySequence = "Ctrl+Shift+S", QString destinationFileOrPath = {}, bool saveToClipboard = true, bool openDestinationPath = true)`:
  A helper to simplify creating screenshots of an application, for documentation purposes. It creates and returns a keyboard shortcut that, when triggered, grabs a screenshot and saves it to disk, optionally also copying it to the clipboard and revealing the folder it was saved to. Returns `nullptr`, without doing anything else, if `parent` is `nullptr`.

  The screenshot itself is of the `QMainWindow` found by walking up from `parent` (its frame, not just `parent`'s own geometry) - if `parent` isn't, and isn't contained in, a `QMainWindow`, nothing is captured (an empty image) and saving it fails.
	* **Parameters**:
		* **parent**: the widget that will own the shortcut (destroyed together with it), and whose `QMainWindow` ancestor is captured. It's also used, when `destinationFileOrPath` doesn't name a file, to find that same `QMainWindow` and use its title to build the screenshot's file name.
		* **keySequence**: the key combination that triggers the screenshot. Defaults to `"Ctrl+Shift+S"`, which (like any sequence built from the portable `"Ctrl"` mnemonic) is triggered with Cmd+Shift+S on macOS, since Qt remaps `Ctrl`/`Cmd` on that platform.
		* **destinationFileOrPath**: a full file path for the screenshot, a folder to save it in, or empty. If it names a file (i.e. it has a suffix), that exact path is used as-is. Otherwise it's treated as a folder - the application's own folder (`Core::appExeOrAppBundleDirPath()`) if empty - and the file name is built as `<Title>-<os>.png`, where `<Title>` is the camelCase title of the `QMainWindow` found from `parent` (or just `Screenshot` if none is found) and `<os>` is `QSysInfo::productType()` (e.g. `windows`, `macos`). Either way, [`QtNoid::Common::File::autoNaming()`](../../QtNoidCommon/doc/QtnoidCommonFile.md) is applied to avoid overwriting an existing screenshot, and the destination folder is created automatically (via `QDir::mkpath()`) if it doesn't exist yet. If that folder can't be created, saving fails the same way as any other save failure.
		* **saveToClipboard**: if true, also copies the captured image to the system clipboard, whether or not saving to disk succeeded.
		* **openDestinationPath**: if true, and the screenshot was saved successfully, opens (via `QDesktopServices::openUrl()`) the folder that received the file.
	* **Returns**: a pointer to the configured `QShortcut`, owned by `parent` and destroyed together with it; or `nullptr` if `parent` is `nullptr`.


- `QAction *createSaveConfigToProjectMenuAction(QMenu *developmentMenu, const QString &projectFolder)`:
  Adds a "Copy ConfigFile To Project Resources" action to `developmentMenu` that, when triggered, overwrites the project's own resource copy of the config file (the one baked into the app via a `.qrc`) with the current live `appConfig` file - so the next build ships whatever configuration the app is currently running with. The action is only added once everything it needs has actually been found; if anything is missing, the method returns `nullptr` and `developmentMenu` is left untouched (no dead/disabled action is ever attached).
  To locate the file to overwrite, it first searches `projectFolder` recursively for a `.qrc` file - the resources are assumed to live in that same folder - then, inside that folder, for a file whose name matches `appConfig->fileName()` and whose extension matches it too. If more than one `.qrc` is found, the first one is used.
	* **Parameters**:
		* **developmentMenu**: the root menu on the dialog where the new `QAction` will be attached. Must have a non-null `parent()` (the action's connection is parented to it); returns `nullptr` otherwise.
		* **projectFolder**: the absolute path of the project folder as it is captured from CMake. The simplest way to get this information in the code is to use a compiler variable such as:  SOURCE_FILES_PATH. Insert this in CMakeLists.txt
		  target_compile_definitions(${PROJECT_NAME}
				PRIVATE
			        SOURCE_FILES_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
			)
	* **Returns**: a pointer to the configured `QAction`, already added to `developmentMenu`; or `nullptr` if `developmentMenu` is `nullptr`, `projectFolder` is empty, no `.qrc` file is found under `projectFolder`, no file matching the current config's name/suffix is found next to it, or `developmentMenu`'s `parent()` is `nullptr`.


- `QAction *initConfigFromResources(QMenu *developmentMenu, const QString &resFileOrPrefix)`:
   Parameters:
   - **developmentMenu**: this is the root menu on the dialog where the new QAction will be attached
   - **resFileOrPrefix**: this is the configuration file name as saved in the resources or just the 
   prefix, suche as :/resources since the fileName can be gathered via the appConfig global variable.





[⬆ Back to QtNoidApp](QtNoidApp.md)
[← Back to README](../../README.md)

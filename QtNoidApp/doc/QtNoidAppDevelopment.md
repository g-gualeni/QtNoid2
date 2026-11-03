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
		* **destinationFileOrPath**: a full file path for the screenshot, a folder to save it in, or empty. If it names a file (i.e. it has a suffix), that exact path is used as-is. Otherwise it's treated as a folder - the application's own folder (`Core::appExeOrAppBundleDirPath()`) if empty - and the file name is built as `<Title>-<os>.png`, where `<Title>` is the camelCase title of the `QMainWindow` found from `parent` (or just `Screenshot` if none is found) and `<os>` is `QSysInfo::productType()` (e.g. `windows`, `macos`). Either way, [`QtNoid::Common::File::autoNaming()`](../../QtNoidCommon/doc/QtnoidCommonFile.md) is applied to avoid overwriting an existing screenshot.
		* **saveToClipboard**: if true, also copies the captured image to the system clipboard, whether or not saving to disk succeeded.
		* **openDestinationPath**: if true, and the screenshot was saved successfully, opens (via `QDesktopServices::openUrl()`) the folder that received the file.
	* **Returns**: a pointer to the configured `QShortcut`, owned by `parent` and destroyed together with it; or `nullptr` if `parent` is `nullptr`.


- `QAction *saveConfigToProject(QMenu *developmentMenu, const QString &projectFolder)`:

   Parameters:
   - **developmentMenu**: this is the root menu on the dialog where the new QAction will be attached
   - **projectFolder**: this is the absolute path of the project folder as it is captured from CMake. The
   simplest way to get this information in the code is to use a compiler variable called SOURCE_FILES_PATH
   using:
   
      `add_compile_definitions(SOURCE_FILES_PATH="${CMAKE_CURRENT_SOURCE_DIR}")`.   
      
      Then we can pass this variable as argument to saveConfigToProject.
   
      `saveConfigToProject(developmentMenu, SOURCE_FILES_PATH);`


- `QAction *initConfigFromResources(QMenu *developmentMenu, const QString &resFileOrPrefix)`:
   Parameters:
   - **developmentMenu**: this is the root menu on the dialog where the new QAction will be attached
   - **resFileOrPrefix**: this is the configuration file name as saved in the resources or just the 
   prefix, suche as :/resources since the fileName can be gathered via the appConfig global variable.





[⬆ Back to QtNoidApp](QtNoidApp.md)
[← Back to README](../../README.md)

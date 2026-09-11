# Class: Development
This is a collection of internal API that facilitate application development by automating common integration tasks. These APIs are for development use only and are not intended for customer-facing implementations.
The design prioritizes ease of integration through automation, though this approach may limit flexibility in certain scenarios.

## Static Methods

- `QShortcut *initFullDialogGrabShortcut(QWidget *parent, const QString &keySequence, QString destinationPath, bool saveToClipboard, bool openDestinationPath)`: 
  This is an helper method to simplify creating screenshots of an application, for documentation purpose. The method Initializes and returns a keyboard shortcut for capturing full dialog screenshot of current dialog. The lambda function to intercept the shortcut and save it to a file or to the clipboard is automatically created. When triggered, this shortcut captures a screenshot of the entire dialog window and saves it to the specified **destination path**. By default, the captured image ia copied to the system clipboard for immediate use, and the folder where it has been saved will be opened.
	* ***Parameters**:
		*  **parent**: the parent widget that will own the shortcut and destinationPath is empty it is used to evaluate the screenshot image file name.
		* **keySequence**: the keyboard combination that triggers the screenshot. Ff empty the default value **"Ctrl+Shift+S"** will be used. This is translated as "Cmd+Shift+S" on macOS.
		* **destinationFileOrPath**: it could be a file path for the resulting screenshot or a folder where the screenshot will be saved. If empty the screenshot will be saved in the application execution folder. If it doesn't contains a FileName, the image file name will be the application window title followed by -windows or by -macos depending on the operating system. If the **destinationPath** is empty then the screenshot will be saved in appExeOrAppBundleDirPath(). Multiple screenshot are managed using [QtNoid::Common::File::autoNaming()](../../QtNoidCommon/doc/QtnoidCommonFile.md).
		* **saveToClipboard**: If true, copies the captured image to clipboard, as an image, in addition to saving to the file.
		* **openDestinationPath**: if true opens the folder where the image has been saved to speed up the use for documentation.
	  * ***Returns**: 
		  * A pointer to the configured QShortcut object that handles the screenshot  functionality. The object is destroyed automatically when parent is destroyed.

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

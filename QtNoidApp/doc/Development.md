# Class: Development
This is a collection of internal API that facilitate application development by automating common integration tasks. These APIs are for development use only and are not intended for customer-facing implementations.
The design prioritizes ease of integration through automation, though this approach may limit flexibility in certain scenarios.

## Static Methods

- `QShortcut *initFullDialogGrabShortcut(QWidget *parent, const QString &keySequence, QString destinationPath, bool saveToClipboard)`: This is an helper method to simplify adding screenshot functionality to an application. Initializes and returns a keyboard shortcut for capturing full dialog screenshots, with the lambda to intercept the shortcut and save the screen to a file. When triggered, this shortcut captures a screenshot of the entire dialog window and saves it to the specified destination path. Optionally, the captured image can also be copied to the system clipboard for immediate use.
	* ***Parameters**:
		*  **parent**: the parent widget that will own the shortcut
		* **keySequence**: the keyboard combination that triggers the screenshot. Ff empty the default value "Ctrl+Shift+S" will be used. This is translated as "Cmd+Shift+S" on macOS.
		* **destinationFileOrPath**: it could be a file path for the resulting screenshot or a folder where the screenshot will be saved. If empty the screenshot will be saved in the application execution folder. If it doesn't contains a FileName, the image file name will be the application window title followed by -windows or by -macOS depending on the operating system. If the **destinationPath** is empty then the screenshot will be saved in appExeOrAppBundleDirPath(). Multiple screenshot are managed using [QtNoid::Common::File::autoNaming()](../../QtNoidCommon/doc/File.md).
		* **saveToClipboard**: If true, copies the captured image to clipboard, as an image, in addition to saving to the file.
	  * ***Returns**: 
		  * A pointer to the configured QShortcut object that handles the screenshot  functionality.

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

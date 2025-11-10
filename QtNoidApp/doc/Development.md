# Class: Development
This is a collection of API that should help the development of an application. They are
not for customers. 
They try to add a lot of automation to simplify the integration, sometimes at the expense
of the flexibility.

- `QShortcut *initFullDialogGrabShortcut(QWidget *parent, const QString &keySequence, QString destinationPath, bool saveToClipboard)`:
This is an helper method to simplify adding screenshot functionality to an application.
Initializes and returns a keyboard shortcut for capturing full dialog screenshots, with
the lambda to intercept the shortcut and save the screen to a file.
When triggered, this shortcut captures a screenshot of the entire dialog window and
saves it to the specified destination path. Optionally, the captured image can also be
copied to the system clipboard for immediate use.

  Parameters:
   - **parent**: the parent widget that will own the shortcut
   - **keySequence**: the keyboard combination that triggers the screenshot;
   if empty the default value "Ctrl+Shift+S" will be used. This is translated as
   "Cmd+Shift+S" on macOs.
   - **destinationFileOrPath**: it could be a file path for the resulting screenshot
   or a folder where the screenshot will be saved. If empty the screenshot will be
   saved in the application execution folder. If it doesn't contains a
   FileName, the image file name will be the application window title followed
   by -windows or by -macOs depending on the operating system.
   - **saveToClipboard**: If true, copies the captured image to clipboard, as an
   image, in addition to saving to the file.

  Returns: A pointer to the configured QShortcut object that handles the screenshot
  functionality.



&nbsp;

[⬆ Back to QtNoidApp](../QtNoidApp.md)

&nbsp;

[← Back to Main Page](../../README.md)

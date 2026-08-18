## Class: File
This class contains a set of helper methods to work with files
### Static Methods
- `QString File::autoNamingNextName(const QString &fileName)`: splits the file
  name in tokens, finds the last number block if present and add 1 while trying to replicate the numbering scheme. This means that 1 becomes 2, 001 becomes 002 and (1) becomes (2). The fileName can also be a full path. If there are no number blocks it adds ` 001` right before the first "." of the file name.

- `QString autoNaming(const QString &filePath)`: given the file path as a string, calculate the next free name in the folder preserving the numbering pattern or adding ` 001` right before the first "." of the file name.

- `QFileInfo autoNaming(const QFileInfo &fileInfo)`: starting from fileInfo, calculate the next free name in the folder preserving the numbering pattern or adding ` 001` right before the first "." of the file name.
  
- `bool compareIfEqual(const QString &filePath1, const QString &filePath2)`: 
  given two full file path returns true if the files are byte per byte equals.
  
- `bool compareIfEqual(const QFileInfo &fileInfo1, const QFileInfo &fileInfo2)`
  Compares 2 files and return true if they are equal byte per byte.

- `bool isTextFile(const QString &filePath)`: read the file and return true
  if it guess this is a text file. 
  
- `bool isTextFile(const QFileInfo &fileInfo)`: read the file pointed by
  fileInfo and returns true if it guess this is a text file. 

- `QString saveAsTextFile(const QString &data, const QString &filePath, const QString &basePath="", const QString &fileSuffix="*")`:
  Save data from a string to a text file as specified by filePath. The parameter filePath can be a fileName with or without suffix (i.e. txt). We can also specify a base folder force the file suffix fileSuffix.
  
- `QString saveAsTextFile(const QStringList &data, const QString &filePath, const QString &basePath="", const QString &fileSuffix="*")`
  Save a string list to a text file as specified by filePath. The parameter filePath can be a fileName with or without suffix (aka extension). We can also specify a base folder and ask the method to replace the file suffix.

- `QStringList readAsStringList(const QString &absoluteFilePath)`: reads a text file and return it as a string list.
  
- `QStringList listPathRecursively(const QString &path, const QStringList &nameFilters)`: returns a string list containing the **full path** of all files matching the given nameFilters, found recursively under the specified path. If path refers to a file, the method returns the file and all siblings files in the same directory. Filters can be a list of strings and validate the file name using endsWith() method.

- `QStringList listSubPathRecursively(const QString &path, const QStringList &nameFilters)`: returns a string list containing the **relative** path of all files found recursively under the specified path that match the given nameFilters. If path refers to a file, the method returns the file and all siblings files in the same directory. Filters can be a list of strings and validate the file name using endsWith() method.

  
### Non Static Methods
* None

⬆ [[QtNoidCommon]]

[← Back to README](../../README.md)



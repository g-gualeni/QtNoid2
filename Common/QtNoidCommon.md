# QtNoidCommon


## Header

```cpp
#include "QtNoidCommon/QtNoidCommon"
```

## Namespace

```cpp
namespace QtNoid::Common
```

## Class Scale
This class contains a set of helper methods to convert numbers to a human readable
text 

### Static Methods
 - `static QStringList scalePrefixList()`:
  - `static qint64 scaleMultiplier(const QString &prefix)`:
 - `static QString scaleMultiplierPrefix(qint64 scale)`:
 
 - `static QString scaleAutoUpToExaByte(qint64 value, 
    const QString& unitOfMeasure="Byte")`:    
 - `static QString scaleAutoUpToExa(qint64 value, const QString& unitOfMeasure)`:
 
 - `static QString scaleNanoSecsUpToDays(quint64 ns)`:
 - `static QString scaleNanoSecsUpToDays(qint64 ns)`:


## Class Text
This class contains a set of helper methods to cleanup, tokenize, convert 
to camelcase or snake case text strings.

### Static Methods

- `static bool isValidUrl(const QString &text)`: Returns true if text contains
    a valid URL.

- `static QStringList tokenizeSnakeCase(const QString &text)`: Split text into tokens
using the "_" character as separator.
- `static QStringList tokenizeSnakeCase(const QStringList &list)`: Split all text from list 
into tokens using the "_" character as separator a returning a single list of tokens.


- `static QStringList tokenizeCamelCase(const QString &txt)`: Split text into tokens
using the character capitalization change as a start for the new token
- `static QStringList tokenizeCamelCase(const QStringList &list)`: Split all text 
from the list into one list of tokens using the character capitalization change as 
a start for the new token.

- `static QStringList tokenizeNumberBlocks(const QString &txt, int minNumberBlockLen=2)`: 
Convert block numbers longer than minNumberBlockLen into tokens. The default length 
is 2 so QtNoid2 remains 1 token, while Thursday25 became {Thursday, 25}.
- `static QStringList tokenizeNumberBlocks(const QStringList &list, int minNumberBlockLen=2)`: 
For every text in the list it convert block numbers longer than minNumberBlockLen 
into tokens. The default length is 2 so QtNoid2 remains 1 token, while Thursday25 
became {Thursday, 25}.

- `static QStringList tokenize(const QString &text, const QString &splittersString, 
    bool splitCamelCase, int minNumBlockLen=2)`: Split the text into tokens. CamelCase
    and numeric blocks can be activated or disabled.

- `static QString convertToCamelCase(const QString &text)`: convert text into a 
camel_case string.

- `static QString convertToSnakeCase(const QString &text, int minNumBlockLen=2)`: 
convert text from whatEver_Case to snake case. Block numbers can be splitted from
text or not depending on minNumBlockLen.



[← Back to Main Page](./../README.md)


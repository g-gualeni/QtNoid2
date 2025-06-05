# QtNoidCommon


## Header

```cpp
#include "QtNoidCommon/QtNoidCommon"
```

## Namespace

```cpp
namespace QtNoid::Common
```

## Class File
This class contains a set of helper methods to work
with files

### Static Methods

### Non Static Methods


## Class Scale
This class contains a set of helper methods to convert numbers to 
a human readable text. 

### Static Methods
 - `static QStringList scalePrefixList()`: return a list of prefix 
    like "k","M","G","T","P","E".
    
 - `static qint64 scaleMultiplier(const QString &prefix)`: given 
    the standard prefix return the value of the multiplier.
 
 - `static QString scaleMultiplierPrefix(qint64 value)`: evaluate 
    the multiplier to be used with value.
 
 - `static QString scaleAutoUpToExaByte(qint64 value, 
    const QString& unitOfMeasure="Byte")`: convert the value into a
    string that has 2 decimal places and a scale prefix like k, M,
    G and so on. This static method consider 1 kByte as 1024 bytes.

 - `static QString scaleAutoUpToExa(qint64 value, const QString& 
    unitOfMeasure)`: converts the value into a string with 2 decimal
    places. The scale prefix is attached to the unitOfMeasure as in
    this example: 1.00 [kg]
 
 - `static QString scaleNanoSecsUpToDays(quint64 ns)`: convert ns 
    elapsed time to a more readable value using the best combinations
    of ms, seconds, minutes, hours, days. In this case ns is an 
    unsigned value.
    
 - `static QString scaleNanoSecsUpToDays(qint64 ns)`: convert ns 
    to a readable value using ms, seconds, minutes, hours, days. In
    this case ns is a signed value.


## Class Text
This class contains a set of helper methods to cleanup, tokenize, 
convert to camelcase or snake case text strings.


### Static Methods

- `static bool isValidUrl(const QString &text)`: Returns true if 
    the text contains a valid URL.
    
- `static bool isValidEmail(const QString &email)`: Returns true if
    the text contains a valid email address.

- `static QString sanitizeString(const QString &input)`: remove 
   double spaces or control characters from the input text. Some
   control characters like TAB or CR or LF are replaced with space.

- `static QStringList tokenizeSnakeCase(const QString &text)`: 
    Split text into tokens using the "_" character as separator.
    
- `static QStringList tokenizeSnakeCase(const QStringList &list)`:
    Split all text from list into tokens using the "_" character 
    as separator and returning a single list of tokens.

- `static QStringList tokenizeCamelCase(const QString &txt)`: 
    Split text into tokens using the character capitalization 
    change as a start for the new token.
    
- `static QStringList tokenizeCamelCase(const QStringList &list)`: Split all text 
    Split all strings in the list and return all tokens into one 
    list of tokens.

- `static QStringList tokenizeNumberBlocks(const QString &txt, 
    int minNumberBlockLen=2)`: Convert block numbers longer than 
    minNumberBlockLen into tokens. The default length is 2 so the
    cases like `QtNoid2` generate 1 token, while `Thursday25` 
    became 2 tokens {Thursday, 25}.
    
- `static QStringList tokenizeNumberBlocks(const QStringList &list,
    int minNumberBlockLen=2)`: For every text in the list it 
    converts block numbers longer than minNumberBlockLen into tokens. 
    The default length is 2 so processing `QtNoid2` generates 1 
    token, while `Thursday25` becames {Thursday, 25}.

- `static QStringList tokenize(const QString &text, const QString 
    &splittersString="_-. /\\", bool splitCamelCase=true, 
    int minNumBlockLen=2)`: Split the text into a list of tokens. 
    CamelCase and numeric blocks can be activated or disabled. 
    The default values should cover 90% of use cases for paths or 
    symbols.

- `static QString convertToCamelCase(const QString &text)`: 
    convert text into a `camel_case` string. This means spaces and 
    _ characters are removed and replaced with a change in character
    capitalization.

- `static QString convertToSnakeCase(const QString &text, 
    int minNumBlockLen=2)`: convert text from whatEver_Case to 
    snake case. numbers block longer than minNumBlockLen are 
    splitted from text. If minNumBlockLen is 0, number blocks
    are left untouched.



[← Back to Main Page](./../README.md)


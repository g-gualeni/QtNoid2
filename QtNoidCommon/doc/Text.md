## Class: Text
This class contains a set of helper methods to cleanup, tokenize, convert to camelCase or s_n_a_k_e case text strings.

### Static Methods

- `bool isValidUrl(const QString &text)`: Returns true if the text contains a valid URL.
    
- `bool isValidEmail(const QString &email)`: Returns true if the text contains a valid email address.

- `QString sanitizeString(const QString &input)`: remove double spaces or control characters from the input text. Control characters like TAB or CR or LF are replaced with space.

- `QStringList tokenizeSnakeCase(const QString &text)`: Split text into tokens using the _ character as separator.
    
- `QStringList tokenizeSnakeCase(const QStringList &list)`: Split all text from list into tokens using the _ character as separator and returning a single list of tokens.

- `QStringList tokenizeCamelCase(const QString &txt)`: Split text into tokens using the character capitalization change as a start for the new token.
    
- `QStringList tokenizeCamelCase(const QStringList &list)`: Split all strings in the list using the character capitalization change as a start for the new token and return all tokens into one list of tokens.

- `QStringList tokenizeNumberBlocks(const QString &txt, int minNumberBlockLen=2)`: 
    Scan the string and convert block numbers longer than minNumberBlockLen into tokens. The default length is 2 so the cases like `QtNoid2` generate 1 token, while `Thursday25` became 2 tokens {Thursday, 25}.
    
- `QStringList tokenizeNumberBlocks(const QStringList &list, int minNumberBlockLen=2)`: 
    For every string in the list it converts block numbers longer than minNumberBlockLen into tokens. The default length is 2 so processing `QtNoid2` generates 1 token, while `Thursday25` becomes {Thursday, 25}.

- `QStringList tokenize(const QString &text, const QString &splittersString="_-. /\\", bool splitCamelCase=true, int minNumBlockLen=2)`: 
    Split the text into a list of tokens. CamelCase and numeric blocks can be activated or disabled. The default values should cover 90% of use cases for paths or symbols.

- `QString convertToCamelCase(const QString &text)`: convert text into a `camelCase` string. This means spaces and _ characters are removed and replaced with a change in character capitalization.

- `QString convertToSnakeCase(const QString &text, int minNumBlockLen=2)`:  
    convert text from whatEver_Case to **snake_case**. Numbers block longer than minNumBlockLen are split from text. If minNumBlockLen is 0, number blocks are left untouched.

⬆ [[QtNoidCommon]]

[← Back to README](../../README.md)


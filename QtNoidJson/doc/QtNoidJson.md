# QtNoidJson
This Library contains:

- [**Yaml2Json**](QtNoidJsonYaml2Json.md): class that parses YAML text and converts it into a `QJsonObject`. The basic application is to help humans generate JSON files using YAML, which is more readable, less redundant and faster to write. The parsing is done by [rapidyaml](https://github.com/biojppm/rapidyaml) (MIT license), downloaded automatically by CMake and linked privately, and errors are reported through `isValid()`, `errorString()` and the `errorOccurred()` signal.

## CMake
```
find_package: QtNoidJson

target_link_libraries: QtNoid::QtNoidJson
```
## Header

```cpp
#include "QtNoidJson/QtNoidJson"
```

## Namespace

```cpp
using namespace QtNoid::Json;
```

## Examples Using this module

- **[JsonYamlToJsonTestSuite:](../../Examples/JsonYamlToJsonTestSuite/doc/JsonYamlToJsonTestSuite.md)** use this example to run the [**Yaml2Json**](QtNoidJsonYaml2Json.md) class against the official YAML test suite data sets and to compare the generated JSON with the expected one. Note: the tokens panel is no longer filled, since `tokens()` was removed with the new implementation.


[← Back to README](../../README.md)

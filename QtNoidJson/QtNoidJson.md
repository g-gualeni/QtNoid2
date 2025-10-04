# QtNoidJson

## Overview

This module contanis:
- [**Txt2Json**](#class-txt2json): that simplify the creation of JSON docs using
a simple plain text description.

This is an example with almost all combinations

## CMake
```
find_package: QtNoidJson

target_link_libraries: QtNoid::QtNoidJson
```

&nbsp;

## Header

```cpp
#include "QtNoidJson/QtNoidJson"
```

&nbsp;

## Namespace

```cpp
using namespace QtNoid::Json;
```

&nbsp;

## Examples


&nbsp;
## Class: Txt2Json

&nbsp;

&nbsp;



# YAML IS THE FUTURE

Latest version for YAML is 1.2.2 on Oct 1, 2021
https://github.com/yaml/yaml-spec/tags


Test Suite to validate the translator:
https://github.com/yaml/yaml-test-suite
-> To have a data set for test, checkout the latest branch data


This is the YAML test matrix
https://github.com/perlpunk/yaml-test-matrix

Notice the - YAML style to create an array of objects


```
person:
    name: "Mario Rossi"
    age: 30
    active: true
    email: mario.rossi@example.com
    address:
        city: Milano
        cap: 20100
        street: "Via Roma 123"
    hobbies: [calcio, cucina, lettura]
    tags: ["tag con spazi", semplice, altro]
    social:
        platforms: [facebook, twitter, linkedin, instagram]
        followers: [1500, 2300, 890, 4200]
    contacts:
        - name: "Luca Bianchi"
          phone: "333-1234567"
          email: luca@example.com
        - name: "Anna Verdi"
          phone: "347-7654321"
          email: anna@example.com

```


Caratteristiche chiave:
- ✅ Indentazione invece di graffe (opzionale)
- ✅ Stringhe senza virgolette (se senza spazi)
- ✅ Sintassi ibrida (graffe + indentazione)
- ✅ Array bracket [...] e lista con -
- ✅ Commenti // e #
- ✅ Conversione bidirezionale DSL ↔ JSON
- ✅ Gestione errori con linea e colonna


&nbsp;
## 3 Levels architecture

DSL Text → [LEXER] → Tokens → [PARSER] → AST → [GENERATOR] → JSON

DSL: 
AST: Abstract Syntax Tree
&nbsp;


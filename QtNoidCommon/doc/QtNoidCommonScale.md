## Class: QtNoid::Common::Scale
This class contains a set of helper methods to convert numbers to 
a human readable text. 

### Static Methods
 - `QStringList scalePrefixList()`: 
    Return the list of supported prefix like {"k", "M", "G", "T", "P", "E"}.
    
 - `qint64 scaleMultiplier(const QString &prefix)`: 
    given the standard prefix return the value of the multiplier.
 
 - `QString scaleMultiplierPrefix(qint64 value)`: 
    evaluate the multiplier to be used with value.
 
 - `QString scaleAutoUpToExaByte(qint64 value, const QString& unitOfMeasure="Byte")`: 
    Convert the value into a QString that has 2 decimal places and a scale prefix like k, M,
    G and so on. This static method consider 1 kByte as 1024 bytes.

 - `QString scaleAutoUpToExa(qint64 value, const QString& unitOfMeasure)`: 
    Converts the value into a string with 2 decimal places. The scale prefix is attached to the unitOfMeasure. So if unitOfMeasure is "g" the results is: 1.00 [kg]
 
 - `QString scaleNanoSecsUpToDays(quint64 ns)`: 
    Convert ns elapsed time as **unsigned value** to a more readable value using the best combinations of ms, seconds, minutes, hours, days. 
    
 - `static QString scaleNanoSecsUpToDays(qint64 ns)`: 
    Convert ns elapsed time as **signed value** to a readable value using ms, seconds, minutes, hours, days. 
     
⬆[[QtNoidCommon]]

[← Back to README](../../README.md)

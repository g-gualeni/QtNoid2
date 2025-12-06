## Class: Scale
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
    
⬆[[QtNoidCommon]]

&nbsp;
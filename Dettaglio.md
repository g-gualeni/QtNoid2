**Documentazione** — una delle due imprecisioni che avevo segnalato si è risolta da sola col fix (la frase “on success, the previous in-memory content is discarded” è ora corretta, perché `clear()` avviene davvero solo sul percorso di successo). Ne resta una aperta:

> `save()`: Returns false only if `isValid()` is false (no fileName).

Non è vero: `save()` ritorna `false` anche se `file.open(QIODevice::WriteOnly)` fallisce (permessi, percorso non scrivibile, ecc.). Proposta di correzione:

> `save()`: Returns false if `isValid()` is false (no fileName), or if the file can’t be opened for writing. Otherwise (re)writes the whole file with `toJsonValues()`, as `QJsonDocument::toJson()` produces it...

**Unico buco di copertura rimasto**: non esiste un test che verifichi esplicitamente che un `load()` fallito (es. JSON con nome diverso da quello già impostato) _non_ cancella il contenuto già in memoria — oggi è vero solo “per costruzione” grazie all’ordine delle operazioni, ma non c’è un test che lo blocchi se qualcuno in futuro invertisse di nuovo l’ordine.

Vuoi che corregga subito la frase su `save()` nel .md e che aggiunga quel test di regressione
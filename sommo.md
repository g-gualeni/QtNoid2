aaaa


Ho la mappa completa: `parameterlist.h`, `parameterlist.cpp`, `doc/QtNoidAppParameterList.md` e `tests/test_parameterlist.cpp`. Li carico tutti per l'analisi.

Letti 4 file, file cercati, ha usato uno strumento

Ho letto `parameterlist.h`, `parameterlist.cpp`, `doc/QtNoidAppParameterList.md` e i test. Ecco l'analisi.

### Bug: `removeParameter(const QString&)` lascia lo stato inconsistente quando il parametro è in posizione 0

Le due overload di `removeParameter` non sono coerenti tra loro. Quella per puntatore (parameterlist.cpp:361-385) fa:

cpp

```cpp
int idx = m_parameterToIndex.value(parameter, -1);
if(idx != -1) {
    m_parameterToIndex.remove(parameter);
    m_parametersByIndex.remove(idx);
}
```

Quella per nome (parameterlist.cpp:387-406) fa invece:

cpp

```cpp
int idx = m_parameterToIndex.value(parameter, -1);
if(idx > 0) {          // <-- dovrebbe essere idx != -1
    m_parameterToIndex.remove(parameter);
    m_parametersByIndex.remove(idx);
}
```

Dato che `m_nextParameterIndex` parte da 0, il **primissimo parametro aggiunto alla lista ha indice 0**. Se lo rimuovi per nome, `idx > 0` è falso, quindi `m_parameterToIndex` e `m_parametersByIndex` non vengono ripuliti: il parametro resta "mezzo rimosso". Conseguenze concrete:

- `list.count()` (riga 405, `m_count = m_parametersByIndex.count();`) non diminuisce, perché la mappa da cui viene calcolato non si è svuotata — quindi `count()` riporta un valore sbagliato (troppo alto).
- `list.contains(parameter)` continua a restituire `true` anche dopo la rimozione.
- `parameter(0)` continua a restituire il puntatore già "rimosso".
- Peggio: la `disconnect` da `QObject::destroyed` è già stata eseguita, quindi se quel `Parameter` viene distrutto in seguito, `onParameterDestroyed` non scatterà più per ripulire l'entry — lasciando un **puntatore pendente** in `m_parametersByIndex`/`m_parameterToIndex`, che esploderà alla prima iterazione (`parameters()`, `toJsonValues()`, `applyPreset()`, l'operatore `<<`...).

Il motivo per cui i test attuali non lo beccano: in `testRemovingParameters()` (test_parameterlist.cpp:431-455) il parametro in posizione 0 viene rimosso con l'overload a puntatore (`removeParameter(param1)`, corretta), e solo il secondo parametro — che si trova in posizione 1, non 0 — viene rimosso per nome. Stessa cosa nell'altro test a riga 1198-1209. Nessun test rimuove per nome il parametro che occupa la posizione 0.

Fix:

cpp

```cpp
void ParameterList::removeParameter(const QString &name)
{
    Parameter* parameter = m_parametersByName.value(name, nullptr);
    if(parameter == nullptr) {
        return;
    }

    m_parametersByName.remove(name);
    m_parametersByUniqueId.remove(parameter->uniqueId());
    int idx = m_parameterToIndex.value(parameter, -1);
    if(idx != -1) {
        m_parameterToIndex.remove(parameter);
        m_parametersByIndex.remove(idx);
    }
    ...
}
```

Test consigliato per non ricadere nello stesso buco: crea una lista con un solo parametro (che finirà quindi in posizione 0), rimuovilo per nome, e verifica `count() == 0` e `isEmpty() == true`.

### Thread-safety: `ParameterList::getNextUniqueId()` ha lo stesso problema già corretto in `Parameter`

cpp

```cpp
QAtomicInt ParameterList::getNextUniqueId()
{
    if (s_nextUniqueId == INT_MAX) {
        s_nextUniqueId = 0;  // Reset if overflow
    }
    return s_nextUniqueId++;
}
```

È esattamente il pattern "check-then-act" che avevamo identificato e risolto in `Parameter::getNextUniqueId()` con un `QMutex` — qui in `ParameterList` non è stato applicato lo stesso fix, e resta un `QAtomicInt` con un controllo e un incremento come due operazioni atomiche separate (non una transazione atomica unica). In pratica, arrivare a `INT_MAX` costruzioni di `ParameterList` è irrealistico, quindi il rischio pratico è bassissimo — ma se vuoi coerenza nel codice, converrebbe allineare questo metodo allo stesso pattern con `QMutex` usato in `Parameter`, o quantomeno lasciare un commento che spieghi perché qui si è scelto un approccio diverso. Non è bloccante, ma è un'incoerenza degna di nota visto che è la stessa classe di problema già affrontata altrove nel progetto.

### Documentazione: alcune cose implementate mancano dal file

- **`visible`** non compare da nessuna parte in `QtNoidAppParameterList.md`: manca dall'elenco proprietà introduttivo (righe 3-7) e mancano le voci `visible()`/`setVisible()`/`bindableVisible()` in "Properties management methods" — anche se è un `Q_PROPERTY` vero e proprio e ha buona copertura test (`testParameterListVisible`, `testBindableParameterListVisible`).
- Nella sezione **Signals** mancano `visibleChanged(bool value)` e `nameEdited(const QString&, const QString&)` — quest'ultimo è anche il segnale che alimenta `ParameterList::onParameterNameEdited`, quindi concettualmente importante da documentare visto il ruolo che gioca nel tenere sincronizzato `m_parametersByName`.
- `uniqueId()` (header riga 95, testato a righe 106-145 del test) non è documentato da nessuna parte — manca del tutto una sezione tipo "Support methods" come quella che invece esiste nel doc di `Parameter`.

Suggerirei di aggiungere una sezione "Support methods" con `uniqueId()`, aggiungere `visible` all'elenco proprietà e ai metodi, e le due voci mancanti nei Signals.

### Il resto

A parte questi punti, la classe è solida: la gestione degli indici tramite `QMap<int, Parameter*>` per l'ordinamento, `QHash` per accesso O(1) per nome/uniqueId, la sincronizzazione `onParameterNameEdited`/`onParameterDestroyed`, e gli iteratori STL-compatibili sono tutti implementati in modo pulito e ben testati. Il costruttore da JSON beneficia già del fix che abbiamo fatto su `Parameter` (l'ordine `initInternalConnections()` prima di `valueFromJson`/`schemaFromJson`), quindi non eredita quel problema.

Vuoi che ti scriva il diff completo per il fix di `removeParameter(const QString&)` e il test che lo verifica?


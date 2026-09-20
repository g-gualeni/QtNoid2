

Recupero i file di `Config` (header, implementazione, test e documentazione), controllando anche lo stato attuale del rename in corso.

Ho letto header, implementazione, test e documentazione di `Config`. È strutturalmente il "fratello maggiore" di `ParametersPage` (un container di pagine, come `ParametersPage` è un container di parametri), e infatti ci trovo alcuni bug della stessa famiglia di quelli risolti in `ParametersPage` — ma non ancora sistemati qui. Il più serio è piuttosto insidioso.

**Critico: il round-trip JSON perde i valori salvati dei parametri readOnly**

Nel costruttore `Config(schemaConfig, valueConfig, parent)` l'ordine delle operazioni è invertito rispetto a quello che avevamo corretto in `ParametersPage`: prima viene processato **tutto** l'array `pages` dello schema, creando ogni `ParametersPage` con `new ParametersPage(schemaObj, {}, this)` — quindi con lo schema già completamente applicato, incluso qualunque `readOnly: true` sui singoli parametri — e solo **dopo** viene processato l'array `pages` dei valori, chiamando `page->valuesFromJson(valueObj)`. Ma `ParametersPage::valuesFromJson()` scrive i valori tramite `Parameter::setValue()`, che è bloccato da `canModify()` se il parametro è già `readOnly`. Risultato: se ricarichi una `Config` salvata che contiene anche un solo parametro readOnly, il suo valore salvato viene silenziosamente scartato e resta quello di default/schema. Esattamente lo stesso bug-pattern risolto in `ParametersPage` (dove il costruttore fa apposta "values first, poi schema" — c'è perfino il commento `// Get Value first to avoid ReadOnly blocking the update`), ma qui a un livello più in alto non è stato applicato. Zero test in `test_app_config.cpp` menzionano `readOnly`, quindi il bug non è mai stato notato.

**Critico/fragile: `onPageDestroyed` usa `qobject_cast` in un punto dove non è affidabile**

cpp

```cpp
void Config::onPageDestroyed(QObject *obj)
{
    ParametersPage *page = qobject_cast<ParametersPage*>(obj);
    if(page == nullptr)
        return;
    ...
}
```

Questo è collegato a `QObject::destroyed()`, che scatta **dopo** che `~ParametersPage()` è già terminato. A quel punto il tipo dinamico dell'oggetto è "regredito" a `QObject` (la vtable/`metaObject()` non è più quella di `ParametersPage`), quindi `qobject_cast<ParametersPage*>` in questo contesto molto probabilmente ritorna `nullptr` sempre — il che significa che `onPageDestroyed` con ogni probabilità **non pulisce mai** le sue mappe interne quando una `ParametersPage` viene distrutta direttamente (es. il suo `QObject` padre viene eliminato altrove, o un `delete page;` esplicito) mentre è ancora registrata in un `Config`. Risultato: puntatori pendenti in tutte e quattro le mappe (`m_pageToIndex`, `m_pagesByIndex`, `m_pagesByUniqueId`, `m_pagesByName`), e la prossima `config.page(i)`/iterazione/`pages()` dereferenzia memoria non più valida. È lo stesso identico problema che abbiamo risolto per `Parameter`/`ParameterList` con il segnale `aboutToBeDestroyed` emesso dal distruttore — la soluzione qui è analoga: aggiungere lo stesso meccanismo a `ParametersPage` (se non c'è già un distruttore dedicato) e far sì che `Config` si agganci a quel segnale invece che a `QObject::destroyed()` + `qobject_cast`.

**Bug: `remove(ParametersPage*)` non verifica che la pagina appartenga davvero a questa `Config`**

cpp

```cpp
void Config::remove(ParametersPage *page)
{
    if (page == nullptr) return;
    m_pagesByUniqueId.remove(page->uniqueId());
    m_pagesByName.remove(page->name());   // <-- nessun controllo di appartenenza prima
    int idx = m_pageToIndex.value(page, -1);
    m_pageToIndex.remove(page);
    m_pagesByIndex.remove(idx);
    ...
    emit pageRemoved(page);   // <-- scatta comunque, anche per una pagina estranea
    ...
}
```

Se passi un puntatore a una pagina che non fa parte della lista, ma il cui `name()` coincide per caso con quello di una pagina realmente presente, `m_pagesByName.remove(page->name())` cancella silenziosamente la voce **sbagliata** dalla mappa nome→pagina, mentre `m_pagesByIndex`/`m_pageToIndex` restano intatti: la pagina vera resta contata e iterabile, ma `config.page("quelNome")`/`config.contains("quelNome")` iniziano a mentire. In più parte comunque un `pageRemoved` con il puntatore sbagliato. Il test esistente `testRemoveNonExistentPage()` non lo intercetta perché usa una pagina "estranea" con nome vuoto (default `ParametersPage(this)` senza nome), che non collide mai con "Page1"/"Page2" — quindi il test passa nonostante il buco. Il fix è lo stesso pattern già applicato in `ParameterList::removeParameter(Parameter*)`: aggiungere `if (!m_pagesByUniqueId.contains(page->uniqueId())) return;` prima di toccare le mappe.

**Bug: `disconnect()` punta al segnale sbagliato in `remove()` e `clear()`**

cpp

```cpp
disconnect(page, &ParametersPage::nameChanged, this, nullptr);
```

ma la connessione originale, fatta in `appendPageAndUpdateIndexs`, è su `&ParametersPage::nameEdited`, non `nameChanged`:

cpp

```cpp
connect(page, &ParametersPage::nameEdited, this, &Config::onPageNameEdited);
```

Quindi questo `disconnect` è un no-op silenzioso: la connessione vera non viene mai sciolta. Se una pagina viene rimossa da una `Config` ma resta viva altrove (tenuta da qualcun altro) e poi viene rinominata, `onPageNameEdited` continua a scattare su una `Config` a cui quella pagina non appartiene più, con rischio di ulteriori corruzioni di `m_pagesByName`. Va corretto in entrambi i punti (`remove()` riga 348 e `clear()` riga 377) sostituendo `nameChanged` con `nameEdited`.

**Documentazione**

`QtNoidAppConfig.md` è di fatto un abbozzo: solo il blocco `Q_PROPERTY` e i segnali incollati grezzi, senza nessuna delle sezioni (costruttori, metodi, iteratori, serializzazione) che invece Parameter/ParametersPage hanno. In più i segnali `pageAdded`/`pageRemoved` sono ancora documentati con il tipo vecchio `QtNoid::App::ParameterList` invece di `ParametersPage` — riflesso del rename non ancora propagato lì.

Da dove vuoi partire? Direi che il bug del round-trip JSON con `readOnly` è quello con l'impatto più concreto (dati utente persi silenziosamente), seguito dal fix di `onPageDestroyed`.
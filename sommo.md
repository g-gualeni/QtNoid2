Buona osservazione sul `label` — hai ragione, `Config` non ce l'ha mentre sia `Parameter` che `ParametersPage` sì. Quindi il pacchetto completo per l'allineamento è in realtà cinque pezzi. Te li elenco nell'ordine in cui ha senso costruirli (per dipendenze), poi ti propongo in dettaglio solo il primo:

1. **`label`** — property mancante, indipendente da tutto il resto.
2. **`uniqueId()`** — infrastruttura di base (mutex, contatore statico) che sia `Parameter` che `ParametersPage` hanno e `Config` no; serve come base per il punto 4.
3. **`isValueChanged`/`isValueChangedChanged`** — aggregazione dalle page contenute, stesso schema del contatore già usato da `ParametersPage` per i suoi `Parameter`. Nessun compromesso di design qui (segnale sincrono, non passa dal debounce).
4. **`aboutToBeDestroyed()` + distruttore** — ora può usare `uniqueId()` e il vero `isValueChanged` invece di un placeholder.
5. **`changed()`** — per ultimo, perché è l'unico con la domanda aperta sull'hop di debounce che non hai ancora deciso; ci torniamo quando arriviamo lì.
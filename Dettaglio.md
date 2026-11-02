er `openDestinationPath = true`, due strade: lo testo comunque accettando che apra una finestra durante il test (sconsigliato in CI), oppure proponiamo di estrarre il calcolo “quale cartella aprire” in un piccolo metodo privato testabile separatamente da `QDesktopServices::openUrl` — così si può verificare il path calcolato senza aprire davvero nulla. Dimmi tu quale preferisci.

White Box testing è meglio

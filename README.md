# 🛍️ DistributoreSwag

Benvenuto nella repository del **Distributore Swag**! Questo progetto si occupa della gestione di un distributore d'acqua, controllato tramite un ESP32 e un servo motore.

## ✅ To-Do List

- [x] Trovare il tappo necessario o individuare una soluzione alternativa *(es. utilizzo di una pompa, taglio diagonale di un tubo con regolazione del flusso tramite foglio di plastica trasparente, ...)*
- [x] Capire bene come funziona il modulo WiFi, inclusi HTTP e protocolli di comunicazione
- [ ] Capire come gestire gli interrupt con ESP32
- [x] Iniziare a progettare la realizzazione pratica del sistema
- [x] Dividerci il lavoro nella pratica

## ✏️ Schema
<img src="Schema.png" alt="Alt text" width="300">

## 📌 Cose Utili

Ecco alcuni link utili per il progetto:

- 🎥 **Collegamento tappo servo:** [Guarda il video](https://youtu.be/ywjtrN_QuFI) [Guarda il video](https://www.youtube.com/shorts/YKhZU1_dNQs) [Ricerca Amazon](https://www.amazon.it/s?k=tappo+erogatore+tanica&__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=3QHH02E7DG4O3&sprefix=tappo+erogatore+tanic%2Caps%2C174&ref=nb_sb_noss_2)
- 🔗 **Guida HTTP ↔ ESP32:** [Leggi l'articolo](https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/)
- ⚠️ **Come gestire gli interrupts:** [Leggi l'articolo](https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/)


## ⚙️ Workflow del DistributoreSwag

#### 1. Stato di Pronto
- Il sistema viene collegato e si entra nello stato di **pronto**.
- Vengono effettuati i controlli generali: verifica che i sensori non abbiano ostacoli e che la parte WiFi sia pronta.
- Il display mostrerà: **"PRONTO. COLLEGARSI AL WIFI SWAG"**.
- **STATUS_LED:** 🔵 (BLU).

#### 2. Connessione e Selezione Quantità
- L'utente si collega al WiFi del distributore tramite il telefono e scannerizza il QR per accedere alla pagina locale del distributore.
- Seleziona la quantità di acqua da erogare (**0.2L, 0.33L, 0.5L, 1L**).
- **STATUS_LED:** 🟣 (VIOLA) quando un dispositivo si connette.
- **STATUS_LED:** 🟡 (GIALLO) quando la scelta è stata effettuata.

#### 3. Controllo della Borraccia ed Erogazione
- Il sensore di prossimità verifica se la borraccia è posizionata correttamente.
- Se la borraccia è inserita e alla giusta distanza, inizia l'erogazione e **STATUS_LED sarà 🟢 (VERDE)**.
- Se la borraccia viene rimossa durante l'erogazione:
  - **STATUS_LED sarà 🟡 (GIALLO)**.
  - Il sistema si ricorderà il livello di erogazione e riprenderà quando la borraccia sarà di nuovo in posizione.

#### 4. Fine del Processo
- **Scenario 1:**
  - L'acqua è stata erogata correttamente.
  - Il display mostrerà **"FINITO"** e il buzzer suonerà.
  - Il sistema tornerà allo **stato 1**.
- **Scenario 2:**
  - L'utente preme il pulsante **STOP**.
  - Il sistema si interrompe immediatamente, indipendentemente dallo stato attuale.
  - Il display mostrerà **"RESET"** e **STATUS_LED sarà 🔴 (ROSSO) per qualche secondo**.
  - Il sistema tornerà allo **stato 1**.



## 📅 Recap 4/2/2025

Giulio e Jago si sono trovati per:
- Test del funzionamento del **LED RGB** *(ne abbiamo solo uno, quindi occhio a non bruciarlo!)*
- Valutazione dei sensori di prossimità: il **sensore a ultrasuoni** sembra molto più preciso di quello a infrarossi
- Stesura del **documento di funzionamento generale** (`workflow.md`, ma è lo stesso che c'è [qui](https://github.com/renna3/distributoreSwag/tree/main#%EF%B8%8F-workflow-del-distributoreswag)), cercando di ripercorrere tutto il ciclo operativo del dispenser in stile macchina a stati
- Ricerca di informazioni sul **tipo di tappo** da utilizzare *(vedi link nella pagina principale della repository GitHub)*

Si è discusso inoltre della **suddivisione del lavoro** basandoci sulle sezioni del documento di funzionamento generale.


## 📅 Recap 3/2/2025

Jago e Giulio si sono trovati per provare a vedere il funzionamento dei sensori vari (e mettere su GitHub i relativi codici pronti per essere usati), in particolare:

- **Display** (per mostrare la percentuale di riempimento)
- **Buzzer** (per emettere il suono di quando si finisce di riempire la borraccia)
- **Servo motore** (per tirare un filetto che aprirà la tanica —> dobbiamo ancora ben capire come gestire sta cosa, ma c’è un video nella pagina principale della nostra repository di un tipo che l’ha fatto)
- **Sensore di distanza ad infrarossi** (per vedere se la borraccia è sufficientemente vicina). C’è da capire se usare questo oppure quello a ultrasuoni (che abbiamo).

### 🛠️ Strumenti

Inoltre, si pensava di usare **PlatformIO** su VS Code per la programmazione delle ESP32 e **GitHub** per la condivisione e la collaborazione sul codice.

Usando Git e GitHub possiamo sfruttare l’estensione per VS Code che permette di caricare i cambiamenti direttamente su GitHub (da VS Code), senza fare 8000 giri inutili.

### 🚀 Prossimi Passi

Comunque, si pensava di farvi vedere dal vivo tutta sta roba (sia i sensori, che la configurazione di GitHub e PlatformIO su VS Code) così che possiamo effettivamente metterci mano tutti quanti partendo dalla stessa base.

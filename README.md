# 🛍️ DistributoreSwag

Benvenuto nella repository del **Distributore Swag**! Questo progetto si occupa della gestione di un distributore d'acqua, controllato tramite un ESP32 e un servo motore.

## ✅ To-Do List

- [ ] Trovare il tappo necessario o individuare una soluzione alternativa *(es. utilizzo di una pompa, taglio diagonale di un tubo con regolazione del flusso tramite foglio di plastica trasparente)*
- [ ] Capire come funziona bene il modulo WiFi, inclusi HTTP e protocolli di comunicazione
- [ ] Iniziare a progettare la realizzazione pratica del sistema
- [ ] Dividerci il lavoro nella pratica

## 📌 Cose Utili

Ecco alcuni link utili per il progetto:

- 🎥 **Collegamento tappo servo:** [Guarda il video](https://youtu.be/ywjtrN_QuFI) [Guarda il video](https://www.youtube.com/shorts/YKhZU1_dNQs) [Ricerca Amazon](https://www.amazon.it/s?k=tappo+erogatore+tanica&__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=3QHH02E7DG4O3&sprefix=tappo+erogatore+tanic%2Caps%2C174&ref=nb_sb_noss_2)
- 🔗 **Guida HTTP ↔ ESP32:** [Leggi l'articolo](https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/)
- ⚠️ **Come gestire gli interrupts:** [Leggi l'articolo](https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/)

## 📅 Recap 4/2/2025

Giulio e Jago si sono trovati per:
- Test del funzionamento del **LED RGB** *(ne abbiamo solo uno, quindi occhio a non bruciarlo!)*
- Valutazione dei sensori di prossimità: il **sensore a ultrasuoni** sembra molto più preciso di quello a infrarossi
- Stesura del **documento di funzionamento generale** (`workflow.txt`), cercando di ripercorrere tutto il ciclo operativo del dispenser in stile macchina a stati
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

# Progetto di Sistemi Operativi Dedicati (SOD)

## OBIETTIVI E SPECIFICHE
Il progetto consiste nello sviluppo di un sistema con i seguenti obiettivi:

• Acquisizione dei dati dal sensore di luminosità e dal sensore di movimento da parte dell'ESP32\
• Gestione della matrice led da parte dell'ESP32\
• Coordinamento dei task gestiti da FreeRTOS\
• Gestione della comunicazione tra ESP32 e Raspberry Pi tramite protocollo MQTT

## PROGETTAZIONE E SVILUPPO
### Ambiente di sviluppo

Per lo sviluppo del progetto è stato utilizzato l'editor [Visual Studio Code](https://code.visualstudio.com/) e l'estensione [PlatformIO](https://platformio.org/).<br>
L'estensione permette di avere un IDE per la gestione delle librerie e dei test di sviuluppo.

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/platform.png" width="550" > 
</p>


### Configurazione del Raspberry Pi
La comunicazione tra RPi e Esp32 avviene tramite protocollo MQTT. <br>
MQTT è un protocollo di messaggistica basato su standard, o un insieme di regole, utilizzato per la comunicazione tra macchine.
Funziona secondo i principi del modello [publish/subscribe](https://it.wikipedia.org/wiki/Publish/subscribe). Nella comunicazione di rete tradizionale, i client e i server comunicano direttamente tra loro. I client richiedono risorse o dati al server, che li elabora e invia una risposta. Tuttavia, MQTT utilizza uno schema publish/subscribe per disaccoppiare il mittente del messaggio (publisher) dal destinatario del messaggio (subscriber). Invece, un terzo componente, chiamato broker di messaggi, gestisce la comunicazione tra publisher e subscriber. Il compito del broker è quello di filtrare tutti i messaggi in arrivo da chi pubblica e distribuirli correttamente a chi si iscrive. 

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/mqtt-schema.png" width="550" > 
</p>

Per poter utilizzare il Rpi come broker è stato utilizzato il software open-source [Mosquitto](https://mosquitto.org/). <br>
La configurazione del Rpi è stata fatta attraverso i seguenti passaggi: <br>

• Installazione del broker Mosquitto
 ```
 sudo apt install -y mosquitto mosquitto-clients
 ```
• Abilitare l'avvio automatico del broker dopo l'avvio del Rpi
 ```
 sudo systemctl enable mosquitto.service
 ```
• Verificare la corretta installazione
 ```
 mosquitto -v
 ```
 
 Inoltre, per abilitare l'accesso remoto (senza autenticazione) è stata utilizzata la seguente procedura: <br>
 • Aprire il file mosquitto.conf
  ```
 sudo nano /etc/mosquitto/mosquitto.conf
 ```
• Alla fine del file aggiungere le due seguenti istruzioni
  ```
 listener 1883
 ```
```
allow_anonymous true
 ```
<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/mosquitto-conf.png" width="550" > 
</p>

• Effettuare un riavvio di Mosquitto
  ```
sudo systemctl restart mosquitto
 ```

#### Utilizzo del RPi

Per lo sviluppo delle funzionalità del RPi è stata utilizzata la libreria [paho-mqtt](https://pypi.org/project/paho-mqtt/). <br>
Nella Cartella **RPi** sono presenti due file principali: <br>

• *pub.py*: script con cui il RPi pubblica sul topic **data/sendImage** i colori dell'immagine da visualizzare sulla matrice led.<br>
• *sub.py*: script con cui il Rpi acquisisce il dato pubblicato dall'ESP32 sul topic **data/reqImage**.



### Acquisizione dei dati da parte dell'ESP32
Nel sistema realizzato l'ESP32 acquisisce i dati dal sensore di luminosità, dal sensore di movimento (PIR) e dal RPi. <br>
Il dato acquisito dal BH1750, che indica l'illuminamento rilevato, definisce l'intensità della luce nei led della matrice. Il valore acquisito dal PIR (0 o 1) viene utilizzato per accendere o spegnere la matrice. I dati ottenuti dal RPi sono i colori utilizzati per rappresentare l'immagine (pixel art) nella matrice led. <br>

#### Sensore di luminosità

Il sensore BH1750 rileva l'illuminamento e restituisce un valore di tipo *float*.<br>
In base al dato, il codice invierà una richiesta tramite il topic **data/reqImage**;ovvero verrà pubblicato il numero che identifica l'immagine desiderata (1,2 o 3) ed il Rpi, che sarà iscritto al topic, acquisirà il dato e pubblicherà sul topic **data/sendImage** una stringa contenente i colori dell'immagine nel formato *CRGB* (formato della libreria *FastLed*).

#### Sensore di movimento

Il sensore di movimento, collegato direttamente all'ESP32, rileva i movimenti e riporta il valore 1 se un movimento è stato rilevato, 0 altrimenti. <br>
Il valore acquisito indica se la matrice è accesa o spenta, oltre a consentire la lettura del valore del sensore BH1750.

### Gestione della matrice led

La matrice composta da 64 led, consente di visualizzare un'immagine con una luminosità dei led dettata dal sensore BH1750.<br>
Di seguito viene riportata un'immagine che rappresenta il funzionamento del sistema composto da ESP32 e RPi.

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/mqtt.png" width="550" > 
 </p>
 
### Coordinamento dei task con FreeRTOS

Il coordinamento delle diverse azioni che il programma deve svolgere viene svolto attraverso la libreria [FreeRTOS](https://www.freertos.org/index.html).<br>
Nello specifico, vengono utilizzate le funzioni:<br>

• *xTaskCreate* per la creazione dei task, in particolare possono essere specificati eventuali parametri e priorità da assegnare.<br>
• *vTaskDelete* per terminare un task ed eliminarlo.<br>
• *vTaskDelay* per tardare l'esecuzione del task.<br>

I task all'interno del codice vengono eseguiti con questo ordine: <br>

• Rilevazione del valore del PIR.<br>
• Rilevazione del valore del sensore di luminosità.<br>
• Attivazione della matrice con la corretta intensità dei led.<br>

<br>

|Task | Componenti | Priorità | 
|---|---|---|
| PIR_STATUS_TASK | ESP32, PIR |1|
| LIGHT_SENSOR_TASK | BH1750, ESP32 |2|
| LEDMATRIX_TASK | ESP32, WS2812B |3|

<br>

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/freertos.png" width="550" > 
 </p>





### Componenti hw
• Sensore di luminosità BH1750\
• Sensore di movimento HC-SR501\
• Matrice LED WS2812B\
• ESP32\
• Raspberry Pi 3B+




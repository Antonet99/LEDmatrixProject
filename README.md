# Progetto di Sistemi Operativi Dedicati (SOD)

## OBIETTIVI E SPECIFICHE
Il progetto consiste nello sviluppo di un sistema con i seguenti obiettivi:\
• Acquisizione dei dati dal sensore di luminosità e dal sensore di movimento da parte dell'ESP32\
• Gestione della matrice led da parte dell'ESP32\
• Coordinamento dei task gestiti da FreeRTOS\
• Gestione della comunicazione tra ESP32 e Raspberry Pi tramite protocollo MQTT

## PROGETTAZIONE E SVILUPPO
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



### Acquisizione dei dati da parte dell'ESP32
#### Sensore di luminosità
#### Sensore di movimento
### Gestione della matrice led
### Coordinamento dei task con FreeRTOS

|Task | Componenti | Priorità | 
|---|---|---|
| PIR_STATUS_TASK | ESP32, PIR |1|
| LIGHT_SENSOR_TASK | BH1750, ESP32 |2|
| LEDMATRIX_TASK | ESP32, WS2812B |3|

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/freertos.png" width="550" > 
 </p>

### Comunicazione tramite MQTT

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/mqtt.png" width="550" > 
 </p>

### Componenti hw
• Sensore di luminosità BH1750\
• Sensore di movimento HC-SR501\
• Matrice LED WS2812B\
• ESP32\
• Raspberry Pi 3B+




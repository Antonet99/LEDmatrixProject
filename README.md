# Progetto di Sistemi Operativi Dedicati (SOD)

## OBIETTIVI E SPECIFICHE
Il progetto consiste nello sviluppo di un sistema con i seguenti obiettivi:\
• Acquisizione dei dati dal sensore di luminosità e dal sensore di movimento da parte dell'ESP32\
• Gestione della matrice led da parte dell'ESP32\
• Coordinamento dei task gestiti da FreeRTOS\
• Gestione della comunicazione tra ESP32 e Raspberry Pi tramite protocollo MQTT

## PROGETTAZIONE E SVILUPPO
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
• Sensore di luminosità BH1750
• Sensore di movimento HC-SR501
• Matrice LED WS2812B
• ESP32
• Raspberry Pi 3B+




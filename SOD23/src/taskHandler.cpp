#include "headers/taskHandler.hpp"
#include "headers/ledMatrixHandler.hpp"


/**
 * Inizializzatione della variabile BH1750
 */
void lightSensorStart()
{
    lightMeter.begin();
}
/**
 * Logica del Task freeRTOS che gestisce il sensore di luminosità.
 * @param pir_status valore che indica se il sensore di movimento ha rilevato movimento (1) oppure no (0)
 */

void lightSensorTask(void *parameter)
{
    unsigned int pir_status = *(unsigned int *)parameter;
    if (pir_status)
    {
        float lux = (int)lightMeter.readLightLevel(); // leggi il valore dal sensore di luminosità
        String message = "Luminosità rilevata: " + String(lux);
        Serial.println(message);
    }
    else
        Serial.println("Sensore di luminosità non attivo, nessun movimento rilevato dal PIR");

    vTaskDelete(NULL);
}
/**
 * Logica del Task freeRTOS per ON/OFF della matrice led.
 * La matrice si accende se il PIR rileva un movimento.
 * @param parameter valore del PIR,1 se il sensore rileva movimento e 0 altrimenti
 */
void ledMatrixTask(void *parameter)
{
    unsigned int pir_status = *(unsigned int *)parameter;

    if (pir_status)
    {
        setColors();
        
    }

    vTaskDelete(NULL);
}

void getTasks(unsigned int pir_status){
 xTaskCreate(lightSensorTask,"MOTION_TASK",10000,(void*)&pir_status,1,NULL);
 xTaskCreate(ledMatrixTask,"MOTION_TASK",10000,(void*)&pir_status,2,NULL);
}

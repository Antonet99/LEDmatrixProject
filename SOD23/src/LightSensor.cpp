#include "headers/lightSensor.hpp"
#include "headers/constLightSensor.hpp"
/**
 * Inizializzatione della variabile BH1750
*/
void lightSensorStart(){
    lightMeter.begin();
}
/**
 * Logica del Task freeRTOS che gestisce il sensore di luminosità.
 * @param pir_status valore che indica se il sensore di movimento ha rilevato movimento (1) oppure no (0)
*/

void lightSensorTask(unsigned int pir_status){

 if(pir_status){
    float lux = (int) lightMeter.readLightLevel(); // leggi il valore dal sensore di luminosità
    Serial.println(lux);
 }
 else Serial.println("Sensore di luminosità non attivo, nessun movimento del PIR rilevato");
 

}
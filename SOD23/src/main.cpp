#include <iostream>
#include <Wire.h>
#include <string.h>
#include "headers/connections.hpp"
#include "headers/lightSensor.hpp"
#include "headers/movSensor.hpp"
#include "headers/matrixHandler.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
using namespace std;

CRGB colors[NUM_LEDS];

/**
 * Funzione di callback utilizzata per verificare la corretta ricezione dei dati 
 * ed il payload con le informazioni.
 * @param topic topic dal quale vengono ricevuti i dati (topic su cui è stato effettuato il subscribe)
 * @param payload dati effettivi ricevuti dal topic
 * @param lenght lunghezza del messaggio.
*/
void callback(char *topic, byte *payload, unsigned int length)
{
  char* message = new char[length];
  Serial.print("Messaggio ricevuto dal topic :");
  Serial.print(topic);
  for (int i = 0; i < length; i++)
  {
    
  }
  Serial.println();

}


void setup()
{
    wifiConn();
    mqttConn();
    Wire.begin();
    lightSensorStart();
    movSensorStart();
}

void loop()
{
}

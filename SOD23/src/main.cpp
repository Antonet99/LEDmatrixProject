#include <iostream>
#include <Wire.h>
#include <string.h>
#include "headers/taskHandler.hpp"
using namespace std;
int PIR_data = 19;


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
  delay(1000);
  Serial.begin(9600);
    //wifiConn();
    //mqttConn();

    //lightSensorStart();
     pinMode(PIR_data, INPUT);

    setMatrixConfig();
    Wire.begin();
    
  
   
}

void loop()
{
   bool pir_status = digitalRead(PIR_data);
    pir_status ? Serial.println("MOVIMENTO RILEVATO") : Serial.println("NESSUN MOVIMENTO RILEVATO...");
    unsigned int pir_value = (int) pir_status;
    getTasks(pir_value);
    delay(2000);

}

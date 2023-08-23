
#include "headers/taskHandler.hpp"


using namespace std;

int PIR_data = 19;


/**
 * 
*/

void setup()
{
  
    Serial.begin(9600);
  // connessione Wifi e MQTT
    wifiConn();
    mqttConn();
    pinMode(PIR_data, INPUT);
  
  // configurazione di base della matrice led
    initPubSub();
    setMatrixConfig();
    imageRequestTask();
    
    Wire.begin();
   
    
  
   
}

void loop()
{
  clientLoop();
  


   bool pir_status = digitalRead(PIR_data);
    //pir_status ? Serial.println("MOVIMENTO RILEVATO") : Serial.println("NESSUN MOVIMENTO RILEVATO...");
   // unsigned int pir_value = (int) pir_status;
    //getTasks(pir_value);
    

}

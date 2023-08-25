
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
    //imageRequestTask();
    setMatrixConfig();
   imageRequestTask();
  
    
    Wire.begin();
    
  
    
    //getTasks(pir_value);
    
  
   
}

void loop()
{
 
  clientLoop();
 
  


  
    

}

#include "headers/taskHandler.hpp"

using namespace std;



// Pin per il sensore di movimento
int PIR_data = 19;

// Funzione di inizializzazione
void setup()
{
  // Inizializzazione della comunicazione seriale
  Serial.begin(9600);

  // Connessione WiFi e MQTT
  wifiConn();
  mqttConn();

  // Impostazione della modalità del pin del sensore di movimento
  pinMode(PIR_data, INPUT);

  // Inizializzazione del client MQTT
  initPubSub();

  // Configurazione della matrice LED
  setMatrixConfig();
  
  // Avvio del task per richiedere l'immagine
  
  //publishImage();
  //imageRequest();
    imageRequest();

  // Inizializzazione del bus I2C
  Wire.begin();
}

void loop()
{

   //imageRequest();


   clientLoop();
  // Esegue il loop del client MQTT

 
  
}

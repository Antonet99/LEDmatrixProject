#include <iostream>
#include <Wire.h>
#include <string.h>
#include "headers/taskHandler.hpp"


using namespace std;

int PIR_data = 19;



const char *ssid = "Vodafone-A48487438";    
const char *wifi_password = "psLLfEEMA4AdGhCX";

const char* topic_images= "rpi/images";
const char* topic_req="data/reqImage";

const char *mqtt_username = "sod";                // my mqtt username
const char *mqtt_password = "sod23";                // my mqtt password
const char *mqtt_clientID = "esp32_sod";            // my clientID
const char *mqtt_server = "192.168.1.21";          // my mqtt server address
unsigned int mqtt_port = 1883;  

WiFiClient askClient;
PubSubClient client(mqtt_server, mqtt_port, askClient);

/**
 * Effettua connessione al WiFi.
*/
void wifiConn()
{
  
  Serial.print("********** Connessione al WiFi in corso :");
  Serial.println(ssid);

  WiFi.begin(ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("->Connessione al WiFi effettuata");
  Serial.println("-> indirizzo IP: ");
  Serial.println(WiFi.localIP());
}


/**
 * Effettua connessione dell' ESP32 al server MQTT
*/
void mqttConn()
{
  Serial.print("**** Connesso al server MQTT : ");
  Serial.println(mqtt_server);

  client.setServer(mqtt_server, mqtt_port);
  reconnect();

  
}

/**
 * Tentativo di riconnessione al server MQTT in caso di insuccesso.
*/
void reconnect()
{
  // ciclo con il quale si tenta la riconnessione
  while (!client.connected())
  {
    Serial.print("********** Tentativo di connessione MQTT...");
    if (client.connect(mqtt_clientID, mqtt_username, mqtt_password))
    {
      Serial.println("-> client MQTT connesso");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("-> nuovo tentativo fra 5 secondi");
      delay(5000);
    }
  }
}

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
  Serial.print("Messaggio ricevuto dal topic : ");
  Serial.print(topic);
  Serial.print('\n');
  String s = "";
  for (int i=0;i<length;i++) {
    
   //Serial.print((char)payload[i]);
    s = s + String ((char) payload[i]);
  }
  Serial.print(s);
  Serial.print('\n');
  parsePayload(s);

  //if(topic == topic_images) Serial.println((char*) payload);
  //else Serial.println("NESSUN SUBSCRIBE");
   
}

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
    //setMatrixConfig();

    client.setServer(mqtt_server, mqtt_port);
    client.connect(mqtt_clientID, mqtt_username, mqtt_password);
   
    client.setCallback(callback);
    client.publish(topic_req,"immagine 1");
   client.subscribe(topic_images);

    Wire.begin();
    
  
   
}

void loop()
{
  client.loop();
  


   bool pir_status = digitalRead(PIR_data);
    //pir_status ? Serial.println("MOVIMENTO RILEVATO") : Serial.println("NESSUN MOVIMENTO RILEVATO...");
    unsigned int pir_value = (int) pir_status;
    //getTasks(pir_value);
    delay(8000);

}

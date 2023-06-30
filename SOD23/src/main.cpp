#include <iostream>
#include <WiFi.h>
#include <PubSubClient.h>
#include <BH1750.h>
#include <Wire.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string>
using namespace std;

 const char *ssid = "TP-LINK_C930";     // Wifi SSID
 const char *wifi_password = "88099774";
// MQTT config
const char *mqtt_username = "sod23";                  // my mqtt username
const char *mqtt_password = "sod23";                  // my mqtt password
const char *mqtt_pubTopic = "publish/esp32/motion";   // publish/username/apiKeyIn
const char* mqtt_clientID = "esp32_sod";              // MQTT client ID
const unsigned int writeInterval = 10000; 
int pinStateCurrent   = LOW;  // current state of pin
int pinStatePrevious  = LOW;  // previous state of pin            // write interval (in ms)

// MQTT config
const char *mqtt_server = "192.168.0.105";
unsigned int mqtt_port = 1883;
int PIR_data = 19; // probabilmente da cambiare su 19

WiFiClient askClient;
PubSubClient client(mqtt_server, mqtt_port, askClient);
BH1750 lightMeter;
//funzione di input per task sensore di movimento
void motion_task(void * parameter){
 unsigned int pir_status = *(unsigned int*) parameter;
  if (pir_status)
  {
    //Serial.println("PIR motion detected");
    Serial.println("********** Publish MQTT data ( motion sensor ) \n");
    char mqtt_payload1[30] = "";
    snprintf(mqtt_payload1, 30, "m1=%ld", pir_status);
    client.publish(mqtt_pubTopic, mqtt_payload1);
    Serial.println("> MQTT data ( motion sensor ) published \n");

  }

  vTaskDelete(NULL);

}
// funzione di input per  task sensore di luminosità
void light_intensity_task(void * parameter){
   unsigned int pir_status = *(unsigned int*) parameter;
  if(pir_status){
     float lux = (int) lightMeter.readLightLevel();
     Serial.println("********** Publish MQTT data ( light sensor ) \n ");
     Serial.print('\n');
     char mqtt_payload2[30] = "";
     snprintf(mqtt_payload2, 30, "lux=%f", lux);
     client.publish(mqtt_pubTopic, mqtt_payload2);
     Serial.println(" > MQTT data ( light sensor ) published \n ");
     String s = "Valore in lux : " + (String) lux;
    
     Serial.println(s);
     Serial.print('\n');
    

  }
  else {Serial.print("Movimento non rilevato, sensore di luminosità non attivo");}

   Serial.println("********** End ");
     Serial.println("*****************************************************");

  vTaskDelete(NULL);
}


void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("********** Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_clientID, mqtt_username, mqtt_password))
    {
      Serial.println("-> MQTT client connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("-> try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup()
{
  Serial.begin(9600);
  Serial.println("*****************************************************");
  Serial.println("********** Program Start : ESP32 publishes PIR Motion data over MQTT");
 Serial.print('\n');
  
  Serial.print("********** connecting to WIFI : ");
  Serial.println(ssid);

  WiFi.begin(ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("->WiFi connected");
  Serial.println("->IP address: ");
  Serial.println(WiFi.localIP());


  client.setServer(mqtt_server, mqtt_port);
  client.connect(mqtt_clientID, mqtt_username, mqtt_password);
  client.setCallback(callback);

  pinMode(PIR_data, INPUT);

 

  Wire.begin();
  lightMeter.begin();
  //Serial.println(F("BH1750 Test begin")); // pir data as input
}

void loop()
{
  //verifica connesione MQTT
  if (!client.connected())
    reconnect();

  client.loop();

  bool PIR_status = digitalRead(PIR_data);
  unsigned int temp=0;

  if (PIR_status)
  {
    Serial.print('\n');
    Serial.println("PIR STATUS : Motion detected");
    Serial.print('\n');
    temp = 1 ;
  }
  else
  {
    Serial.println("NO motion detected");
    Serial.print('\n');
  }
 


 xTaskCreate(motion_task,"MOTION_TASK",10000,(void*)&temp,1,NULL);
 xTaskCreate(light_intensity_task,"MOTION_TASK",10000,(void*)&temp,2,NULL);

  /*
  Serial.print(" - Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  */
 
  delay(writeInterval); // delay
}






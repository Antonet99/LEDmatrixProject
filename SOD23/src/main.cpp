/*
 * MQTT and AskSensors IoT Platform
 * Description: ESP32 publishes PIR motion data to AskSensors over MQTT
 *  Author: https://asksensors.com, 2020
 *  github: https://github.com/asksensors
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <BH1750.h>
#include <string>
using namespace std;
// TODO: ESP32 MQTT user config
const char *ssid = "iliadbox-5F89A1_2.4Ghz";     // Wifi SSID
const char *wifi_password = "bt55mqf773q32995qn3b73"; // Wifi Password

// MQTT config
const char *mqtt_username = "sod23";                  // my mqtt username
const char *mqtt_password = "sod23";                  // my mqtt password
const char *mqtt_pubTopic = "publish/esp32/motion";   // publish/username/apiKeyIn
const char* mqtt_clientID = "esp32_sod";              // MQTT client ID
const unsigned int writeInterval = 25000;             // write interval (in ms)

// MQTT config
const char *mqtt_server = "192.168.1.49";
unsigned int mqtt_port = 1883;
int PIR_data = 19; // probabilmente da cambiare su 19

WiFiClient askClient;
PubSubClient client(mqtt_server, mqtt_port, askClient);
BH1750 lightMeter;
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
  //client.connect(mqtt_clientID, mqtt_username, mqtt_password)
  client.setCallback(callback);

  pinMode(PIR_data, INPUT);

  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin")); // pir data as input
}

void loop()
{

  if (!client.connected())
    reconnect();
  client.loop();

  bool PIR_status = digitalRead(PIR_data);

  if (PIR_status)
  {
    Serial.println("PIR motion detected");
  }
  else
  {
    Serial.println("NO PIR motion detected");
  }

  float lux = (int) lightMeter.readLightLevel();
  
  Serial.print(" - Light: ");
  Serial.print(lux);
  Serial.println(" lx");
 

  Serial.println("********** Publish MQTT data");
  char mqtt_payload1[30] = "";
  snprintf(mqtt_payload1, 30, "m1=%ld", PIR_status);
  char mqtt_payload2[30] = "";
  snprintf(mqtt_payload2, 30, "lux=%f", lux);
  Serial.print("Publish message: ");
  Serial.println(mqtt_payload1);
  Serial.println(mqtt_payload2);
  client.publish(mqtt_pubTopic, mqtt_payload1);
  client.publish(mqtt_pubTopic, mqtt_payload2);
  Serial.println("> MQTT data published");
  Serial.println("********** End ");
  Serial.println("*****************************************************");

  delay(writeInterval); // delay
}


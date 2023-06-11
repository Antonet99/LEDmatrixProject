/*
 * MQTT and AskSensors IoT Platform
 * Description: ESP32 publishes PIR motion data to AskSensors over MQTT
 *  Author: https://asksensors.com, 2020
 *  github: https://github.com/asksensors
 */

#include <WiFi.h>
#include <PubSubClient.h>

// TODO: ESP32 MQTT user config
const char *ssid = "iliadbox-5F89A1_2.4Ghz";     // Wifi SSID
const char *password = "bt55mqf773q32995qn3b73"; // Wifi Password
const char *username = "sod23";                  // my mqtt username
const char *pubTopic = "publish/esp32/motion";   // publish/username/apiKeyIn
const unsigned int writeInterval = 25000;        // write interval (in ms)

// MQTT config
const char *mqtt_server = "192.168.1.149";
unsigned int mqtt_port = 1883;
int PIR_data = 19; // probabilmente da cambiare su 19

WiFiClient askClient;
PubSubClient client(askClient);

void setup()
{
  Serial.begin(9600);
  Serial.println("*****************************************************");
  Serial.println("********** Program Start : ESP32 publishes PIR Motion data over MQTT");
  Serial.print("********** connecting to WIFI : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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
  client.setCallback(callback);

  pinMode(PIR_data, INPUT); // pir data as input
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

  Serial.println("********** Publish MQTT data");
  char mqtt_payload[30] = "";
  snprintf(mqtt_payload, 30, "m1=%ld", PIR_status);
  Serial.print("Publish message: ");
  Serial.println(mqtt_payload);
  client.publish(pubTopic, mqtt_payload);
  Serial.println("> MQTT data published");
  Serial.println("********** End ");
  Serial.println("*****************************************************");

  delay(writeInterval); // delay
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
    if (client.connect("ESP32Client", username, ""))
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

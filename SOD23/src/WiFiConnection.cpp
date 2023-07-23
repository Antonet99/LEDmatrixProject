
#include "headers/constants.hpp"
#include "headers/wifiConnection.hpp"
#include "headers/constMqtt.hpp"

void wifiConn()
{
  Serial.begin(9600);
  Serial.print("********** connecting to WIFI :");
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
}
void mqttConn()
{
  Serial.print("********** connecting to MQTT : ");
  Serial.println(mqtt_server);

  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected())
  {
    Serial.println("********** connecting to MQTT...");
    if (client.connect(mqtt_clientID, mqtt_username, mqtt_password))
    {
      Serial.println("ESP32 connected to MQTT server");
    }
    else
    {
      Serial.print("Connection failed with state ");
      Serial.print(client.state());
      delay(500);
    }
  }
}

#include "headers/constants.hpp"
#include "headers/wifiConnection.hpp"


void wifiConn(){
  Serial.begin(9600);
  Serial.print("********** connecting to WIFI : ");
  Serial.println(ssid);

  WiFi.begin(ssid,wifi_password);
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
void mqttConn(){
  
}
#include <iostream>
#include <WiFi.h>
using namespace std;




class WiFiConnection
{
    public:
    void checkWiFi(const char*ssid,const char*password)
    {
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
    }
};
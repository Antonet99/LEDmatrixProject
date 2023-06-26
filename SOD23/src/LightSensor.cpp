#include <iostream>
#include <BH1750.h>
#include <PubSubClient.h>
using namespace std;


class LightSensor{
   
    private: 
    int i = 0;
    public:
     LightSensor(){}
     ~LightSensor(){
         cout << "My Data is destroyed\n";  
     }
    void getSensorData(BH1750 lightSensor, const char* mqtt_pubTopic,PubSubClient client)
    {

        Serial.println("********** Publish MQTT data light sensor");
        float lux = (int) lightSensor.readLightLevel();
        char mqtt_payload2[30] = "";
        snprintf(mqtt_payload2, 30, "lux (intensità luce) = %f", lux);
        Serial.println(mqtt_payload2);
        client.publish(mqtt_pubTopic, mqtt_payload2);

        Serial.println("> MQTT data light sensor published");
        Serial.println("********** End ");
        Serial.println("*****************************************************");
        



    }

};


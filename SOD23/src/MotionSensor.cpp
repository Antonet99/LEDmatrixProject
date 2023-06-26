#include <PubSubClient.h>

class MotionSensor{
    public:
    void getSensorData(bool pir_status,const char* mqtt_pubTopic,PubSubClient client){
 Serial.println("********** Publish MQTT motion sensor data");
  char mqtt_payload1[30] = "";
  snprintf(mqtt_payload1, 30, "m1=%ld", pir_status);
Serial.print("Publish message (motion sensor) : ");
  Serial.println(mqtt_payload1);
  client.publish(mqtt_pubTopic, mqtt_payload1);

    }
};
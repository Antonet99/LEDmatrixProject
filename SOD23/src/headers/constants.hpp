#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <PubSubClient.h>
#include <WiFi.h>
#include "headers/constMqtt.hpp"

const char *ssid = "iPhone";    
const char *wifi_password = "alex12345";

WiFiClient askClient;
PubSubClient client(mqtt_server, mqtt_port, askClient);

#endif
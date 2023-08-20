#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <PubSubClient.h>
#include <WiFi.h>
#include "headers/constants/constMqtt.hpp"

const char *ssid = "Vodafone-A48487438";    
const char *wifi_password = "psLLfEEMA4AdGhCX";

WiFiClient askClient;
PubSubClient client(mqtt_server, mqtt_port, askClient);

#endif
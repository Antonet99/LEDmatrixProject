#include <iostream>
#include <Wire.h>
#include "headers/connections.hpp"
#include "headers/lightSensor.hpp"
#include "headers/movSensor.hpp"
#include "headers/matrixHandler.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
using namespace std;

void setup()
{
    wifiConn();
    mqttConn();
    Wire.begin();
    lightSensorStart();
    movSensorStart();
}

void loop()
{
}

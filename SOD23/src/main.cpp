#include <iostream>
#include <Arduino.h>
#include <Wire.h>
#include "headers/wifiConnection.hpp"
#include "headers/lightSensor.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
using namespace std;

void setup()
{
 wifiConn();
 Wire.begin();
 lightSensorStart();


}

void loop()
{
 
   
}






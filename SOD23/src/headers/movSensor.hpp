#ifndef MOVSENSOR.HPP
#define MOVSENSOR.HPP
#include "headers/constants/constMovSensor.hpp"
#include <Arduino.h>

void movSensorStart(){
    Serial.begin(9600);
    pinMode(PIR_data, INPUT);
}
void motionSensorTask(int pir_status){
if (pir_status)
  {
    //TODO se il sensore è accesso (pir_status = 1) accendi la matrice 
 
  }
}

#endif
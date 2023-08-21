#ifndef TASKHANDLER_HPP
#define TASKHANDLER_HPP
#include "headers/constants/constTasks.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// void lightSensorStart();
// void lightSensorTask(void *parameter);
void getTasks(unsigned int pir_status);
void ledMatrixTask(void *parameter);

uint8_t XY(uint8_t x, uint8_t y);
void setColors(int pir_status);
void setMatrixConfig();
uint8_t setBrightness(float lux);
float getLux();
void setPinPir();

#endif

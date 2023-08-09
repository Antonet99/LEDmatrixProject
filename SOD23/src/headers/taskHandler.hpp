#ifndef TASKHANDLER_HPP
#define TASKHANDLER_HPP
#include "headers/constants/constTasks.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void lightSensorStart();
void lightSensorTask(void *parameter);
void getTasks(unsigned int pir_status);
void ledMatrixTask(void *parameter);


#endif

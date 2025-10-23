#ifndef APDS9960_SIMPLE_H
#define APDS9960_SIMPLE_H

#include "SimpleI2C.h"
#include <Arduino.h>

#define APDS9960_ADDR 0x39

typedef struct
{
    // Сырые данные
    uint16_t red, green, blue, clear;
    uint8_t proximity;

    // Простая фильтрация - усреднение
    float filteredRed, filteredGreen, filteredBlue, filteredClear;
    float filteredProximity;

    // Буферы для простого усреднения
    uint16_t redBuffer[3], greenBuffer[3], blueBuffer[3], clearBuffer[3];
    uint8_t proximityBuffer[5];
    uint8_t bufferIndex;

    bool isInitialized;

} APDS9960_Sensor;

bool APDS9960_SimpleInit(APDS9960_Sensor *sensor);
void APDS9960_SimpleUpdate(APDS9960_Sensor *sensor);

#endif
#ifndef APDS9960_INTEGRATED_H
#define APDS9960_INTEGRATED_H

#include "SimpleFilters.h"
#include "SimpleI2C.h"
#include <Arduino.h>

#define APDS9960_ADDR 0x39

typedef struct
{
    // Сырые данные
    uint16_t red, green, blue, clear;
    uint8_t proximity;

    // Отфильтрованные данные
    float filteredRed, filteredGreen, filteredBlue, filteredClear;
    float filteredProximity;

    // Фильтры
    RunningAverageFilter redFilter;
    RunningAverageFilter greenFilter;
    RunningAverageFilter blueFilter;
    RunningAverageFilter clearFilter;
    RunningAverageFilter proximityFilter;

    // Добавляем поле статуса инициализации
    bool isInitialized;

} APDS9960_Sensor;

bool APDS9960_Init(APDS9960_Sensor *sensor);
void APDS9960_Update(APDS9960_Sensor *sensor);

#endif
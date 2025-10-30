#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>

// === НАСТРАИВАЕМЫЕ ПАРАМЕТРЫ ДАТЧИКА ===

#define GY21_ADDRESS 0x40

// Команды датчика
#define TEMP_CMD 0xE3
#define HUM_CMD 0xE5

// Задержки
#define SENSOR_DELAY 50

// Коэффициенты калибровки
#define TEMP_MULTIPLIER 175.72
#define TEMP_DIVISOR 65536.0
#define TEMP_OFFSET -46.85

#define HUM_MULTIPLIER 125.0
#define HUM_DIVISOR 65536.0
#define HUM_OFFSET -6.0

// =======================================

void sensor_read(void);
bool sensor_ready(void);
void sensor_set_ready(bool state);
float sensor_get_temp(void);
float sensor_get_hum(void);

#endif
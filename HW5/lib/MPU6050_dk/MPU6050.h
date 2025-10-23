#ifndef MPU6050_H
#define MPU6050_H

#include "SimpleFilters.h"
#include "SimpleI2C.h"
#include <Arduino.h>

// Структура для хранения всех данных и фильтров одного датчика MPU6050
typedef struct
{
    // Адрес датчика на I2C шине
    uint8_t address;

    // Сырые данные
    int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

    // Отфильтрованные физические величины
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float temperature;
    float angleX, angleY;

    // Фильтры
    RunningAverageFilter accelXFilter;
    RunningAverageFilter accelYFilter;
    RunningAverageFilter accelZFilter;
    RunningAverageFilter gyroXFilter;
    RunningAverageFilter gyroYFilter;
    RunningAverageFilter gyroZFilter;
    KalmanFilter kfX;
    KalmanFilter kfY;

    // Время последнего обновления
    unsigned long lastUpdate;

    // Статус инициализации
    bool isInitialized;
} MPU6050_Sensor;

// Инициализация датчика - теперь возвращает bool
bool MPU6050_Init(MPU6050_Sensor *sensor, uint8_t address);

// Обновление данных датчика
void MPU6050_Update(MPU6050_Sensor *sensor, float dt);

// Чтение сырых данных
void MPU6050_ReadRaw(MPU6050_Sensor *sensor);

#endif
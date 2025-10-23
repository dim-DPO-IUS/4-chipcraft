#ifndef SENSORSI2C_H
#define SENSORSI2C_H

#include "SimpleI2C.h"
#include <Arduino.h>

// ==================== GY-521 (MPU-605)  ====================
// I2C адрес MPU-6050
#define MPU_ADDR 0x68    // I2C адрес первого MPU-6050
#define MPU_ADDR2 0x69   // I2C адрес второго MPU-6050

// Константы MPU-6050
#define ACCEL_SENSITIVITY 16384.0f // LSB/g для ±2g
#define GYRO_SENSITIVITY 131.0f    // LSB/°/сек для ±250°/сек
#define TEMP_SENSITIVITY 340.0f    // LSB/°C
#define TEMP_OFFSET 36.53f         // Смещение температуры

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif // M_PI

// Универсальная функция чтения IMU датчика
void readIMU(uint8_t deviceAddr, int16_t *acx, int16_t *acy, int16_t *acz,
             int16_t *gyx, int16_t *gyy, int16_t *gyz, int16_t *tmp);

// Перевод акселерометра в g (ускорение свободного падения)
float accelToG(int16_t rawValue);

// Перевод гироскопа в градусы/секунду
float gyroToDegPerSec(int16_t rawValue);

// Вычисление углов наклона из акселерометра
void accelToTiltAngles(int16_t rawX, int16_t rawY, int16_t rawZ, float *angleX, float *angleY);

// Вычисление температуры в °C
float tempToCelsius(int16_t rawValue);

// ==================== TMP102  ==============================
// Для датчика температуры TMP102
#define TMP102_ADDR 0x48
float readTemperature();

// ==================== BH1750  ==============================
// Для датчика освещенности BH1750
#define BH1750_ADDR 0x23
uint16_t readLight();

#endif
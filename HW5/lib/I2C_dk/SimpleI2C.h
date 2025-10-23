#ifndef SIMPLEI2C_H
#define SIMPLEI2C_H

#include <Arduino.h>
#include <Wire.h>

// Инициализация I2C (вызвать один раз в setup)
void I2C_Init(void);

// Чтение нескольких байтов из устройства
void I2C_ReadBytes(uint8_t deviceAddr, uint8_t regAddr, uint8_t *buffer, uint8_t length);

// Запись байта в устройство
void I2C_WriteByte(uint8_t deviceAddr, uint8_t regAddr, uint8_t data);

#endif
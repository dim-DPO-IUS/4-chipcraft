#ifndef CONFIG_MATRIX_H
#define CONFIG_MATRIX_H

#include <LedControl.h>

// Конфигурация пинов матрицы
#define MATRIX_DIN_PIN 11
#define MATRIX_CLK_PIN 13
#define MATRIX_CS_PIN 10
#define MATRIX_DEVICES 1

// Температурные пороги для отображения
#define TEMP_NEUTRAL 25.0
#define TEMP_HAPPY 28.0
#define TEMP_HOT 32.0
#define TEMP_OVERHEAT 36.0
#define TEMP_EXTREME 40.0

// Битовые маски цифр для матрицы
extern const byte digits[10][5];

// Прототипы функций работы с матрицей
void initMatrix();
void displayDigit( int digit, int x_offset);
void displayTemperature(float temperatureC);
void testAllDigits();

#endif
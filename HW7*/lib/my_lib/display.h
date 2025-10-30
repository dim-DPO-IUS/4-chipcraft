#ifndef DISPLAY_H
#define DISPLAY_H

#include "sensor.h"
#include <Arduino_ST7789_Fast.h>

// === НАСТРАИВАЕМЫЕ ПАРАМЕТРЫ ДИСПЛЕЯ ===

// Пины
#define TFT_DC 7
#define TFT_RST 8
#define TFT_CS 10

// Цвета
#define BLACK 0x0000
#define YELLOW 0xFFE0
#define CYAN 0x07FF
#define WHITE 0xFFFF
#define GREEN 0x07E0

// Позиции и размеры
#define HEADER_TEXT_SIZE 2
#define VALUE_TEXT_SIZE 3
// -------------------------------
#define TEMP_LABEL_X 20
#define TEMP_LABEL_Y 15
#define TEMP_VALUE_X 30
#define TEMP_VALUE_Y 40
// -------------------------------
#define HUM_LABEL_X 20
#define HUM_LABEL_Y 95
#define HUM_VALUE_X 30
#define HUM_VALUE_Y 120
// -------------------------------
#define LINE_X1 10
#define LINE_Y 75
#define LINE_LENGTH 220
// -------------------------------
// Для тестового индикатора
// #define INDICATOR_TEXT_SIZE 3
// #define INDICATOR_X 200
// #define INDICATOR_Y 5
// Для графического индикатора
#define INDICATOR_CENTER_X 210
#define INDICATOR_CENTER_Y 15
#define INDICATOR_RADIUS 8
// -------------------------------

// Цвета элементов
#define LABEL_COLOR GREEN
#define LINE_COLOR GREEN
#define TEMP_COLOR YELLOW
#define HUM_COLOR CYAN
#define INDICATOR_COLOR WHITE

// =======================================

void display_init(void);
void display_update(void);
bool display_ready(void);
void display_set_ready(bool state);

#endif
#ifndef CLOCK_DISPLAY_H
#define CLOCK_DISPLAY_H

#include <Arduino_ST7789_Fast.h>
#include <math.h>

// Структура для хранения состояния часов
typedef struct
{
    Arduino_ST7789 *tft;
    bool clockStopped;
    unsigned long stopStartTime;
    int lastSecX, lastSecY;
    int lastMinX, lastMinY;
    int lastHourX, lastHourY;
} ClockDisplay;

// Функции инициализации
void clock_init(ClockDisplay *clock, Arduino_ST7789 *tft);
void clock_drawStatic(ClockDisplay *clock);

// Функции обновления
void clock_updateTime(ClockDisplay *clock, int hours, int minutes, int seconds);
void clock_setStopped(ClockDisplay *clock, bool stopped);
void clock_drawStatus(ClockDisplay *clock);

#endif
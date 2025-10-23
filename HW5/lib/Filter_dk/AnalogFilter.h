#ifndef ANALOG_FILTER_H
#define ANALOG_FILTER_H

#include <Arduino.h>

// Базовый класс фильтра
typedef struct
{
    float (*read)(void *filter);             // Функция чтения
    void (*update)(void *filter, int value); // Функция обновления
    void (*reset)(void *filter);             // Функция сброса
} FilterInterface;

// Структура для простого усреднения
typedef struct
{
    FilterInterface interface;
    int *readings;
    int numReadings;
    int readIndex;
    long total;
} MovingAverageFilter;

// Структура для экспоненциального фильтра
typedef struct
{
    FilterInterface interface;
    float filteredValue;
    float alpha;
} ExponentialFilter;

// Структура для медианного фильтра
typedef struct
{
    FilterInterface interface;
    int *values;
    int windowSize;
    int currentIndex;
} MedianFilter;

// Структура для фильтра с гистерезисом
typedef struct
{
    FilterInterface interface;
    int lastValue;
    int threshold;
} HysteresisFilter;

// Функции инициализации
void initMovingAverage(MovingAverageFilter *filter, int numReadings);
void initExponential(ExponentialFilter *filter, float alpha);
void initMedian(MedianFilter *filter, int windowSize);
void initHysteresis(HysteresisFilter *filter, int threshold);

// Вспомогательные функции
int quickSelect(int arr[], int n, int k);

#endif
#include "AnalogFilter.h"
#include <stdlib.h>

// ===== Moving Average Filter =====
float readMovingAverage(void *filter)
{
    MovingAverageFilter *f = (MovingAverageFilter *)filter;
    return (float)(f->total / f->numReadings);
}

void updateMovingAverage(void *filter, int value)
{
    MovingAverageFilter *f = (MovingAverageFilter *)filter;

    f->total -= f->readings[f->readIndex];
    f->readings[f->readIndex] = value;
    f->total += value;

    f->readIndex++;
    if (f->readIndex >= f->numReadings) {
        f->readIndex = 0;
    }
}

void resetMovingAverage(void *filter)
{
    MovingAverageFilter *f = (MovingAverageFilter *)filter;
    f->total = 0;
    f->readIndex = 0;
    for (int i = 0; i < f->numReadings; i++) {
        f->readings[i] = 0;
    }
}

void initMovingAverage(MovingAverageFilter *filter, int numReadings)
{
    filter->readings = (int *)malloc(numReadings * sizeof(int));
    filter->numReadings = numReadings;
    filter->readIndex = 0;
    filter->total = 0;

    filter->interface.read = readMovingAverage;
    filter->interface.update = updateMovingAverage;
    filter->interface.reset = resetMovingAverage;

    resetMovingAverage(filter);
}

// ===== Exponential Filter =====
float readExponential(void *filter)
{
    ExponentialFilter *f = (ExponentialFilter *)filter;
    return f->filteredValue;
}

void updateExponential(void *filter, int value)
{
    ExponentialFilter *f = (ExponentialFilter *)filter;
    // f->filteredValue = f->alpha * value + (1 - f->alpha) * f->filteredValue;
    f->filteredValue += (value - f->filteredValue) * f->alpha;
}

void resetExponential(void *filter)
{
    ExponentialFilter *f = (ExponentialFilter *)filter;
    f->filteredValue = 0;
}

void initExponential(ExponentialFilter *filter, float alpha)
{
    filter->alpha = alpha;
    filter->filteredValue = 0;

    filter->interface.read = readExponential;
    filter->interface.update = updateExponential;
    filter->interface.reset = resetExponential;
}

// ===== Median Filter =====
// Быстрый выбор для медианы (QuickSelect алгоритм)
int quickSelect(int arr[], int n, int k)
{
    int low = 0, high = n - 1;

    while (low <= high) {
        int pivotIndex = (low + high) / 2;
        int pivotValue = arr[pivotIndex];

        // Перестановка элементов
        int i = low, j = high;
        while (i <= j) {
            while (arr[i] < pivotValue)
                i++;
            while (arr[j] > pivotValue)
                j--;
            if (i <= j) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                i++;
                j--;
            }
        }

        if (k <= j) {
            high = j;
        } else if (k >= i) {
            low = i;
        } else {
            return arr[k];
        }
    }
    return arr[k];
}

float readMedian(void *filter)
{
    MedianFilter *f = (MedianFilter *)filter;

    // Создаем копию для сортировки
    int *temp = (int *)malloc(f->windowSize * sizeof(int));
    for (int i = 0; i < f->windowSize; i++) {
        temp[i] = f->values[i];
    }

    int median = quickSelect(temp, f->windowSize, f->windowSize / 2);
    free(temp);

    return (float)median;
}

void updateMedian(void *filter, int value)
{
    MedianFilter *f = (MedianFilter *)filter;
    f->values[f->currentIndex] = value;
    f->currentIndex = (f->currentIndex + 1) % f->windowSize;
}

void resetMedian(void *filter)
{
    MedianFilter *f = (MedianFilter *)filter;
    f->currentIndex = 0;
    for (int i = 0; i < f->windowSize; i++) {
        f->values[i] = 0;
    }
}

void initMedian(MedianFilter *filter, int windowSize)
{
    filter->values = (int *)malloc(windowSize * sizeof(int));
    filter->windowSize = windowSize;
    filter->currentIndex = 0;

    filter->interface.read = readMedian;
    filter->interface.update = updateMedian;
    filter->interface.reset = resetMedian;

    resetMedian(filter);
}

// ===== Hysteresis Filter =====
float readHysteresis(void *filter)
{
    HysteresisFilter *f = (HysteresisFilter *)filter;
    return (float)f->lastValue;
}

void updateHysteresis(void *filter, int value)
{
    HysteresisFilter *f = (HysteresisFilter *)filter;
    if (abs(value - f->lastValue) >= f->threshold) {
        f->lastValue = value;
    }
}

void resetHysteresis(void *filter)
{
    HysteresisFilter *f = (HysteresisFilter *)filter;
    f->lastValue = 0;
}

void initHysteresis(HysteresisFilter *filter, int threshold)
{
    filter->threshold = threshold;
    filter->lastValue = 0;

    filter->interface.read = readHysteresis;
    filter->interface.update = updateHysteresis;
    filter->interface.reset = resetHysteresis;
}
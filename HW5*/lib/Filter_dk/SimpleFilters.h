
#ifndef SIMPLEFILTERS_H
#define SIMPLEFILTERS_H

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// ==================== БЕГУЩЕЕ СРЕДНЕЕ ====================

typedef struct
{
    float buffer[10];
    int index;
    float sum;
    int samples;
    int size;
} RunningAverageFilter;

float runningAverage(RunningAverageFilter *filter, int size, float newValue);
void resetRunningAverage(RunningAverageFilter *filter);

// ==================== ЭКСПОНЕНЦИАЛЬНЫЙ ФИЛЬТР ====================

typedef struct
{
    float value;
    float alpha;
} ExponentialFilter;

float exponentialFilter(ExponentialFilter *filter, float alpha, float newValue);
void resetExponential(ExponentialFilter *filter);

// ==================== МЕДИАННЫЙ ФИЛЬТР ====================

typedef struct
{
    float buffer[5];
    int index;
    int size;
} MedianFilter;

float medianFilter(MedianFilter *filter, int size, float newValue);
void resetMedian(MedianFilter *filter);

// ==================== ОКОННЫЙ ФИЛЬТР ====================

typedef struct
{
    uint8_t state;
    unsigned long lastTime;
    uint32_t samplePeriod;
    uint8_t windowSize;
} WindowFilter;

int windowFilter(WindowFilter *filter, uint8_t windowSize, uint32_t samplePeriod, bool newState);
void resetWindow(WindowFilter *filter);

// ==================== COMPLEMENTARY FILTER ====================

typedef struct
{
    float angle;
    float alpha;
    unsigned long lastTime;
} ComplementaryFilter;

float complementaryFilter(ComplementaryFilter *filter, float alpha, float accelAngle, float gyroRate, float dt);
void resetComplementary(ComplementaryFilter *filter);

// ==================== KALMAN FILTER ====================

typedef struct
{
    float x;  // Estimated value
    float p;  // Estimation error covariance
    float q;  // Process noise covariance
    float r;  // Measurement noise covariance
    unsigned long lastTime;
} KalmanFilter;

float kalmanFilter(KalmanFilter *filter, float measurement, float dt);
void resetKalman(KalmanFilter *filter);
void initKalman(KalmanFilter *filter, float q, float r, float initialValue);

#endif

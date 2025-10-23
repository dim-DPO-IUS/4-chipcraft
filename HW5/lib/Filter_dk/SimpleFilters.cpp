
#include "SimpleFilters.h"

// ==================== БЕГУЩЕЕ СРЕДНЕЕ ====================

float runningAverage(RunningAverageFilter *filter, int size, float newValue)
{
    // Авто-инициализация при первом вызове
    if (filter->size == 0) {
        filter->size = (size <= 10) ? size : 10;
        resetRunningAverage(filter);
    }

    // Стандартная логика фильтра
    filter->sum -= filter->buffer[filter->index];
    filter->buffer[filter->index] = newValue;
    filter->sum += newValue;
    filter->index = (filter->index + 1) % filter->size;

    if (filter->samples < filter->size) {
        filter->samples++;
    }

    return filter->sum / filter->samples;
}

void resetRunningAverage(RunningAverageFilter *filter)
{
    filter->index = 0;
    filter->sum = 0;
    filter->samples = 0;
    memset(filter->buffer, 0, sizeof(filter->buffer));
}

// ==================== ЭКСПОНЕНЦИАЛЬНЫЙ ФИЛЬТР ====================

float exponentialFilter(ExponentialFilter *filter, float alpha, float newValue)
{
    // Авто-инициализация при первом вызове
    if (filter->alpha == 0) {
        filter->alpha = alpha;
        filter->value = newValue; // Первое значение
    } else {
        filter->value = filter->alpha * newValue + (1.0f - filter->alpha) * filter->value;
    }
    return filter->value;
}

void resetExponential(ExponentialFilter *filter)
{
    filter->value = 0;
    filter->alpha = 0; // Сбрасываем alpha для повторной инициализации
}

// ==================== МЕДИАННЫЙ ФИЛЬТР ====================

float medianFilter(MedianFilter *filter, int size, float newValue)
{
    // Авто-инициализация при первом вызове
    if (filter->size == 0) {
        filter->size = (size <= 5) ? size : 5;
        resetMedian(filter);
    }

    filter->buffer[filter->index] = newValue;
    filter->index = (filter->index + 1) % filter->size;

    // Сортировка пузырьком
    float temp[5];
    for (int i = 0; i < filter->size; i++) {
        temp[i] = filter->buffer[i];
    }

    for (int i = 0; i < filter->size - 1; i++) {
        for (int j = 0; j < filter->size - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                float swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }
        }
    }

    return temp[filter->size / 2];
}

void resetMedian(MedianFilter *filter)
{
    filter->index = 0;
    filter->size = 0; // Сбрасываем size для повторной инициализации
    memset(filter->buffer, 0, sizeof(filter->buffer));
}

// ==================== ОКОННЫЙ ФИЛЬТР ====================

int windowFilter(WindowFilter *filter, uint8_t windowSize, uint32_t samplePeriod, bool newState)
{
    // Авто-инициализация при первом вызове
    if (filter->windowSize == 0) {
        filter->windowSize = (windowSize <= 8) ? windowSize : 8;
        filter->samplePeriod = samplePeriod;
        resetWindow(filter);
    }

    unsigned long currentTime = millis();

    if (currentTime - filter->lastTime >= filter->samplePeriod) {
        filter->lastTime = currentTime;

        filter->state = (filter->state << 1) | (newState ? 1 : 0);
        uint8_t mask = (1 << filter->windowSize) - 1;
        filter->state &= mask;

        if (filter->state == mask)
            return 1; // Стабильно HIGH
        else if (filter->state == 0)
            return 0; // Стабильно LOW
        else
            return -1; // Переходное состояние
    }

    return -2; // Время не пришло
}

void resetWindow(WindowFilter *filter)
{
    filter->lastTime = 0;
    filter->state = 0;
    filter->windowSize = 0; // Сбрасываем для повторной инициализации
    filter->samplePeriod = 0;
}

// ==================== COMPLEMENTARY FILTER ====================

float complementaryFilter(ComplementaryFilter *filter, float alpha, float accelAngle, float gyroRate, float dt)
{
    // Авто-инициализация при первом вызове
    if (filter->alpha == 0) {
        filter->alpha = alpha;
        filter->angle = accelAngle; // Первое значение из акселерометра
        filter->lastTime = millis();
        return filter->angle;
    }

    // Расчет времени, если dt не предоставлен
    if (dt <= 0) {
        unsigned long currentTime = millis();
        dt = (currentTime - filter->lastTime) / 1000.0f;
        filter->lastTime = currentTime;
    }

    // Complementary filter formula
    filter->angle = filter->alpha * (filter->angle + gyroRate * dt) + (1.0f - filter->alpha) * accelAngle;

    return filter->angle;
}

void resetComplementary(ComplementaryFilter *filter)
{
    filter->angle = 0;
    filter->alpha = 0;
    filter->lastTime = 0;
}

// ==================== KALMAN FILTER ====================

float kalmanFilter(KalmanFilter *filter, float measurement, float dt)
{
    // Авто-инициализация при первом вызове
    if (filter->q == 0 && filter->r == 0) {
        // Значения по умолчанию если не инициализированы
        filter->q = 0.1f;
        filter->r = 0.1f;
        filter->x = measurement;
        filter->p = 1.0f;
        filter->lastTime = millis();
        return filter->x;
    }

    // Расчет времени, если dt не предоставлен
    if (dt <= 0) {
        unsigned long currentTime = millis();
        dt = (currentTime - filter->lastTime) / 1000.0f;
        filter->lastTime = currentTime;
    }

    // Prediction step
    filter->p = filter->p + filter->q * dt;

    // Update step
    float k = filter->p / (filter->p + filter->r);
    filter->x = filter->x + k * (measurement - filter->x);
    filter->p = (1 - k) * filter->p;

    return filter->x;
}

void resetKalman(KalmanFilter *filter)
{
    filter->x = 0;
    filter->p = 1.0f;
    filter->q = 0;
    filter->r = 0;
    filter->lastTime = 0;
}

// Инициализация фильтра Калмана с кастомными параметрами
void initKalman(KalmanFilter *filter, float q, float r, float initialValue)
{
    filter->x = initialValue;
    filter->p = 1.0f;
    filter->q = q;
    filter->r = r;
    filter->lastTime = millis();
}

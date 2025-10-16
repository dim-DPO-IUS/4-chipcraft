#ifndef NTC_THERMISTOR_H
#define NTC_THERMISTOR_H

#include <math.h>
#include <stdint.h>

typedef struct
{
    uint8_t pin;
    float nominal_resistance;
    float nominal_temp;
    float b_coefficient;
    float series_resistor;
    uint16_t adc_resolution;
} NTC_Thermistor;

// Готовая конфигурация по умолчанию (наш датчик)
extern const NTC_Thermistor DEFAULT_NTC_CONFIG;

// Функции библиотеки
void NTC_init(NTC_Thermistor *sensor);
float NTC_readCelsius(NTC_Thermistor *sensor);
float NTC_readFahrenheit(NTC_Thermistor *sensor);
float NTC_readKelvin(NTC_Thermistor *sensor);
float NTC_readResistance(NTC_Thermistor *sensor);
int NTC_readRawADC(NTC_Thermistor *sensor);

#endif
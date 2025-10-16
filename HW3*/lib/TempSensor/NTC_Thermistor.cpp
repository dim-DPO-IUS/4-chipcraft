#include "NTC_Thermistor.h"
#include "Arduino.h"

// Конфигурация НАШЕГО конкретного датчика
const NTC_Thermistor DEFAULT_NTC_CONFIG = {
    .pin = A0,
    .nominal_resistance = 10000, // 10 кОм при 25°C (nominal_temp)
    .nominal_temp = 25,          // Температура калибровки
    .b_coefficient = 3950,       // Бета-коэффициент - УТОЧНИТЬ!!!
    .series_resistor = 10000,    // Резистор делителя
    .adc_resolution = 1024       // Разрешение АЦП Arduino
};

void NTC_init(NTC_Thermistor *sensor)
{
    // Копируем конфигурацию по умолчанию
    *sensor = DEFAULT_NTC_CONFIG;
    pinMode(sensor->pin, INPUT);
}

float NTC_readCelsius(NTC_Thermistor *sensor)
{
    float resistance = NTC_readResistance(sensor);
    float steinhart = resistance / sensor->nominal_resistance;
    steinhart = logf(steinhart);
    steinhart /= sensor->b_coefficient;
    steinhart += 1.0f / (sensor->nominal_temp + 273.15f);
    steinhart = 1.0f / steinhart;
    return steinhart - 273.15f;
}

float NTC_readFahrenheit(NTC_Thermistor *sensor)
{
    return NTC_readCelsius(sensor) * 9.0f / 5.0f + 32.0f;
}

float NTC_readKelvin(NTC_Thermistor *sensor)
{
    return NTC_readCelsius(sensor) + 273.15f;
}

float NTC_readResistance(NTC_Thermistor *sensor)
{
    int raw_value = NTC_readRawADC(sensor);
    float voltage = (raw_value / (float)sensor->adc_resolution) * 5.0f;
    return sensor->series_resistor * (5.0f / voltage - 1.0f);
}

int NTC_readRawADC(NTC_Thermistor *sensor)
{
    return analogRead(sensor->pin);
}
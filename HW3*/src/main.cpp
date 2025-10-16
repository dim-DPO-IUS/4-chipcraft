#include "../lib/AnalogFilter/AnalogFilter.h"
#include "../lib/Config/config_pins.h"
#include "../lib/Matrix/matrix.h"
#include "../lib/TempSensor/NTC_Thermistor.h"
#include <Arduino.h>

// Объявляем фильтр
MovingAverageFilter avgFilter;
// Объявляем термодатчик
NTC_Thermistor thermistor;

void setup()
{
    Serial.begin(9600);

    init_all_pins();

    // Инициализируем термодатчик
    NTC_init(&thermistor);
    // Инициализируем фильтр (10 samples)
    initMovingAverage(&avgFilter, 10);
    // Инициализация матрицы
    initMatrix();

    Serial.println("Temperature display ready");
}

void loop()
{
    float temperatureC = NTC_readCelsius(&thermistor);

    // Преобразуем в int для фильтра (сохраняем точность ×10)
    int temperature_int = (int)(temperatureC * 10);

    // Фильтрация данных
    avgFilter.interface.update(&avgFilter, temperature_int);
    float filteredValue = avgFilter.interface.read(&avgFilter) / 10.0;

    displayTemperature(filteredValue);

    static unsigned long last_print = 0;
    if (millis() - last_print > 1000) {
        Serial.print("Raw: ");
        Serial.print(temperatureC, 1);
        Serial.print("°C | Filtered: ");
        Serial.print(filteredValue, 1);
        Serial.println("°C");
        last_print = millis();
    }

    delay(100);
}

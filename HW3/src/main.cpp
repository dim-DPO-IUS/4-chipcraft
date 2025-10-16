#include "AnalogFilter.h"
#include "config_pins.h"

// Объявляем фильтр
MovingAverageFilter avgFilter;

void setup()
{
    Serial.begin(9600);

    // Инициализация всех пинов
    init_all_pins();

    // Инициализация фильтра
    initMovingAverage(&avgFilter, 12);

    Serial.println("System ready");
}

void loop()
{
    // Чтение потенциометра с A0
    int pot_value = analogRead(POTENTIOMETER_PIN);

    // Фильтрация данных
    avgFilter.interface.update(&avgFilter, pot_value);
    int filteredValue = (int)avgFilter.interface.read(&avgFilter);

    // Преобразование в ШИМ (0-255)
    int brightness = map(filteredValue, 0, 1023, 0, 255);

    // Управление яркостью светодиода на D6
    analogWrite(LED_PWM_PIN, brightness);

    // Отладочная информация (каждые 200мс)
    static unsigned long last_print = 0;
    if (millis() - last_print > 200) {
        Serial.print("Raw: ");
        Serial.print(pot_value);
        Serial.print(" Filtered: ");
        Serial.print(filteredValue);
        Serial.print(" PWM: ");
        Serial.println(brightness);
        last_print = millis();
    }

    delay(10);
}
#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include "debounce_simple.h"
#include <Arduino.h>

/**
 * @brief Обрабатывает нажатие кнопки с интервалом игнорирования
 * @param pin Номер пина кнопки
 * @param on_press Функция, которая вызывается при валидном нажатии
 * @return true - если было обработано нажатие, false - если нет
 *
 * ПРИМЕР ИСПОЛЬЗОВАНИЯ:
 *
 * void setup() {
 *     pinMode(13, OUTPUT);
 * }
 *
 * void loop() {
 *     if (handle_button(2, []() {
 *         digitalWrite(13, HIGH);
 *         Serial.println("Кнопка нажата!");
 *     })) {
 *         // Опционально: код после обработки нажатия
 *         digitalWrite(13, LOW);
 *     }
 * }
 */
bool handle_button(uint8_t pin, void (*on_press)());

/*ПРИМЕРЫ:
=======================================================
ДЛЯ КНОПКИ
=======================================================
#include "button_handler.h"
#include <Arduino.h>

#define BUTTON_PIN 2
#define LED_PIN 13

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
    handle_button(BUTTON_PIN, []() {
        digitalWrite(LED_PIN, HIGH);
        Serial.println(" Нажатие обработано");
        delay(1000); // Держим светодиод включенным 1 секунду
    });
    digitalWrite(LED_PIN, LOW);
    delay(10);
}

=======================================================
ДЛЯ КНОПКИ И ПРЕРЫВАНИЙ
=======================================================
-------------------------------------------------------
Сочетает преимущества прерываний и антидребезга
Максимально простой - всего 2 строки в loop()
Чистая логика - прерывание ставит флаг, антидребезг его обрабатывает
Стабильная работа - никаких ложных срабатываний
................................
Как это работает:
1. Прерывание -> button_irq = true
2. handle_button() проверяет антидребезг
3. Если антидребезг пройден -> вызывается lambda
4. В lambda проверяем button_irq (флаг прерывания) и обрабатываем
5. Сбрасываем button_irq = false
................................
Это сочетает:
Мгновенную реакцию прерывания
Надежный антидребезг
Простоту использования
Минимальный код

-------------------------------------------------------
#include "button_handler.h"
#include <Arduino.h>

#define BUTTON_PIN 2
#define LED_PIN 13

volatile bool button_irq = false;

void handleInterrupt()
{
    button_irq = true;
}

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);
    Serial.println("Старт...");
}

void loop()
{
    // Просто вызываем handle_button в loop() - она сама обработает антидребезг
    handle_button(BUTTON_PIN, []() {
        // Этот блок обработки флага прерывания вызовется только после антидребезга
        if (button_irq) {
            digitalWrite(LED_PIN, !digitalRead(LED_PIN));
            Serial.println("Нажатие обработано!");
            button_irq = false;
        }
    });

    delay(10);
}
=======================================================
ЕЩЕ ВАРИАНТ ДЛЯ КНОПКИ И ПРЕРЫВАНИЙ
=======================================================
#include "button_handler.h"
#include <Arduino.h>

#define BUTTON_PIN 2
#define LED_PIN 4

volatile bool button_irq = false;

void handleInterrupt()
{
    button_irq = true;
}

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);
    Serial.println("Старт...");
}

void loop()
{
    // Обработка кнопки с антидребезгом + обработка флага прерываний
    handle_button(BUTTON_PIN, []() {
        if (button_irq) {
            digitalWrite(LED_PIN, !digitalRead(LED_PIN));
            Serial.println(" >> Прерывание + антидребезг!");
            button_irq = false;
        }
    });

    // Основная программа что то делает раз в 100 мс
    static unsigned long last_print = 0;
    if (millis() - last_print > 100) {
        last_print = millis();
        Serial.print(".");
    }
}
=======================================================
=======================================================

=======================================================
=======================================================

=======================================================
*/

#endif
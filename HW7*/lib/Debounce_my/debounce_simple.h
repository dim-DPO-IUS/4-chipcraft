#ifndef DEBOUNCE_SIMPLE_H
#define DEBOUNCE_SIMPLE_H

#include <Arduino.h>

/**
 * @brief Универсальная функция антидребезга для любого значения
 * @param read_period Период опроса в миллисекундах
 * @param current_value Текущее значение для проверки
 * @param target_state Целевое состояние (true/false)
 * @return true - достигнуто стабильное состояние, false - нет
 *
 * ПРИМЕРЫ ИСПОЛЬЗОВАНИЯ:
 *
 * // Для кнопки
 * if (debounce_check(10, digitalRead(BUTTON_PIN), false)) {
 *     // Кнопка стабильно нажата
 * }
 *
 * // Для любого флага
 * if (debounce_check(10, some_variable, true)) {
 *     // Переменная стабильно true
 * }
 *
 * // Для датчика
 * if (debounce_check(10, digitalRead(SENSOR_PIN), true)) {
 *     // Датчик стабильно активен
 * }
 */
bool debounce_check(uint16_t read_period, bool current_value, bool target_state);

/* ПРИМЕРЫ:
=======================================================
ДЛЯ ДАТЧИКОВ
=======================================================
#include "debounce_simple.h"

bool sensor_active = false;
bool system_ready = true;

void loop() {
    // Читаем датчик
    sensor_active = (analogRead(A0) > 500);

    // Проверяем, стабильно ли активен датчик
    if (debounce_check(10, sensor_active, true)) {
        Serial.println("Датчик стабильно активен!");
    }

    // Проверяем, стабильно ли система готова
    if (debounce_check(10, system_ready, false)) {
        Serial.println("Система стабильно не готова!");
    }
}

=======================================================
ДЛЯ КНОПКИ:
=======================================================
#include "debounce_simple.h"

#define BUTTON_PIN 2
#define LED_PIN 13

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
    // Проверяем стабильное нажатие кнопки
    if (debounce_check(10, digitalRead(BUTTON_PIN), false)) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Кнопка стабильно нажата!");
    }

    // Проверяем стабильное отпускание кнопки
    if (debounce_check(10, digitalRead(BUTTON_PIN), true)) {
        digitalWrite(LED_PIN, LOW);
        Serial.println("Кнопка стабильно отпущена!");
    }
}

=======================================================
ДЛЯ ЛЮБОЙ ПЕРЕМЕННОЙ:
=======================================================
#include "debounce_simple.h"

bool sensor_active = false;
bool system_ready = true;

void loop() {
    // Читаем датчик
    sensor_active = (analogRead(A0) > 500);

    // Проверяем, стабильно ли активен датчик
    if (debounce_check(10, sensor_active, true)) {
        Serial.println("Датчик стабильно активен!");
    }

    // Проверяем, стабильно ли система готова
    if (debounce_check(10, system_ready, false)) {
        Serial.println("Система стабильно не готова!");
    }
}

=======================================================
ДЛЯ КНОПКИ С ИНТЕРВАЛОМ ИГНОРИРОВАНИЯ ПОСЛЕ СОБЫТИЯ:
=======================================================
#include "debounce_simple.h"
#include <Arduino.h>

#define BUTTON_PIN 2
#define LED_PIN 13

bool ignore_period = true; // Начинаем с игнорирования

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
    // Если не в периоде игнорирования - проверяем кнопку
    if (!ignore_period && debounce_check(10, digitalRead(BUTTON_PIN), false)) {
        Serial.println(" Нажатие обработано");
        digitalWrite(LED_PIN, HIGH);
        // Начинаем период игнорирования
        ignore_period = true;
        Serial.println(" Игнорируем следующие нажатия...");
    }

    // Проверяем, закончился ли период игнорирования
    // Используем debounce_check с фиктивной переменной true
    // Ждет 80ms (8 × 10ms) с помощью фиктивной переменной
    if (ignore_period && debounce_check(10, true, true)) {
        ignore_period = false;
        Serial.println(" Готов к новому нажатию");
        digitalWrite(LED_PIN, LOW);
    }

    delay(10);
}

=======================================================
=======================================================
=======================================================
=======================================================


*/

#endif
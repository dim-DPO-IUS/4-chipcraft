#include "bit_ops.h"
#include "sr_74hc595.h"
#include <Arduino.h>

// Создаем объект сдвигового регистра
ShiftRegister74HC595 sr;

// ==================== ЭФФЕКТЫ ====================

// Эффект 1: Бегущий светодиод с разной скоростью
void effect_running_led()
{
    Serial.println("Эффект: Бегущий светодиод");

    // Быстрое движение
    for (uint16_t i = 0; i < sr_get_total_bits(&sr); i++) {
        sr_clear(&sr);
        sr_set_absolute_bit(&sr, i);
        sr_update(&sr);
        delay(80);
    }

    // Медленное движение в обратную сторону
    for (int16_t i = sr_get_total_bits(&sr) - 1; i >= 0; i--) {
        sr_clear(&sr);
        sr_set_absolute_bit(&sr, i);
        sr_update(&sr);
        delay(150);
    }
}

// Эффект 2: Двоичный счетчик с визуализацией
void effect_binary_counter()
{
    Serial.println("Эффект: Двоичный счетчик");

    for (uint16_t i = 0; i < 64; i++) {
        sr_set_register(&sr, 0, i);
        sr_update(&sr);

        // Вывод в Serial для наглядности
        Serial.print("Счетчик: ");
        Serial.print(i);
        Serial.print(" -> 0b");
        PRINT_BITS8(i);
        Serial.println();

        delay(200);
    }
}

// Эффект 3: Симметричные узоры
void effect_symmetric_patterns()
{
    Serial.println("Эффект: Симметричные узоры");

    uint8_t patterns[] = {
        B8(10000001), // Крайние
        B8(01000010),
        B8(00100100),
        B8(00011000), // Центр
        B8(00100100),
        B8(01000010),
        B8(10000001),
        B8(00000000) // Пауза
    };

    for (uint8_t i = 0; i < sizeof(patterns); i++) {
        sr_set_register(&sr, 0, patterns[i]);
        sr_update(&sr);
        delay(300);
    }
}

// Эффект 4: Случайные вспышки
void effect_random_sparkles()
{
    Serial.println("Эффект: Случайные вспышки");

    for (uint8_t i = 0; i < 20; i++) {
        uint8_t random_pattern = random(256); // Случайный байт
        sr_set_register(&sr, 0, random_pattern);
        sr_update(&sr);

        Serial.print("Случайный паттерн: 0b");
        PRINT_BITS8(random_pattern);
        Serial.println();

        delay(120);
    }
}

// Эффект 5: "Дыхание" - плавное заполнение
void effect_breathing()
{
    Serial.println("Эффект: Дыхание");

    // Заполнение
    for (uint8_t i = 0; i <= 8; i++) {
        uint8_t pattern = (1 << i) - 1; // Постепенное заполнение
        sr_set_register(&sr, 0, pattern);
        sr_update(&sr);

        Serial.print("Заполнение: ");
        Serial.print(i);
        Serial.print("/8 -> 0b");
        PRINT_BITS8(pattern);
        Serial.println();

        delay(200);
    }

    // Опустошение
    for (int8_t i = 8; i >= 0; i--) {
        uint8_t pattern = (1 << i) - 1;
        sr_set_register(&sr, 0, pattern);
        sr_update(&sr);
        delay(200);
    }
}

// Эффект 6: Бегущая группа светодиодов
void effect_running_group()
{
    Serial.println("Эффект: Бегущая группа");

    // Группа из 3 светодиодов
    for (uint8_t i = 0; i <= 5; i++) {
        uint8_t pattern = B8(00000111) << i;
        sr_set_register(&sr, 0, pattern);
        sr_update(&sr);

        Serial.print("Позиция: ");
        Serial.print(i);
        Serial.print(" -> 0b");
        PRINT_BITS8(pattern);
        Serial.println();

        delay(250);
    }
}

// Эффект 7: Мигание с разной частотой
void effect_blinking()
{
    Serial.println("Эффект: Мигание");

    // Быстрое мигание всех светодиодов
    for (uint8_t i = 0; i < 8; i++) {
        sr_set_all(&sr);
        delay(100);
        sr_clear(&sr);
        delay(100);
    }

    // Медленное мигание шахматкой
    for (uint8_t i = 0; i < 4; i++) {
        sr_set_register(&sr, 0, B8(10101010));
        delay(300);
        sr_set_register(&sr, 0, B8(01010101));
        delay(300);
    }
}

// ==================== ДЕМОНСТРАЦИЯ ====================

void setup()
{
    Serial.begin(9600);
    Serial.println("Демонстрация 74HC595");
    Serial.println("=============================================");

    // Инициализация сдвигового регистра
    // (DS-14)dataPin=13, (SHCP-11)clockPin=11, (STCP-12)latchPin=12
    sr_init(&sr, 13, 11, 12);

    // Очищаем регистр
    sr_clear(&sr);

    Serial.println("Библиотека 74HC595 инициализирована");
    Serial.print("Количество светодиодов: ");
    Serial.println(sr_get_total_bits(&sr));
    Serial.println();
}

void loop()
{
    // Последовательно запускаем все эффекты
    effect_running_led();
    delay(500);

    effect_binary_counter();
    delay(500);

    effect_symmetric_patterns();
    delay(500);

    effect_random_sparkles();
    delay(500);

    effect_breathing();
    delay(500);

    effect_running_group();
    delay(500);

    effect_blinking();
    delay(1000);

    Serial.println("=============================================");
    Serial.println("Цикл эффектов завершен, начинаем заново!");
    Serial.println("=============================================");
}
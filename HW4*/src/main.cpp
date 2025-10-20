#include "bit_ops.h"
#include "debounce.h"
#include "encoder.h"
#include "sr_74hc595.h"
#include <Arduino.h>

ShiftRegister74HC595 sr_cascade;
Encoder my_encoder;

// Фильтры для антидребезга
DebounceAdvanced clk_filter;
DebounceAdvanced dt_filter;
DebounceAdvanced btn_filter;

void setup()
{
    Serial.begin(9600);
    Serial.println("Каскад 2x74HC595 + Энкодер");

    sr_init_cascade(&sr_cascade, 13, 11, 12, 2);
    encoder_init(&my_encoder, 2, 3, 4);

    // Фильтры для антидребезга
    clk_filter = DEBOUNCE_ADVANCED_INIT(2); // Быстрая фильтрация для энкодера
    dt_filter = DEBOUNCE_ADVANCED_INIT(2);
    btn_filter = DEBOUNCE_ADVANCED_INIT(5); // Надежная фильтрация для кнопки

    sr_clear(&sr_cascade);
    sr_update(&sr_cascade);

    Serial.println("Система инициализирована");
}

void loop()
{
    // ФИЛЬТРАЦИЯ ДЛЯ ЭНКОДЕРА
    bool clk_current = digitalRead(my_encoder.clk_pin);
    bool dt_current = digitalRead(my_encoder.dt_pin);

    // Обновляем фильтры энкодера
    debounce_advanced(&clk_filter, clk_current);
    debounce_advanced(&dt_filter, dt_current);

    // Обработка энкодера с использованием стабильных состояний
    encoder_process_debounced(&my_encoder,
                              clk_filter.last_stable_state,
                              dt_filter.last_stable_state);

    if (encoder_changed(&my_encoder)) {
        my_encoder.value &= 0x3F;
        uint8_t units = my_encoder.value & 0x07;
        uint8_t tens = (my_encoder.value >> 3) & 0x07;

        sr_clear(&sr_cascade);
        for (uint8_t i = 0; i < units; i++) {
            sr_set_absolute_bit(&sr_cascade, i);
        }
        for (uint8_t i = 0; i < tens; i++) {
            sr_set_absolute_bit(&sr_cascade, 8 + i);
        }
        sr_update(&sr_cascade);

        Serial.print("Счет: ");
        Serial.print(my_encoder.value);
        Serial.print(" = ");
        Serial.print(tens);
        Serial.print(" десятков + ");
        Serial.print(units);
        Serial.println(" единиц");
    }

    // ФИЛЬТРАЦИЯ ДЛЯ КНОПКИ
    static unsigned long last_btn_time = 0;

    bool btn_current = (digitalRead(my_encoder.sw_pin) == LOW);

    if (debounce_falling_edge(&btn_filter, btn_current)) {
        if (millis() - last_btn_time > 300) {
            last_btn_time = millis();
            encoder_reset(&my_encoder);
            sr_clear(&sr_cascade);
            sr_update(&sr_cascade);
            Serial.println("СБРОС!");
        }
    }

    delay(1);
}
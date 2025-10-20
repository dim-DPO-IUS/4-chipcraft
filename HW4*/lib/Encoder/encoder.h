#ifndef ENCODER_H
#define ENCODER_H

#include "bit_ops.h"
#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

// Структура для работы с энкодером
typedef struct
{
    uint8_t clk_pin;
    uint8_t dt_pin;
    uint8_t sw_pin;
    int value;
    int last_value;
    uint8_t state; // Состояние CLK и DT в битах 0 и 1
} Encoder;

// Прототипы функций
void encoder_init(Encoder *enc, uint8_t clk_pin, uint8_t dt_pin, uint8_t sw_pin);
void encoder_process(Encoder *enc);
void encoder_process_debounced(Encoder *enc, bool clk_stable, bool dt_stable); // +Антидребезг
bool encoder_changed(Encoder *enc);
void encoder_reset(Encoder *enc);
bool encoder_button_pressed(Encoder *enc);

#endif // ENCODER_H
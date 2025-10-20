#ifndef SR_74HC595_H
#define SR_74HC595_H

#include "../BitOps/bit_ops.h"
#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*
74HC595 должен быть подключен:
PIN16 (VCC)  → 5V
PIN8 (GND)   → GND
PIN10 (MR)   → 5V (СБРОС - активный LOW, поэтому к 5V чтобы отключить сброс)
PIN13 (OE)   → GND (РАЗРЕШЕНИЕ ВЫХОДОВ - активный LOW, к GND чтобы включить выходы)
PIN14 (DS)   → цифровой пин
PIN11 (SHCP)   → цифровой пин
PIN12 (STCP)   → цифровой пин

*/
// ==================== СТРУКТУРЫ ДАННЫХ ====================

typedef struct
{
    uint8_t dataPin;
    uint8_t clockPin;
    uint8_t latchPin;
    uint8_t numRegisters;
    uint8_t *registerValues;
} ShiftRegister74HC595;

// ==================== ПРОТОТИПЫ ФУНКЦИЙ ====================

// Инициализация и освобождение
void sr_init(ShiftRegister74HC595 *sr, uint8_t dataPin, uint8_t clockPin, uint8_t latchPin);
void sr_init_cascade(ShiftRegister74HC595 *sr, uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t numRegisters);
void sr_destroy(ShiftRegister74HC595 *sr);

// Основные функции
void sr_update(ShiftRegister74HC595 *sr);
void sr_clear(ShiftRegister74HC595 *sr);
void sr_set_all(ShiftRegister74HC595 *sr);

// Работа с битами
void sr_set_bit(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t bit);
void sr_clear_bit(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t bit);
void sr_toggle_bit(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t bit);
bool sr_read_bit(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t bit);

// Работа с абсолютными битами
void sr_set_absolute_bit(ShiftRegister74HC595 *sr, uint16_t absoluteBit);
void sr_clear_absolute_bit(ShiftRegister74HC595 *sr, uint16_t absoluteBit);
bool sr_read_absolute_bit(ShiftRegister74HC595 *sr, uint16_t absoluteBit);

// Работа с байтами
void sr_set_register(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t value);
uint8_t sr_get_register(ShiftRegister74HC595 *sr, uint8_t registerIndex);
void sr_set_all_registers(ShiftRegister74HC595 *sr, const uint8_t *values);

// Работа с многобайтными значениями
void sr_set_value_16(ShiftRegister74HC595 *sr, uint16_t value);
void sr_set_value_32(ShiftRegister74HC595 *sr, uint32_t value);
uint16_t sr_get_value_16(ShiftRegister74HC595 *sr);

// Вспомогательные функции
uint8_t sr_get_num_registers(ShiftRegister74HC595 *sr);
uint16_t sr_get_total_bits(ShiftRegister74HC595 *sr);
void sr_print_state(ShiftRegister74HC595 *sr);

// Готовые шаблоны эффектов
void sr_running_bit(ShiftRegister74HC595 *sr, uint16_t delayMs);
void sr_binary_counter(ShiftRegister74HC595 *sr, uint16_t maxCount, uint16_t delayMs);
void sr_progress_bar(ShiftRegister74HC595 *sr, uint16_t delayMs);
void sr_breathing_effect(ShiftRegister74HC595 *sr, uint16_t delayMs);

#endif // SR_74HC595_H
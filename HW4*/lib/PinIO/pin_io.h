#ifndef PIN_IO_H
#define PIN_IO_H

#include "bit_ops.h"
#include "pin_mapping.h" // Добавляем для PinMapping
#include <Arduino.h>

// Структура одной ножки МК
typedef struct
{
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin;
    uint8_t bit; // номер ножки МК
} DirectPin;

// Режимы работы пина
typedef enum
{
    PIN_MODE_INPUT = 0,
    PIN_MODE_OUTPUT = 1,
    PIN_MODE_INPUT_PULLUP = 2
} PinMode;

// Инициализация структуры пина
void pin_init(DirectPin *pin, volatile uint8_t *ddr, volatile uint8_t *port, volatile uint8_t *pin_reg, uint8_t bit);

// Универсальная инициализация по ардуино-пину
bool pin_init_arduino(DirectPin *pin, uint8_t arduino_pin, PinMode mode);

// Установка режима пина
void pin_mode(DirectPin *pin, PinMode mode);

// Цифровая запись
void pin_write(DirectPin *pin, uint8_t value);

// Установка нескольких пинов одновременно
void pins_write(DirectPin pins[], uint8_t count, uint8_t values);

// Цифровое чтение
bool pin_read(DirectPin *pin);

// Чтение нескольких пинов в байт
uint8_t pins_read(DirectPin pins[], uint8_t count);

// Переключение состояния пина
void pin_toggle(DirectPin *pin);

// Аналоговое чтение пина (ОТЛАДИТЬ!)
int analog_pin_read(uint8_t analog_pin);

#endif // PIN_IO_H
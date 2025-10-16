#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

#include <Arduino.h>

// Дополняем Arduino.h D-константами для единообразия
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13

typedef struct
{
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin;
    uint8_t bit;
} PinMapping;

// Функция для получения mapping по ардуино-пину
bool get_pin_mapping(uint8_t arduino_pin, PinMapping *result);

#endif // PIN_MAPPING_H
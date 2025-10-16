#include "pin_mapping.h"

bool get_pin_mapping(uint8_t arduino_pin, PinMapping *result)
{
    switch (arduino_pin) {
    // Аналоговые пины
    case A0:
        result->ddr = &DDRC;
        result->port = &PORTC;
        result->pin = &PINC;
        result->bit = 0;
        return true;
    case A1:
        result->ddr = &DDRC;
        result->port = &PORTC;
        result->pin = &PINC;
        result->bit = 1;
        return true;
    case A2:
        result->ddr = &DDRC;
        result->port = &PORTC;
        result->pin = &PINC;
        result->bit = 2;
        return true;
    case A3:
        result->ddr = &DDRC;
        result->port = &PORTC;
        result->pin = &PINC;
        result->bit = 3;
        return true;
    case A4:
        result->ddr = &DDRC;
        result->port = &PORTC;
        result->pin = &PINC;
        result->bit = 4;
        return true;
    case A5:
        result->ddr = &DDRC;
        result->port = &PORTC;
        result->pin = &PINC;
        result->bit = 5;
        return true;

    // Цифровые пины - используем D-константы
    case D0:
        result->ddr = &DDRD;
        result->port = &PORTD;
        result->pin = &PIND;
        result->bit = 0;
        return true;
    case D1:
        result->ddr = &DDRD;
        result->port = &PORTD;
        result->pin = &PIND;
        result->bit = 1;
        return true;
    case D2:
        result->ddr = &DDRD;
        result->port = &PORTD;
        result->pin = &PIND;
        result->bit = 2;
        return true;
    case D3:
        result->ddr = &DDRD;
        result->port = &PORTD;
        result->pin = &PIND;
        result->bit = 3;
        return true;
    case D4:
        result->ddr = &DDRD;
        result->port = &PORTD;
        result->pin = &PIND;
        result->bit = 4;
        return true;
    case D5:
        result->ddr = &DDRD;
        result->port = &PORTD;
        result->pin = &PIND;
        result->bit = 5;
        return true;
    case D6:
        result->ddr = &DDRD;
        result->port = &PORTD;
        result->pin = &PIND;
        result->bit = 6;
        return true;
    case D7:
        result->ddr = &DDRD;
        result->port = &PORTD;
        result->pin = &PIND;
        result->bit = 7;
        return true;

    case D8:
        result->ddr = &DDRB;
        result->port = &PORTB;
        result->pin = &PINB;
        result->bit = 0;
        return true;
    case D9:
        result->ddr = &DDRB;
        result->port = &PORTB;
        result->pin = &PINB;
        result->bit = 1;
        return true;
    case D10:
        result->ddr = &DDRB;
        result->port = &PORTB;
        result->pin = &PINB;
        result->bit = 2;
        return true;
    case D11:
        result->ddr = &DDRB;
        result->port = &PORTB;
        result->pin = &PINB;
        result->bit = 3;
        return true;
    case D12:
        result->ddr = &DDRB;
        result->port = &PORTB;
        result->pin = &PINB;
        result->bit = 4;
        return true;
    case D13:
        result->ddr = &DDRB;
        result->port = &PORTB;
        result->pin = &PINB;
        result->bit = 5;
        return true;

    default:
        return false;
    }
}
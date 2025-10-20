#include "pin_io.h"

// Инициализация структуры пина
void pin_init(DirectPin *pin, volatile uint8_t *ddr, volatile uint8_t *port, volatile uint8_t *pin_reg, uint8_t bit)
{
    pin->ddr = ddr;
    pin->port = port;
    pin->pin = pin_reg;
    pin->bit = bit;
}

// Универсальная инициализация по ардуино-пину
bool pin_init_arduino(DirectPin *pin, uint8_t arduino_pin, PinMode mode)
{
    PinMapping mapping;
    if (get_pin_mapping(arduino_pin, &mapping)) {
        pin_init(pin, mapping.ddr, mapping.port, mapping.pin, mapping.bit);
        pin_mode(pin, mode);
        return true;
    }
    return false;
}

// Установка режима пина
void pin_mode(DirectPin *pin, PinMode mode)
{
    switch (mode) {
    case PIN_MODE_OUTPUT:
        BIT_SET(*pin->ddr, pin->bit);
        break;

    case PIN_MODE_INPUT:
        BIT_CLEAR(*pin->ddr, pin->bit);
        BIT_CLEAR(*pin->port, pin->bit); // Отключаем подтяжку
        break;

    case PIN_MODE_INPUT_PULLUP:
        BIT_CLEAR(*pin->ddr, pin->bit);
        BIT_SET(*pin->port, pin->bit); // Включаем подтяжку
        break;
    }
}

// Цифровая запись
void pin_write(DirectPin *pin, bool value)
{
    BIT_WRITE(*pin->port, pin->bit, value);
}

// Установка нескольких пинов одновременно
void pins_write(DirectPin pins[], uint8_t count, uint8_t values)
{
    for (uint8_t i = 0; i < count; i++) {
        BIT_WRITE(*pins[i].port, pins[i].bit, BIT_READ(values, i));
    }
}

// Цифровое чтение
bool pin_read(DirectPin *pin)
{
    return BIT_READ(*pin->pin, pin->bit);
}

// Чтение нескольких пинов в байт
uint8_t pins_read(DirectPin pins[], uint8_t count)
{
    uint8_t result = 0;
    for (uint8_t i = 0; i < count; i++) {
        BIT_WRITE(result, i, pin_read(&pins[i]));
    }
    return result;
}

// Переключение состояния пина
void pin_toggle(DirectPin *pin)
{
    BIT_TOGGLE(*pin->port, pin->bit);
}

// Аналоговое чтение пина (ОТЛАДИТЬ!)
// пока не используй!
int analog_pin_read(uint8_t analog_pin)
{
    if (analog_pin > 5)
        return 0;

    // Настройка АЦП
    ADMUX = B8(01000000) | (analog_pin & 0x07); // Референс AVcc, выбор канала

    // Правильная настройка ADCSRA
    // Включение, старт преобразования, предделитель 128
    ADCSRA = B8(11000111); // ADEN=1, ADSC=1, ADPS2=1, ADPS1=1, ADPS0=1
    // ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Ожидание завершения преобразования
    while (BIT_IS_SET(ADCSRA, ADSC)) // 6 бит
        ;

    return ADC;
}
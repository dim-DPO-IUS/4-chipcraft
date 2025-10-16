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
bool pin_init_arduino(DirectPin *pin, uint8_t arduino_pin, uint8_t mode)
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
void pin_mode(DirectPin *pin, uint8_t mode)
{
    if (mode == OUTPUT) {
        *pin->ddr |= (1 << pin->bit);
    } else if (mode == INPUT) {
        *pin->ddr &= ~(1 << pin->bit);
    } else if (mode == INPUT_PULLUP) {
        *pin->ddr &= ~(1 << pin->bit);
        *pin->port |= (1 << pin->bit);
    }
}

// Цифровая запись
void pin_write(DirectPin *pin, uint8_t value)
{
    if (value) {
        *pin->port |= (1 << pin->bit);
    } else {
        *pin->port &= ~(1 << pin->bit);
    }
}

// Установка нескольких пинов одновременно
void pins_write(DirectPin pins[], uint8_t count, uint8_t values)
{
    for (uint8_t i = 0; i < count; i++) {
        pin_write(&pins[i], (values >> i) & 1);
    }
}

// Цифровое чтение
uint8_t pin_read(DirectPin *pin)
{
    return (*pin->pin >> pin->bit) & 1;
}

// Чтение нескольких пинов в байт
uint8_t pins_read(DirectPin pins[], uint8_t count)
{
    uint8_t result = 0;
    for (uint8_t i = 0; i < count; i++) {
        result |= (pin_read(&pins[i]) << i);
    }
    return result;
}

// Переключение состояния пина
void pin_toggle(DirectPin *pin)
{
    *pin->port ^= (1 << pin->bit);
}

// Аналоговое чтение пина (ОТЛАДИТЬ!)
// пока не используй!
int analog_pin_read(uint8_t analog_pin)
{
    if (analog_pin > 5)
        return 0;

    // Настройка АЦП
    ADMUX = (1 << REFS0) | (analog_pin & 0x07);                                      // Референс AVcc, выбор канала
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Включение, старт преобразования, предделитель 128

    // Ожидание завершения преобразования
    while (ADCSRA & (1 << ADSC))
        ;

    return ADC;
}
#include "sr_74hc595.h"

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

// Эти функции становятся static в .cpp файле
static void sr_pulse_clock(ShiftRegister74HC595 *sr)
{
    digitalWrite(sr->clockPin, HIGH);
    digitalWrite(sr->clockPin, LOW);
}

static void sr_pulse_latch(ShiftRegister74HC595 *sr)
{
    digitalWrite(sr->latchPin, HIGH);
    digitalWrite(sr->latchPin, LOW);
}

// ==================== РЕАЛИЗАЦИЯ ПУБЛИЧНЫХ ФУНКЦИЙ ====================

// Инициализация для одного регистра
void sr_init(ShiftRegister74HC595 *sr, uint8_t dataPin, uint8_t clockPin, uint8_t latchPin)
{
    sr->dataPin = dataPin;
    sr->clockPin = clockPin;
    sr->latchPin = latchPin;
    sr->numRegisters = 1;
    sr->registerValues = (uint8_t *)malloc(1 * sizeof(uint8_t));
    VALUE_CLEAR(sr->registerValues[0]);

    pinMode(sr->dataPin, OUTPUT);
    pinMode(sr->clockPin, OUTPUT);
    pinMode(sr->latchPin, OUTPUT);
    digitalWrite(sr->latchPin, LOW);
}

// Инициализация для каскада регистров
void sr_init_cascade(ShiftRegister74HC595 *sr, uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t numRegisters)
{
    sr->dataPin = dataPin;
    sr->clockPin = clockPin;
    sr->latchPin = latchPin;
    sr->numRegisters = numRegisters;
    sr->registerValues = (uint8_t *)malloc(numRegisters * sizeof(uint8_t));

    for (uint8_t i = 0; i < numRegisters; i++) {
        VALUE_CLEAR(sr->registerValues[i]);
    }

    pinMode(sr->dataPin, OUTPUT);
    pinMode(sr->clockPin, OUTPUT);
    pinMode(sr->latchPin, OUTPUT);
    digitalWrite(sr->latchPin, LOW);
}

// Освобождение памяти
void sr_destroy(ShiftRegister74HC595 *sr)
{
    free(sr->registerValues);
    sr->registerValues = NULL;
}

// Обновить все регистры (отправить данные в аппаратуру)
void sr_update(ShiftRegister74HC595 *sr)
{
    digitalWrite(sr->latchPin, LOW);

    // Отправляем от последнего регистра к первому (каскад)
    for (int8_t reg = sr->numRegisters - 1; reg >= 0; reg--) {
        SHIFT_OUT_MSB(sr->dataPin, sr->clockPin, sr->registerValues[reg]);
    }

    sr_pulse_latch(sr);
}

// Установить все выходы в 0
void sr_clear(ShiftRegister74HC595 *sr)
{
    for (uint8_t i = 0; i < sr->numRegisters; i++) {
        VALUE_CLEAR(sr->registerValues[i]);
    }
    sr_update(sr);
}

// Установить все выходы в 1
void sr_set_all(ShiftRegister74HC595 *sr)
{
    for (uint8_t i = 0; i < sr->numRegisters; i++) {
        VALUE_SET_ALL(sr->registerValues[i], 8);
    }
    sr_update(sr);
}

// Установить бит в конкретном регистре
void sr_set_bit(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t bit)
{
    if (registerIndex < sr->numRegisters && bit < 8) {
        BIT_SET(sr->registerValues[registerIndex], bit);
    }
}

// Сбросить бит в конкретном регистре
void sr_clear_bit(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t bit)
{
    if (registerIndex < sr->numRegisters && bit < 8) {
        BIT_CLEAR(sr->registerValues[registerIndex], bit);
    }
}

// Переключить бит в конкретном регистре
void sr_toggle_bit(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t bit)
{
    if (registerIndex < sr->numRegisters && bit < 8) {
        BIT_TOGGLE(sr->registerValues[registerIndex], bit);
    }
}

// Прочитать бит из конкретного регистра
bool sr_read_bit(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t bit)
{
    if (registerIndex < sr->numRegisters && bit < 8) {
        return BIT_READ(sr->registerValues[registerIndex], bit);
    }
    return false;
}

// Установить бит по абсолютному номеру (0-15 для 2 регистров и т.д.)
void sr_set_absolute_bit(ShiftRegister74HC595 *sr, uint16_t absoluteBit)
{
    uint8_t reg = absoluteBit / 8;
    uint8_t bit = absoluteBit % 8;
    if (reg < sr->numRegisters) {
        sr_set_bit(sr, reg, bit);
    }
}

// Сбросить бит по абсолютному номеру
void sr_clear_absolute_bit(ShiftRegister74HC595 *sr, uint16_t absoluteBit)
{
    uint8_t reg = absoluteBit / 8;
    uint8_t bit = absoluteBit % 8;
    if (reg < sr->numRegisters) {
        sr_clear_bit(sr, reg, bit);
    }
}

// Прочитать бит по абсолютному номеру
bool sr_read_absolute_bit(ShiftRegister74HC595 *sr, uint16_t absoluteBit)
{
    uint8_t reg = absoluteBit / 8;
    uint8_t bit = absoluteBit % 8;
    if (reg < sr->numRegisters) {
        return sr_read_bit(sr, reg, bit);
    }
    return false;
}

// Установить значение конкретного регистра
void sr_set_register(ShiftRegister74HC595 *sr, uint8_t registerIndex, uint8_t value)
{
    if (registerIndex < sr->numRegisters) {
        sr->registerValues[registerIndex] = value;
    }
}

// Получить значение конкретного регистра
uint8_t sr_get_register(ShiftRegister74HC595 *sr, uint8_t registerIndex)
{
    if (registerIndex < sr->numRegisters) {
        return sr->registerValues[registerIndex];
    }
    return 0;
}

// Установить значения всех регистров
void sr_set_all_registers(ShiftRegister74HC595 *sr, const uint8_t *values)
{
    for (uint8_t i = 0; i < sr->numRegisters; i++) {
        sr->registerValues[i] = values[i];
    }
}

// Установить 16-битное значение (для 2+ регистров)
void sr_set_value_16(ShiftRegister74HC595 *sr, uint16_t value)
{
    if (sr->numRegisters >= 2) {
        sr->registerValues[0] = value & 0xFF;        // Младший байт
        sr->registerValues[1] = (value >> 8) & 0xFF; // Старший байт
    }
}

// Установить 32-битное значение (для 4+ регистров)
void sr_set_value_32(ShiftRegister74HC595 *sr, uint32_t value)
{
    if (sr->numRegisters >= 4) {
        sr->registerValues[0] = value & 0xFF;
        sr->registerValues[1] = (value >> 8) & 0xFF;
        sr->registerValues[2] = (value >> 16) & 0xFF;
        sr->registerValues[3] = (value >> 24) & 0xFF;
    }
}

// Получить 16-битное значение
uint16_t sr_get_value_16(ShiftRegister74HC595 *sr)
{
    if (sr->numRegisters >= 2) {
        return MAKE_WORD(sr->registerValues[1], sr->registerValues[0]);
    }
    return sr->registerValues[0];
}

// Получить количество регистров
uint8_t sr_get_num_registers(ShiftRegister74HC595 *sr)
{
    return sr->numRegisters;
}

// Получить общее количество битов
uint16_t sr_get_total_bits(ShiftRegister74HC595 *sr)
{
    return sr->numRegisters * 8;
}

// Вывести состояние всех регистров в Serial
void sr_print_state(ShiftRegister74HC595 *sr)
{
    Serial.print("74HC595 State [");
    Serial.print(sr->numRegisters);
    Serial.print(" registers]: ");

    for (int8_t reg = sr->numRegisters - 1; reg >= 0; reg--) {
        PRINT_BITS8(sr->registerValues[reg]);
        if (reg > 0)
            Serial.print(" ");
    }
    Serial.println();
}

// Бегущий бит через все регистры
void sr_running_bit(ShiftRegister74HC595 *sr, uint16_t delayMs)
{
    for (uint16_t bit = 0; bit < sr_get_total_bits(sr); bit++) {
        sr_clear(sr);
        sr_set_absolute_bit(sr, bit);
        sr_update(sr);
        delay(delayMs);
    }
}

// Двоичный счетчик
void sr_binary_counter(ShiftRegister74HC595 *sr, uint16_t maxCount, uint16_t delayMs)
{
    for (uint16_t count = 0; count < maxCount; count++) {
        sr_set_value_16(sr, count);
        sr_update(sr);
        delay(delayMs);
    }
}

// Заполнение светодиодов (прогресс-бар)
void sr_progress_bar(ShiftRegister74HC595 *sr, uint16_t delayMs)
{
    for (uint16_t bit = 0; bit < sr_get_total_bits(sr); bit++) {
        sr_set_absolute_bit(sr, bit);
        sr_update(sr);
        delay(delayMs);
    }
}

// Эффект "дыхания" - плавное заполнение и опустошение
void sr_breathing_effect(ShiftRegister74HC595 *sr, uint16_t delayMs)
{
    // Заполнение
    for (uint16_t bit = 0; bit < sr_get_total_bits(sr); bit++) {
        sr_set_absolute_bit(sr, bit);
        sr_update(sr);
        delay(delayMs);
    }
    // Опустошение
    for (int16_t bit = sr_get_total_bits(sr) - 1; bit >= 0; bit--) {
        sr_clear_absolute_bit(sr, bit);
        sr_update(sr);
        delay(delayMs);
    }
}
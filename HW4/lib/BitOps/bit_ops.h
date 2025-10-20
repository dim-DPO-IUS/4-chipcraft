#ifndef BIT_OPS_H
#define BIT_OPS_H

#include <stdint.h>

// ==================== ОСНОВНЫЕ ОПЕРАЦИИ ====================

// Установить бит (bit set)
#define BIT_SET(value, bit) ((value) |= (1UL << (bit)))

// Сбросить бит (bit clear)
#define BIT_CLEAR(value, bit) ((value) &= ~(1UL << (bit)))

// Переключить бит (bit toggle)
#define BIT_TOGGLE(value, bit) ((value) ^= (1UL << (bit)))

// Прочитать бит (bit read)
#define BIT_READ(value, bit) (((value) >> (bit)) & 0x01)

// Записать значение в бит (bit write)
#define BIT_WRITE(value, bit, val) ((val) ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))

// ==================== РАБОТА С ГРУППАМИ БИТОВ ====================

// Установить несколько битов (bit mask set)
#define BITS_SET(value, mask) ((value) |= (mask))

// Сбросить несколько битов (bit mask clear)
#define BITS_CLEAR(value, mask) ((value) &= ~(mask))

// Записать значение в группу битов
#define BITS_WRITE(value, mask, val) ((value) = ((value) & ~(mask)) | ((val) & (mask)))

// Прочитать группу битов
#define BITS_READ(value, mask) ((value) & (mask))

// ==================== БИТОВЫЕ МАСКИ ====================

// Создать битовую маску из n битов, начиная с позиции pos
#define BITMASK(length, position) (((1UL << (length)) - 1) << (position))

// Маска для одного бита
#define SINGLE_BIT(bit) (1UL << (bit))

// Маска для всех битов указанной ширины
#define ALL_BITS(width) ((1UL << (width)) - 1)

// ==================== ОПЕРАЦИИ С ЗНАЧЕНИЯМИ ====================

// Записать значение
#define VALUE_WRITE(dest, src) ((dest) = (src))

// Прочитать значение
#define VALUE_READ(value) (value)

// Очистить значение (все биты в 0)
#define VALUE_CLEAR(value) ((value) = 0)

// Установить все биты в 1 для указанной ширины
#define VALUE_SET_ALL(value, width) ((value) = ALL_BITS(width))

// ==================== ПРОВЕРКИ И УСЛОВИЯ ====================

// Проверить установлен ли бит
#define BIT_IS_SET(value, bit) (BIT_READ(value, bit))

// Проверить сброшен ли бит
#define BIT_IS_CLEAR(value, bit) (!BIT_READ(value, bit))

// Проверить установлена ли маска битов
#define BITS_ARE_SET(value, mask) (((value) & (mask)) == (mask))

// Проверить сброшена ли маска битов
#define BITS_ARE_CLEAR(value, mask) (((value) & (mask)) == 0)

// Проверить установлен ли любой бит из маски
#define ANY_BIT_SET(value, mask) (((value) & (mask)) != 0)

// ==================== БИТОВЫЕ ПОЛЯ ====================

// Записать значение в битовое поле
#define FIELD_WRITE(value, mask, pos, val) \
    BITS_WRITE(value, mask, ((val) << (pos)))

// Прочитать значение из битового поля
#define FIELD_READ(value, mask, pos) \
    (((value) & (mask)) >> (pos))

// ==================== БИТОВАЯ АРИФМЕТИКА ====================

// Подсчет количества установленных битов (popcount)
#define BIT_COUNT(value)                                                           \
    (((value) & 0x01) + (((value) >> 1) & 0x01) + (((value) >> 2) & 0x01) +        \
     (((value) >> 3) & 0x01) + (((value) >> 4) & 0x01) + (((value) >> 5) & 0x01) + \
     (((value) >> 6) & 0x01) + (((value) >> 7) & 0x01))

// Найти позицию младшего установленного бита (LSB)
#define BIT_POS_LSB(value)                               \
    (BIT_IS_SET(value, 0) ? 0 : BIT_IS_SET(value, 1) ? 1 \
                            : BIT_IS_SET(value, 2)   ? 2 \
                            : BIT_IS_SET(value, 3)   ? 3 \
                            : BIT_IS_SET(value, 4)   ? 4 \
                            : BIT_IS_SET(value, 5)   ? 5 \
                            : BIT_IS_SET(value, 6)   ? 6 \
                            : BIT_IS_SET(value, 7)   ? 7 \
                                                     : -1)

// Циклический сдвиг влево
#define ROTATE_LEFT(value, n, width) \
    (((value) << (n)) | ((value) >> ((width) - (n)))) & ALL_BITS(width)

// Циклический сдвиг вправо
#define ROTATE_RIGHT(value, n, width) \
    (((value) >> (n)) | ((value) << ((width) - (n)))) & ALL_BITS(width)

// Извлечь байт из значения (0 = младший байт)
#define GET_BYTE(value, n) (((value) >> (8 * (n))) & 0xFF)

// Объединить байты в значение
#define MAKE_WORD(high, low) (((high) << 8) | (low))

// ==================== ДВОИЧНЫЕ КОНСТАНТЫ ====================

// Двоичные литералы
#define B8(binary) ((uint8_t)0b##binary)
#define B16(binary) ((uint16_t)0b##binary)
#define B32(binary) ((uint32_t)0b##binary)

// ==================== ДЛЯ РАБОТЫ С АППАРАТУРОЙ ====================

// Отправить байт в сдвиговый регистр (MSB first)
#define SHIFT_OUT_MSB(dataPin, clockPin, value)      \
    for (int8_t i = 7; i >= 0; i--) {                \
        digitalWrite((dataPin), BIT_READ(value, i)); \
        digitalWrite((clockPin), HIGH);              \
        digitalWrite((clockPin), LOW);               \
    }

// Отправить байт в сдвиговый регистр (LSB first)
#define SHIFT_OUT_LSB(dataPin, clockPin, value)      \
    for (uint8_t i = 0; i < 8; i++) {                \
        digitalWrite((dataPin), BIT_READ(value, i)); \
        digitalWrite((clockPin), HIGH);              \
        digitalWrite((clockPin), LOW);               \
    }

// ==================== ДЕБАГ И ВЫВОД ====================

// Вывести битовое представление числа
#define PRINT_BITS8(value)                    \
    do {                                      \
        Serial.print("0b");                   \
        for (int8_t i = 7; i >= 0; i--) {     \
            Serial.print(BIT_READ(value, i)); \
        }                                     \
    } while (0)

#define PRINT_BITS16(value)                   \
    do {                                      \
        Serial.print("0b");                   \
        for (int8_t i = 15; i >= 0; i--) {    \
            Serial.print(BIT_READ(value, i)); \
            if (i == 8)                       \
                Serial.print(" ");            \
        }                                     \
    } while (0)

#define PRINT_BITS32(value)                   \
    do {                                      \
        Serial.print("0b");                   \
        for (int8_t i = 31; i >= 0; i--) {    \
            Serial.print(BIT_READ(value, i)); \
            if (i == 24 || i == 16 || i == 8) \
                Serial.print(" ");            \
        }                                     \
    } while (0)

#endif // BIT_OPS_H
#include "encoder.h"

// Инициализация энкодера
void encoder_init(Encoder *enc, uint8_t clk_pin, uint8_t dt_pin, uint8_t sw_pin)
{
    enc->clk_pin = clk_pin;
    enc->dt_pin = dt_pin;
    enc->sw_pin = sw_pin;
    enc->value = 0;
    enc->last_value = 0;

    // Настройка пинов
    pinMode(enc->clk_pin, INPUT_PULLUP);
    pinMode(enc->dt_pin, INPUT_PULLUP);
    pinMode(enc->sw_pin, INPUT_PULLUP);

    // Инициализация начального состояния с помощью битовых операций
    enc->state = 0;
    BIT_WRITE(enc->state, 0, digitalRead(enc->clk_pin));
    BIT_WRITE(enc->state, 1, digitalRead(enc->dt_pin));
}

// Обработка энкодера БЕЗ антидребезга (для аппаратной фильтрации)
void encoder_process(Encoder *enc)
{
    // Читаем текущее состояние напрямую
    uint8_t current_state = 0;
    BIT_WRITE(current_state, 0, digitalRead(enc->clk_pin));
    BIT_WRITE(current_state, 1, digitalRead(enc->dt_pin));

    // Обнаруживаем переход HIGH → LOW только на CLK (бит 0)
    if (BIT_IS_SET(enc->state, 0) && BIT_IS_CLEAR(current_state, 0)) {
        // Определяем направление по состоянию DT (бит 1)
        if (BIT_IS_SET(current_state, 1)) {
            enc->value++;
        } else {
            enc->value--;
        }
    }

    enc->state = current_state;
}

// Обработка энкодера С антидребезгом (внешние фильтры)
void encoder_process_debounced(Encoder *enc, bool clk_stable, bool dt_stable)
{
    uint8_t current_state = 0;
    BIT_WRITE(current_state, 0, clk_stable);
    BIT_WRITE(current_state, 1, dt_stable);

    // Обнаруживаем переход HIGH → LOW только на CLK (бит 0)
    if (BIT_IS_SET(enc->state, 0) && BIT_IS_CLEAR(current_state, 0)) {
        // Определяем направление по состоянию DT (бит 1)
        if (BIT_IS_SET(current_state, 1)) {
            enc->value++;
        } else {
            enc->value--;
        }
    }

    enc->state = current_state;
}

// Проверка изменения значения
bool encoder_changed(Encoder *enc)
{
    if (enc->value != enc->last_value) {
        enc->last_value = enc->value;
        return true;
    }
    return false;
}

// Сброс значения энкодера
void encoder_reset(Encoder *enc)
{
    enc->value = 0;
    enc->last_value = 0;
}

// Проверка нажатия кнопки энкодера
// button_handled - защита от многократной обработки ОДНОГО нажатия
// millis() - last_press_time > 300 - защита от СЛИШКОМ ЧАСТЫХ нажатий
bool encoder_button_pressed(Encoder *enc)
{
    static bool button_handled = true;        // Флаг обработки нажатия
    static unsigned long last_press_time = 0; // Время последнего нажатия

    if (digitalRead(enc->sw_pin) == LOW) {
        // Кнопка нажата - обрабатываем
        if (button_handled && (millis() - last_press_time > 300)) {
            // Условия выполнены!
            button_handled = false;     // Запрещаем повторную обработку
            last_press_time = millis(); // Запоминаем время нажатия
            return true;                // Сообщаем о нажатии
        }
    } else {
        button_handled = true; // Сбрасываем флаг, когда кнопка отпущена
    }

    return false;
}
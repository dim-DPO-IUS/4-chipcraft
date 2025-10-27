#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

// ==================== ФИЛЬТР СКОЛЬЗЯЩЕГО ОКНА ====================
/* ПРИМЕНЕНИЕ:
Объявление:
WindowFilter clk_filter;
WindowFilter dt_filter;
WindowFilter btn_filter;

void setup(){
    // Инициализируем фильтры скользящего окна
    // period_ms=2, window_size=4 (быстрее чем 8)
    clk_filter = WINDOW_FILTER_INIT(2, 4);
    dt_filter = WINDOW_FILTER_INIT(2, 4);
    btn_filter = WINDOW_FILTER_INIT(5, 6); // Для кнопки побольше окно

    // Принудительно устанавливаем начальное состояние (все светодиоды выключены)
    sr_clear(&sr_cascade);
    sr_update(&sr_cascade);
}

void loop()
{
    // Обновляем фильтры скользящего окна
    int8_t clk_result = window_filter(&clk_filter, digitalRead(my_encoder.clk_pin));
    int8_t dt_result = window_filter(&dt_filter, digitalRead(my_encoder.dt_pin));

    // Используем стабильные состояния только если они определились
    bool clk_stable = (clk_result == 1); // 1 = стабильно HIGH, 0 = стабильно LOW
    bool dt_stable = (dt_result == 1);

    // Обработка энкодера только при стабильных сигналах
    if (clk_result >= 0 && dt_result >= 0) { // -1 = переходное состояние, -2 = время не пришло
        encoder_process_debounced(&my_encoder, clk_stable, dt_stable);
    }
}
*/

// Структура для фильтра скользящего окна
typedef struct
{
    uint8_t state;             // Битовое окно состояний
    uint32_t last_sample_time; // Время последнего отсчета
    uint16_t sample_period;    // Период опроса (мс)
    uint8_t window_size;       // Размер окна (обычно 8)
} WindowFilter;

// Инициализация фильтра
#define WINDOW_FILTER_INIT(period_ms, size) { \
    .state = 0xFF,                            \
    .last_sample_time = 0,                    \
    .sample_period = (period_ms),             \
    .window_size = (size)}

// Функция фильтрации скользящим окном
int8_t window_filter(WindowFilter *filter, bool current_state);

// ==================== ПРОСТОЙ АНТИДРЕБЕЗГ ====================
/* ПРИМЕНЕНИЕ:

Объявление:
DebounceSimple clk_filter;
DebounceSimple dt_filter;
DebounceSimple btn_filter;

void setup(){
    // Инициализируем фильтры
    clk_filter = DEBOUNCE_SIMPLE_INIT(5);
    dt_filter = DEBOUNCE_SIMPLE_INIT(5);
    btn_filter = DEBOUNCE_SIMPLE_INIT(50);
}

void loop()
{
    // Обновляем простые фильтры
    debounce_simple(&clk_filter, digitalRead(my_encoder.clk_pin));
    debounce_simple(&dt_filter, digitalRead(my_encoder.dt_pin));

    // Используем стабильные состояния
    encoder_process_debounced(&my_encoder,
                              clk_filter.stable_state,
                              dt_filter.stable_state);
}
*/

// Структура для простого антидребезга
typedef struct
{
    bool last_state;
    bool stable_state;
    unsigned long last_change_time;
    unsigned long debounce_delay;
} DebounceSimple;

// Инициализация простого фильтра (начинаем с HIGH состояния)
// .last_state = true, \           // Начинаем с HIGH
// .stable_state = true, \         // Начинаем с HIGH
#define DEBOUNCE_SIMPLE_INIT(delay_ms) { \
    .last_state = true,                  \
    .stable_state = true,                \
    .last_change_time = 0,               \
    .debounce_delay = (delay_ms)}

// Функция простого антидребезга
bool debounce_simple(DebounceSimple *filter, bool current_state);

// ==================== УНИВЕРСАЛЬНЫЙ АНТИДРЕБЕЗГ ====================

// Универсальная структура антидребезга на основе скользящего окна
typedef struct
{
    WindowFilter filter;    // Основной фильтр скользящего окна
    bool last_stable_state; // Последнее стабильное состояние
    bool edge_detected;     // Флаг обнаружения фронта
} DebounceAdvanced;

// Инициализация расширенного фильтра
#define DEBOUNCE_ADVANCED_INIT(period_ms) {     \
    .filter = WINDOW_FILTER_INIT(period_ms, 8), \
    .last_stable_state = false,                 \
    .edge_detected = false}

// Универсальная функция антидребезга с детекцией фронтов
bool debounce_advanced(DebounceAdvanced *db, bool current_state);

// Детектор фронта (возвращает true только при изменении)
bool debounce_edge(DebounceAdvanced *db, bool current_state);

// Детектор нажатия (LOW активная кнопка)
bool debounce_falling_edge(DebounceAdvanced *db, bool current_state);

// Детектор отпускания (LOW активная кнопка)
bool debounce_rising_edge(DebounceAdvanced *db, bool current_state);

#endif // DEBOUNCE_H
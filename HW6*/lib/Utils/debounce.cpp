#include "debounce.h"

// ==================== ФИЛЬТР СКОЛЬЗЯЩЕГО ОКНА ====================

int8_t window_filter(WindowFilter *filter, bool current_state)
{
    uint32_t current_time = millis();

    // Если пришло время нового отсчета
    if (current_time - filter->last_sample_time >= filter->sample_period) {
        filter->last_sample_time = current_time;

        // Сдвигаем окно и добавляем новое значение
        filter->state = (filter->state << 1) | (current_state ? 1 : 0);

        // Маска для актуальных битов (убираем старые)
        uint8_t mask = (1 << filter->window_size) - 1;
        filter->state &= mask;

        // Анализируем состояние окна
        if (filter->state == mask) {
            return 1; // Все биты = 1 - стабильно HIGH
        } else if (filter->state == 0) {
            return 0; // Все биты = 0 - стабильно LOW
        } else {
            return -1; // Переходное состояние
        }
    }

    return -2; // Время еще не пришло, состояние не изменилось
}

// ==================== ПРОСТОЙ АНТИДРЕБЕЗГ ====================

bool debounce_simple(DebounceSimple *filter, bool current_state)
{
    unsigned long current_time = millis();

    // Если состояние изменилось
    if (current_state != filter->last_state) {
        filter->last_change_time = current_time;
        filter->last_state = current_state;
    }

    // Если состояние стабильно в течение debounce_delay
    if ((current_time - filter->last_change_time) > filter->debounce_delay) {
        if (current_state != filter->stable_state) {
            filter->stable_state = current_state;
            return true; // Состояние изменилось и стабилизировалось
        }
    }

    return false; // Состояние не изменилось или еще дребезжит
}

// ==================== УНИВЕРСАЛЬНЫЙ АНТИДРЕБЕЗГ ====================

bool debounce_advanced(DebounceAdvanced *db, bool current_state)
{
    // 1. Используем базовый WindowFilter
    int8_t filtered = window_filter(&db->filter, current_state);

    if (filtered >= 0) { // Есть стабильное состояние
        bool new_stable_state = (filtered == 1);

        // 2. Автоматически детектируем фронты
        if (new_stable_state != db->last_stable_state) {
            db->last_stable_state = new_stable_state;
            db->edge_detected = true;
            return true; // Состояние изменилось
        }
    }

    db->edge_detected = false;
    return false; // Состояние не изменилось
}

bool debounce_edge(DebounceAdvanced *db, bool current_state)
{
    if (debounce_advanced(db, current_state)) {
        return db->edge_detected;
    }
    return false;
}

bool debounce_falling_edge(DebounceAdvanced *db, bool current_state)
{
    if (debounce_advanced(db, current_state) && db->edge_detected) {
        return !db->last_stable_state; // Переход 1→0
    }
    return false;
}

bool debounce_rising_edge(DebounceAdvanced *db, bool current_state)
{
    if (debounce_advanced(db, current_state) && db->edge_detected) {
        return db->last_stable_state; // Переход 0→1
    }
    return false;
}
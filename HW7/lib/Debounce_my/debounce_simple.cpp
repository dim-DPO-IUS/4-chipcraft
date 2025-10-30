#include "debounce_simple.h"

bool debounce_check(uint16_t read_period, bool current_value, bool target_state)
{
    static uint8_t value_state = 0xFF; // Начинаем с true состояния
    static uint32_t last_read_time = 0;

    // Если пришло время нового отсчета
    if (millis() - last_read_time >= read_period) {
        last_read_time = millis();

        // Сдвигаем окно и добавляем новое значение
        value_state = (value_state << 1) | (current_value ? 1 : 0);

        // ВЫВОДИМ ИНФОРМАЦИЮ
        // Serial.print("debounce_check: state=0b");
        // for (int i = 7; i >= 0; i--) {
        //     Serial.print(bitRead(value_state, i));
        // }
        // Serial.print(" current=");
        // Serial.print(current_value);
        // Serial.print(" target=");
        // Serial.print(target_state);

        // Проверяем, все ли биты соответствуют целевому состоянию
        if (target_state) {
            bool result = (value_state == 0xFF);
            // Serial.print(" result=");
            // Serial.println(result ? "STABLE_TRUE" : "UNSTABLE");
            return result; // Все биты = 1 (target_state = true)
        } else {
            bool result = (value_state == 0x00);
            // Serial.print(" result=");
            // Serial.println(result ? "STABLE_FALSE" : "UNSTABLE");
            return result; // Все биты = 0 (target_state = false)
        }
    } else {
        // Время еще не пришло
        // Serial.print("debounce_check: WAITING (");
        // Serial.print(millis() - last_read_time);
        // Serial.print("/");
        // Serial.print(read_period);
        // Serial.println("ms)");
    }

    return false; // Время еще не пришло
}
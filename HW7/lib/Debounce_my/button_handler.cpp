#include "button_handler.h"

bool handle_button(uint8_t pin, void (*on_press)())
{
    static bool ignore_period = true;

    // Если не в периоде игнорирования - проверяем кнопку
    if (!ignore_period && debounce_check(10, digitalRead(pin), false)) {
        if (on_press)
            on_press();
        ignore_period = true;
        return true;
    }

    // Проверяем, закончился ли период игнорирования
    if (ignore_period && debounce_check(10, true, true)) {
        ignore_period = false;
    }

    return false;
}
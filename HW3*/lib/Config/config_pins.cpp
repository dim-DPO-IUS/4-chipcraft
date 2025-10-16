#include "config_pins.h"

// Определяем структуры пинов
DirectPin ntc_pin;
DirectPin led_pwm_pin;

void init_all_pins()
{
    // Просто указываем ардуино-пины и режимы

    // Потенциометр на A0
    pin_init_arduino(&ntc_pin, NTC_PIN, INPUT);

    // Светодиод на D6 (ШИМ)
    pin_init_arduino(&led_pwm_pin, LED_PWM_PIN, OUTPUT);
}
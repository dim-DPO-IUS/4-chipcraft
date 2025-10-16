#ifndef CONFIG_PINS_H
#define CONFIG_PINS_H

#include "pin_io.h"
#include "pin_mapping.h" // Здесь все константы A0, D6 и т.д.

//
#define POTENTIOMETER_PIN A0
#define LED_PWM_PIN D6

void init_all_pins();

#endif // CONFIG_PINS_H
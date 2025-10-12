#include <Arduino.h>

const uint8_t buttonPin = 2;
const uint8_t ledPin = 13;
void setup()
{
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
}

int8_t debounce1(int state_period, const uint8_t pin)
{
    // Состояние кнопки; каждый бит соответствует состоянию входа в очередной момент времени
    static uint8_t state = 0;
    // здесь храним момент последнего отсчёта
    static uint32_t ts = 0;
    // Если с момента последнего отсчёта прошло более state_period миллисекунд
    if (millis() - ts > state_period) {
        ts = millis(); // Запоминам этот момент
        // сдвигаем текущее состояние на один бит влево, «забывая» самое старое значение
        state <<= 1; // И в крайний правый бит записываем текущий отсчёт
        state |= digitalRead(pin);
    }

    switch (state) {
    // то есть все биты «1» — последние 8 отсчётов кнопка была отпущена
    case 0xff:
        // Кнопка отпущена в течение минимум (state_period * 8) миллисекунд
        return 1;
        break;
    // то есть все биты «0» — последние 8 отсчётов кнопка была нажата
    case 0x00:
        // Кнопка нажата в течение минимум (state_period * 8) миллисекунд
        return 0;
        break;
    default:
        // Кнопка в процессе переключения
        return -1;
        break;
    }
}

void loop()
{
    int btnState = debounce1(10, buttonPin);
    Serial.println(btnState);

    if (btnState >= 0)
        btnState > 0 ? digitalWrite(ledPin, LOW) : digitalWrite(ledPin, HIGH);
}

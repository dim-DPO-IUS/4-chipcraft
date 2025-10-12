#include "AnalogFilter.h" // библиотека фильтров
#include <Arduino.h>

const int photoPin = A0;

// Объявляем фильтр
MovingAverageFilter avgFilter;

// Структура одной ножки МК
typedef struct
{
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    uint8_t bit; // номер ножки МК
} PinConfig;

// Массив структур ножек МК для светодиодов
const PinConfig leds[] = {
    {&DDRE, &PORTE, 4}, // D2
    {&DDRE, &PORTE, 5}, // D3
    {&DDRG, &PORTG, 5}, // D4
    {&DDRE, &PORTE, 3}, // D5
    {&DDRH, &PORTH, 3}, // D6
};

const int numLeds = 5;
byte currentPattern = 0;

void setup()
{
    Serial.begin(9600);

    // Инициализация фильтра
    initMovingAverage(&avgFilter, 12);

    // Инициализация пинов
    for (int i = 0; i < numLeds; i++) {
        *leds[i].ddr |= (1 << leds[i].bit);
        *leds[i].port &= ~(1 << leds[i].bit);
    }
}

void setLed(int ledIndex, bool state)
{
    volatile uint8_t *port = leds[ledIndex].port;
    uint8_t bitMask = (1 << leds[ledIndex].bit);
    state ? *port |= bitMask : *port &= ~bitMask;
}

void updateLeds(byte pattern)
{
    for (int i = 0; i < numLeds; i++) {
        setLed(i, (pattern >> i) & 1);
    }
}

void loop()
{
    int photoValue = analogRead(photoPin);

    // Используем функции фильтра
    avgFilter.interface.update(&avgFilter, photoValue);
    int filteredValue = (int)avgFilter.interface.read(&avgFilter);

    /* Границы освещенности
    Реальные границы:
        свет 160
        тень 665
    Усредненные границы:
        свет 320
        тень 620
    */

    // Масштабируем: больше темноты = больше горящих светодиодов
    // filteredValue: MIN (свет) → 0 светодиодов
    // filteredValue: MAX (темнота) → все светодиоды
    // Границы берем чуть шире чем усредненные
    int numLedsOn = map(filteredValue, 300, 650, 0, numLeds * 2);
    numLedsOn = constrain(numLedsOn, 0, numLeds * 2);
    numLedsOn /= 2; // 5 светодиодов

    // Создаем паттерн с несколькими горящими светодиодами
    // Например: для 3 светодиодов: (1 << 3) - 1 = 0b00000111
    byte newPattern = (1 << numLedsOn) - 1;

    if (newPattern != currentPattern) {
        currentPattern = newPattern;
        updateLeds(currentPattern);

        // Отладочная информация
        Serial.print("Raw: ");
        Serial.print(photoValue);
        Serial.print(" Light: ");
        Serial.print(filteredValue);
        Serial.print(" LEDs on: ");
        Serial.print(numLedsOn);
        Serial.print(" Pattern: 0b");
        for (int i = numLeds - 1; i >= 0; i--) {
            Serial.print((currentPattern >> i) & 1);
        }
        Serial.println();
    }

    delay(50);
}
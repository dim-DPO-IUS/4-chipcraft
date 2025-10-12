#include "AnalogFilter.h" // библиотека фильтров
#include <Arduino.h>

const int potPin = A0;

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
    initMovingAverage(&avgFilter, 8);

    // Быстрая инициализация всех нужных пинов
    for (int i = 0; i < numLeds; i++) {
        *leds[i].ddr |= (1 << leds[i].bit);   // устанавливаем биты в 1
        *leds[i].port &= ~(1 << leds[i].bit); // сбрасываем биты в 0
    }
}

void setLed(int ledIndex, bool state)
{ // Установка состояния одного пина светодиода
    volatile uint8_t *port = leds[ledIndex].port;
    uint8_t bitMask = (1 << leds[ledIndex].bit);
    state ? *port |= bitMask : *port &= ~bitMask; // ON-OFF Led
}

void updateLeds(byte pattern)
{ // Установка состояния группы пинов светодиодов
    for (int i = 0; i < numLeds; i++) {
        setLed(i, (pattern >> i) & 1);
    }
}

void loop()
{
    int potValue = analogRead(potPin);

    // Используем функции фильтра
    avgFilter.interface.update(&avgFilter, potValue);
    int filteredValue = (int)avgFilter.interface.read(&avgFilter);

    // Масштабируем
    byte ledIndex = map(filteredValue, 0, 1023, 0, numLeds);
    ledIndex = constrain(ledIndex, 0, numLeds - 1);

    byte newPattern = 1 << ledIndex;

    if (newPattern != currentPattern) {
        currentPattern = newPattern;
        updateLeds(currentPattern);
        // Отладочная
        Serial.println(ledIndex);
    }

    delay(10);
}
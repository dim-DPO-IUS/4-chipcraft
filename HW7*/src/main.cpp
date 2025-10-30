#include "button_handler.h"
#include "clock_display.h"
#include <Arduino_ST7789_Fast.h>
#include <SPI.h>

#define TFT_DC 7
#define TFT_RST 8
#define BUTTON_PIN 2
#define LED_PIN 4

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);
ClockDisplay clock;

// Переменные для обработки прерываний и дребезга
volatile bool button_irq = false;
volatile unsigned long lastInterruptTime = 0;
volatile unsigned long interruptCount = 0;

// Переменные времени
int seconds = 0, minutes = 0, hours = 10;
unsigned long lastSecondUpdate = 0;
bool clockStopped = false;

void handleInterrupt()
{
    button_irq = true;
    interruptCount++;
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);

    tft.init(240, 240);
    clock_init(&clock, &tft);
}

void loop()
{
    // Обработка кнопки с антидребезгом + обработка флага прерываний
    handle_button(BUTTON_PIN, []() {
        // Остановка часов по флагу прерывания
        if (button_irq) {
            button_irq = false;
            clockStopped = !clockStopped;
            clock_setStopped(&clock, clockStopped);
            digitalWrite(LED_PIN, clockStopped ? HIGH : LOW);
        }
    });

    // Обновление времени раз в секунду
    if (!clockStopped && millis() - lastSecondUpdate >= 1000) {
        lastSecondUpdate = millis();
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours = (hours + 1) % 12;
            }
        }
        clock_updateTime(&clock, hours, minutes, seconds);
    }
}
#include "clock_display.h"

void clock_init(ClockDisplay *clock, Arduino_ST7789 *tft)
{
    clock->tft = tft;
    clock->clockStopped = false;
    clock->stopStartTime = 0;
    clock->lastSecX = clock->lastSecY = 0;
    clock->lastMinX = clock->lastMinY = 0;
    clock->lastHourX = clock->lastHourY = 0;

    clock_drawStatic(clock);
}

void clock_drawStatic(ClockDisplay *clock)
{
    clock->tft->fillScreen(BLACK);

    // Заголовок
    clock->tft->setTextSize(2);
    clock->tft->setTextColor(CYAN);
    clock->tft->setCursor(50, 10);
    clock->tft->print("CLOCK DEMO");

    // Циферблат
    clock->tft->drawCircle(120, 100, 50, WHITE);
    clock->tft->drawCircle(120, 100, 49, WHITE);

    // Метки часов
    for (int i = 0; i < 12; i++) {
        float angle = i * M_PI / 6;
        int x1 = 120 + 45 * cos(angle);
        int y1 = 100 + 45 * sin(angle);
        int x2 = 120 + 40 * cos(angle);
        int y2 = 100 + 40 * sin(angle);
        clock->tft->drawLine(x1, y1, x2, y2, WHITE);
    }

    // Центр
    clock->tft->fillCircle(120, 100, 4, RED);

    // Инструкция
    clock->tft->setTextSize(1);
    clock->tft->setTextColor(WHITE);
    clock->tft->setCursor(60, 170);
    clock->tft->print("Press button");
    clock->tft->setCursor(70, 185);
    clock->tft->print("to STOP clock");
}

static void clock_updateHands(ClockDisplay *clock, int hours, int minutes, int seconds)
{
    // Вычисляем углы
    float secAngle = 2 * M_PI * seconds / 60;
    float minAngle = 2 * M_PI * minutes / 60;
    float hourAngle = 2 * M_PI * (hours % 12) / 12 + minAngle / 12;

    // Вычисляем новые позиции стрелок
    int secX = 120 + 40 * cos(secAngle - M_PI / 2);
    int secY = 100 + 40 * sin(secAngle - M_PI / 2);
    int minX = 120 + 35 * cos(minAngle - M_PI / 2);
    int minY = 100 + 35 * sin(minAngle - M_PI / 2);
    int hourX = 120 + 25 * cos(hourAngle - M_PI / 2);
    int hourY = 100 + 25 * sin(hourAngle - M_PI / 2);

    // Стираем старые стрелки
    if (clock->lastSecX != 0)
        clock->tft->drawLine(120, 100, clock->lastSecX, clock->lastSecY, BLACK);
    if (clock->lastMinX != 0)
        clock->tft->drawLine(120, 100, clock->lastMinX, clock->lastMinY, BLACK);
    if (clock->lastHourX != 0)
        clock->tft->drawLine(120, 100, clock->lastHourX, clock->lastHourY, BLACK);

    // Рисуем новые стрелки
    clock->tft->drawLine(120, 100, hourX, hourY, WHITE);
    clock->tft->drawLine(120, 100, minX, minY, WHITE);
    clock->tft->drawLine(120, 100, secX, secY, RED);

    // Сохраняем позиции
    clock->lastSecX = secX;
    clock->lastSecY = secY;
    clock->lastMinX = minX;
    clock->lastMinY = minY;
    clock->lastHourX = hourX;
    clock->lastHourY = hourY;
}

void clock_updateTime(ClockDisplay *clock, int hours, int minutes, int seconds)
{
    if (!clock->clockStopped) {
        clock_updateHands(clock, hours, minutes, seconds);

        // Цифровое время
        clock->tft->fillRect(90, 150, 60, 15, BLACK);
        clock->tft->setTextColor(WHITE);
        clock->tft->setCursor(90, 150);
        clock->tft->print(hours);
        clock->tft->print(":");
        if (minutes < 10)
            clock->tft->print("0");
        clock->tft->print(minutes);
        clock->tft->print(":");
        if (seconds < 10)
            clock->tft->print("0");
        clock->tft->print(seconds);
    }
}

void clock_setStopped(ClockDisplay *clock, bool stopped)
{
    clock->clockStopped = stopped;
    if (stopped) {
        clock->stopStartTime = millis();
    }
}

void clock_drawStatus(ClockDisplay *clock)
{
    clock->tft->fillRect(70, 200, 100, 20, BLACK);
    clock->tft->setTextColor(clock->clockStopped ? RED : GREEN);
    clock->tft->setCursor(75, 203);

    if (clock->clockStopped) {
        if ((millis() - clock->stopStartTime) % 500 < 250) {
            clock->tft->print("STOPPED!");
        }
    } else {
        clock->tft->print("RUNNING");
    }
}
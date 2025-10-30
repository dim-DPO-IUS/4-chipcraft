#include "display.h"

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);
static bool display_ready_flag = false;

void display_init(void)
{
    tft.init(240, 240);
    tft.fillScreen(BLACK);
    tft.setTextWrap(false);
}

void display_update(void)
{
    static bool firstDraw = true;

    if (firstDraw) {
        tft.fillScreen(BLACK);

        // Статические элементы
        tft.setTextSize(HEADER_TEXT_SIZE);
        tft.setTextColor(LABEL_COLOR);

        tft.setCursor(TEMP_LABEL_X, TEMP_LABEL_Y);
        tft.print("TEMPERATURE:");

        tft.setCursor(HUM_LABEL_X, HUM_LABEL_Y);
        tft.print("HUMIDITY:");

        tft.drawFastHLine(LINE_X1, LINE_Y, LINE_LENGTH, LINE_COLOR);

        firstDraw = false;
    }

    // Динамические значения
    tft.setTextSize(VALUE_TEXT_SIZE);

    // Температура
    tft.setTextColor(TEMP_COLOR, BLACK);
    tft.setCursor(TEMP_VALUE_X, TEMP_VALUE_Y);
    tft.print(sensor_get_temp(), 1);
    tft.print(" C  ");

    // Влажность
    tft.setTextColor(HUM_COLOR, BLACK);
    tft.setCursor(HUM_VALUE_X, HUM_VALUE_Y);
    tft.print(sensor_get_hum(), 1);
    tft.print(" %  ");

    // Текстовый индикатор обновления (текст: + и *)
    // static bool indicator = false;
    // tft.setTextSize(INDICATOR_TEXT_SIZE);
    // tft.setTextColor(INDICATOR_COLOR, BLACK);
    // tft.setCursor(INDICATOR_X, INDICATOR_Y);
    // tft.print(indicator ? "+" : "*");
    // indicator = !indicator;

    // Графический индикатор обновления (кружок) вместо текста
    // Графический индикатор (пульсирующий кружок)
    static bool indicator = false;
    if (indicator) {
        tft.fillCircle(INDICATOR_CENTER_X,
                       INDICATOR_CENTER_Y,
                       INDICATOR_RADIUS,
                       INDICATOR_COLOR);
    } else {
        tft.fillCircle(INDICATOR_CENTER_X,
                       INDICATOR_CENTER_Y,
                       INDICATOR_RADIUS,
                       BLACK);
        tft.drawCircle(INDICATOR_CENTER_X,
                       INDICATOR_CENTER_Y,
                       INDICATOR_RADIUS,
                       INDICATOR_COLOR);
    }
    indicator = !indicator;
}

bool display_ready(void)
{
    return display_ready_flag;
}

void display_set_ready(bool state)
{
    display_ready_flag = state;
}
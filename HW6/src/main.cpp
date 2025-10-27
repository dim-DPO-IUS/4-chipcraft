#define TFT_DC 7
#define TFT_RST 8
#define SCR_WD 240
#define SCR_HT 240

#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#include <DallasTemperature.h>
#include <SPI.h>

// ---------------------------------------------------
#define ONE_WIRE_BUS 2
#define TEMP1_X 30
#define TEMP1_Y 45 // Сместили выше из-за уменьшения рамки
#define TEMP2_X 30
#define TEMP2_Y 115 // Сместили выше из-за уменьшения рамки

// Определяем свои цвета если нужно
#define DARKGREY 0x4208  // Темно-серый
#define LIGHTGREY 0xC618 // Светло-серый

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ---------------------------------------------------

void deletePreviousTemp1();
void deletePreviousTemp2();
void printTemp1();
void printTemp2();
void printMinTemp1();
void printMaxTemp1();
void printMinTemp2();
void printMaxTemp2();
void deleteMinTemp1();
void deleteMaxTemp1();
void deleteMinTemp2();
void deleteMaxTemp2();
void printError(int sensorNumber);
bool isValidTemperature(float temp);
void checkSensors();
void drawTemperatureBackground();
void printFormattedTemp(float temp, int x, int y, uint16_t color);

// ---------------------------------------------------

float previousTemp1 = -100.0;
float previousTemp2 = -100.0;
float temp1C = 0;
float temp2C = 0;
float minTemp1 = 200;
float maxTemp1 = 0;
float minTemp2 = 200;
float maxTemp2 = 0;
bool sensor1OK = false;
bool sensor2OK = false;

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);

void setup()
{
    Serial.begin(9600);
    Serial.println("Starting up ...");
    sensors.begin();

    delay(1000);
    checkSensors();

    //---------------------------
    tft.init(SCR_WD, SCR_HT);
    //---------------------------
    tft.fillScreen(BLACK);

    // Рисуем красивый фон
    drawTemperatureBackground();

    // Заголовки для датчиков
    tft.setCursor(15, 10);
    tft.setTextSize(2);
    tft.setTextColor(CYAN);
    tft.println("TEMPERATURE");

    // Подписи датчиков
    tft.setCursor(20, 32);
    tft.setTextSize(1);
    tft.setTextColor(GREEN);
    tft.println("SENSOR 1");

    tft.setCursor(20, 102);
    tft.setTextSize(1);
    tft.setTextColor(BLUE);
    tft.println("SENSOR 2");

    // Символы градусов
    tft.setCursor(180, 45);
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
    tft.println((char)247);
    tft.setCursor(200, 45);
    tft.println("C");

    tft.setCursor(180, 115);
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
    tft.println((char)247);
    tft.setCursor(200, 115);
    tft.println("C");

    // MIN/MAX
    tft.setCursor(30, 160);
    tft.setTextSize(1);
    tft.setTextColor(CYAN);
    tft.println("MIN");

    tft.setCursor(120, 160);
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("MAX");

    tft.setCursor(30, 195);
    tft.setTextSize(1);
    tft.setTextColor(CYAN);
    tft.println("MIN");

    tft.setCursor(120, 195);
    tft.setTextSize(1);
    tft.setTextColor(RED);
    tft.println("MAX");
}

void drawTemperatureBackground()
{
    // Уменьшенные рамки: высота 50 вместо 70
    tft.fillRoundRect(10, 30, 220, 50, 5, DARKGREY);
    tft.drawRoundRect(10, 30, 220, 50, 5, WHITE);

    tft.fillRoundRect(10, 100, 220, 50, 5, DARKGREY);
    tft.drawRoundRect(10, 100, 220, 50, 5, WHITE);

    // Разделительная линия смещена выше
    tft.drawFastHLine(10, 190, 220, LIGHTGREY);
}

void printFormattedTemp(float temp, int x, int y, uint16_t color)
{
    tft.setCursor(x, y);
    tft.setTextSize(4);
    tft.setTextColor(color);

    // Форматируем вывод - всегда показываем 1 знак после запятой
    int intPart = (int)temp;
    int fracPart = (int)(abs(temp) * 10) % 10;

    // Если температура отрицательная
    if (temp < 0) {
        tft.print("-");
        intPart = abs(intPart);
    }

    tft.print(intPart);
    tft.print(".");
    tft.print(fracPart);
}

void loop()
{
    delay(1000);

    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 30000) {
        checkSensors();
        lastCheck = millis();
    }

    sensors.requestTemperatures();

    previousTemp1 = temp1C;
    previousTemp2 = temp2C;

    temp1C = sensors.getTempCByIndex(0);
    temp2C = sensors.getTempCByIndex(1);

    Serial.print("Sensor 1: ");
    Serial.print(temp1C);
    Serial.print(" C, Sensor 2: ");
    Serial.print(temp2C);
    Serial.println(" C");

    // Обновление MIN/MAX
    if (isValidTemperature(temp1C) && sensor1OK) {
        if (temp1C < minTemp1) {
            deleteMinTemp1();
            minTemp1 = temp1C;
        }
        if (temp1C > maxTemp1) {
            deleteMaxTemp1();
            maxTemp1 = temp1C;
        }
    }

    if (isValidTemperature(temp2C) && sensor2OK) {
        if (temp2C < minTemp2) {
            deleteMinTemp2();
            minTemp2 = temp2C;
        }
        if (temp2C > maxTemp2) {
            deleteMaxTemp2();
            maxTemp2 = temp2C;
        }
    }

    // Обновление дисплея
    if (previousTemp1 != temp1C || !sensor1OK) {
        deletePreviousTemp1();
        if (isValidTemperature(temp1C) && sensor1OK) {
            printTemp1();
        } else {
            printError(1);
        }
        printMinTemp1();
        printMaxTemp1();
    }

    if (previousTemp2 != temp2C || !sensor2OK) {
        deletePreviousTemp2();
        if (isValidTemperature(temp2C) && sensor2OK) {
            printTemp2();
        } else {
            printError(2);
        }
        printMinTemp2();
        printMaxTemp2();
    }
}

bool isValidTemperature(float temp)
{
    return (temp > -50.0 && temp < 125.0);
}

void printError(int sensorNumber)
{
    if (sensorNumber == 1) {
        tft.setCursor(TEMP1_X, TEMP1_Y);
        tft.setTextSize(3);
        tft.setTextColor(YELLOW);
        tft.println("Error  ");
    } else {
        tft.setCursor(TEMP2_X, TEMP2_Y);
        tft.setTextSize(3);
        tft.setTextColor(YELLOW);
        tft.println("Error  ");
    }
}

// Функции для датчика 1
void deletePreviousTemp1()
{
    tft.fillRect(TEMP1_X, TEMP1_Y, 140, 40, DARKGREY);
}

void printTemp1()
{
    printFormattedTemp(temp1C, TEMP1_X, TEMP1_Y, GREEN);
}

void printMinTemp1()
{
    tft.setCursor(50, 175);
    tft.setTextSize(2);
    tft.setTextColor(CYAN);
    if (isValidTemperature(minTemp1) && sensor1OK) {
        tft.print(minTemp1, 1);
    } else {
        tft.print("---");
    }
}

void printMaxTemp1()
{
    tft.setCursor(140, 175);
    tft.setTextSize(2);
    tft.setTextColor(RED);
    if (isValidTemperature(maxTemp1) && sensor1OK) {
        tft.print(maxTemp1, 1);
    } else {
        tft.print("---");
    }
}

void deleteMinTemp1()
{
    tft.setCursor(50, 175);
    tft.setTextSize(2);
    tft.setTextColor(DARKGREY);
    tft.print(minTemp1, 1);
}

void deleteMaxTemp1()
{
    tft.setCursor(140, 175);
    tft.setTextSize(2);
    tft.setTextColor(DARKGREY);
    tft.print(maxTemp1, 1);
}

// Функции для датчика 2
void deletePreviousTemp2()
{
    tft.fillRect(TEMP2_X, TEMP2_Y, 140, 40, DARKGREY);
}

void printTemp2()
{
    printFormattedTemp(temp2C, TEMP2_X, TEMP2_Y, BLUE);
}

void printMinTemp2()
{
    tft.setCursor(50, 210);
    tft.setTextSize(2);
    tft.setTextColor(CYAN);
    if (isValidTemperature(minTemp2) && sensor2OK) {
        tft.print(minTemp2, 1);
    } else {
        tft.print("---");
    }
}

void printMaxTemp2()
{
    tft.setCursor(140, 210);
    tft.setTextSize(2);
    tft.setTextColor(RED);
    if (isValidTemperature(maxTemp2) && sensor2OK) {
        tft.print(maxTemp2, 1);
    } else {
        tft.print("---");
    }
}

void deleteMinTemp2()
{
    tft.setCursor(50, 210);
    tft.setTextSize(2);
    tft.setTextColor(DARKGREY);
    tft.print(minTemp2, 1);
}

void deleteMaxTemp2()
{
    tft.setCursor(140, 210);
    tft.setTextSize(2);
    tft.setTextColor(DARKGREY);
    tft.print(maxTemp2, 1);
}

void checkSensors()
{
    int deviceCount = sensors.getDeviceCount();
    Serial.print("Found ");
    Serial.print(deviceCount);
    Serial.println(" devices.");

    sensor1OK = false;
    sensor2OK = false;

    if (deviceCount >= 1) {
        DeviceAddress addr1;
        if (sensors.getAddress(addr1, 0)) {
            sensor1OK = true;
        }
    }

    if (deviceCount >= 2) {
        DeviceAddress addr2;
        if (sensors.getAddress(addr2, 1)) {
            sensor2OK = true;
        }
    }
}
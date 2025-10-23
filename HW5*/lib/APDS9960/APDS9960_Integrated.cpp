#include "APDS9960_Integrated.h"

void writeAPDSRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(APDS9960_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t readAPDSRegister(uint8_t reg)
{
    Wire.beginTransmission(APDS9960_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(APDS9960_ADDR, 1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0;
}

bool APDS9960_Init(APDS9960_Sensor *sensor)
{
    // Инициализируем фильтры
    sensor->isInitialized = false;

    // Проверка подключения
    Wire.beginTransmission(APDS9960_ADDR);
    byte error = Wire.endTransmission();

    if (error != 0) {
        Serial.println("APDS9960 not found on I2C bus!");
        return false;
    }

    // Проверка ID
    uint8_t id = readAPDSRegister(0x92);
    Serial.print("APDS9960 ID: 0x");
    Serial.println(id, HEX);

    if (!(id == 0xAB || id == 0x9C || id == 0xA8)) {
        Serial.println("Invalid APDS9960 ID");
        return false;
    }

    // Инициализация
    writeAPDSRegister(0x80, 0x00); // Все выключить

    // Настройка цвета
    writeAPDSRegister(0x81, 0xFF); // ATIME = 256 циклов
    writeAPDSRegister(0x8F, 0x01); // CONTROL - усиление 4x

    // Настройка приближения
    writeAPDSRegister(0x8E, 0x87); // PPULSE - 16 импульсов
    writeAPDSRegister(0x90, 0x01); // CONFIG2

    // Включение функций
    writeAPDSRegister(0x80, 0x01); // PON
    delay(50);
    writeAPDSRegister(0x80, 0x03); // PON + PEN
    delay(50);
    writeAPDSRegister(0x80, 0x07); // PON + PEN + AEN
    delay(50);

    sensor->isInitialized = true;
    Serial.println("APDS9960 initialized successfully");
    return true;
}

void readAPDSColor(APDS9960_Sensor *sensor)
{
    Wire.beginTransmission(APDS9960_ADDR);
    Wire.write(0x94); // CDATAL
    Wire.endTransmission();

    Wire.requestFrom(APDS9960_ADDR, 8);
    if (Wire.available() == 8) {
        sensor->clear = Wire.read() | (Wire.read() << 8);
        sensor->red = Wire.read() | (Wire.read() << 8);
        sensor->green = Wire.read() | (Wire.read() << 8);
        sensor->blue = Wire.read() | (Wire.read() << 8);

        // Применение фильтров
        sensor->filteredClear = runningAverage(&sensor->clearFilter, 5, sensor->clear);
        sensor->filteredRed = runningAverage(&sensor->redFilter, 5, sensor->red);
        sensor->filteredGreen = runningAverage(&sensor->greenFilter, 5, sensor->green);
        sensor->filteredBlue = runningAverage(&sensor->blueFilter, 5, sensor->blue);
    }
}

void readAPDSProximity(APDS9960_Sensor *sensor)
{
    sensor->proximity = readAPDSRegister(0x9C);
    sensor->filteredProximity = runningAverage(&sensor->proximityFilter, 8, sensor->proximity);
}

void APDS9960_Update(APDS9960_Sensor *sensor)
{
    if (!sensor->isInitialized)
        return;

    readAPDSColor(sensor);
    readAPDSProximity(sensor);
}
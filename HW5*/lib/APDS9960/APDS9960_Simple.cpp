#include "APDS9960_Simple.h"

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

bool APDS9960_SimpleInit(APDS9960_Sensor *sensor)
{
    // Инициализация структур
    sensor->isInitialized = false;
    sensor->bufferIndex = 0;

    // Обнуление буферов
    memset(sensor->redBuffer, 0, sizeof(sensor->redBuffer));
    memset(sensor->greenBuffer, 0, sizeof(sensor->greenBuffer));
    memset(sensor->blueBuffer, 0, sizeof(sensor->blueBuffer));
    memset(sensor->clearBuffer, 0, sizeof(sensor->clearBuffer));
    memset(sensor->proximityBuffer, 0, sizeof(sensor->proximityBuffer));

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

// Простая функция усреднения
float simpleAverage(uint16_t *buffer, uint8_t size)
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < size; i++) {
        sum += buffer[i];
    }
    return (float)sum / size;
}

float simpleAverageUint8(uint8_t *buffer, uint8_t size)
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < size; i++) {
        sum += buffer[i];
    }
    return (float)sum / size;
}

void readAPDSColorSimple(APDS9960_Sensor *sensor)
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

        // Обновление буферов для цветов
        sensor->redBuffer[sensor->bufferIndex % 3] = sensor->red;
        sensor->greenBuffer[sensor->bufferIndex % 3] = sensor->green;
        sensor->blueBuffer[sensor->bufferIndex % 3] = sensor->blue;
        sensor->clearBuffer[sensor->bufferIndex % 3] = sensor->clear;

        // Простое усреднение
        sensor->filteredRed = simpleAverage(sensor->redBuffer, 3);
        sensor->filteredGreen = simpleAverage(sensor->greenBuffer, 3);
        sensor->filteredBlue = simpleAverage(sensor->blueBuffer, 3);
        sensor->filteredClear = simpleAverage(sensor->clearBuffer, 3);
    }
}

void readAPDSProximitySimple(APDS9960_Sensor *sensor)
{
    sensor->proximity = readAPDSRegister(0x9C);

    // Обновление буфера для приближения
    sensor->proximityBuffer[sensor->bufferIndex % 5] = sensor->proximity;

    // Простое усреднение
    sensor->filteredProximity = simpleAverageUint8(sensor->proximityBuffer, 5);
}

void APDS9960_SimpleUpdate(APDS9960_Sensor *sensor)
{
    if (!sensor->isInitialized)
        return;

    readAPDSColorSimple(sensor);
    readAPDSProximitySimple(sensor);

    // Увеличиваем индекс буфера
    sensor->bufferIndex++;
    if (sensor->bufferIndex >= 15) {
        sensor->bufferIndex = 0;
    }
}
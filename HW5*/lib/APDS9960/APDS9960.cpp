#include "APDS9960.h"

// Запись байта в регистр
void APDS9960_WriteByte(uint8_t reg, uint8_t data)
{
    I2C_WriteByte(APDS9960_ADDR, reg, data);
}

// Чтение байта из регистра
uint8_t APDS9960_ReadByte(uint8_t reg)
{
    uint8_t data;
    I2C_ReadBytes(APDS9960_ADDR, reg, &data, 1);
    return data;
}

// Проверка ID датчика
bool APDS9960_CheckID()
{
    uint8_t id = APDS9960_ReadByte(APDS9960_ID);
    return (id == 0xAB || id == 0x9C || id == 0xA8); // Различные версии датчика
}

// Инициализация датчика
bool APDS9960_Init(APDS9960_Sensor *sensor)
{
    // Проверка наличия датчика
    if (!APDS9960_CheckID()) {
        Serial.println("APDS-9960 not found!");
        sensor->isInitialized = false;
        return false;
    }

    // Сброс всех настроек
    APDS9960_WriteByte(APDS9960_ENABLE, 0x00);

    // Настройка измерения цвета
    APDS9960_WriteByte(APDS9960_ATIME, 0xFF);   // Время интеграции = 2.78ms
    APDS9960_WriteByte(APDS9960_CONTROL, 0x03); // Усиление 8x

    // Настройка приближения
    APDS9960_WriteByte(APDS9960_PPULSE, 0x87);  // 16 pulses
    APDS9960_WriteByte(APDS9960_CONFIG2, 0x01); // No saturation interrupts

    // Настройка жестов (упрощенная)
    APDS9960_WriteByte(APDS9960_GCONF4, 0x00); // All photodiodes active during gesture
    APDS9960_WriteByte(APDS9960_GPULSE, 0xC9); // 16 pulses, 16us length

    // Включение функций
    uint8_t enable = 0x00;
    enable |= 0x01; // Power ON
    enable |= 0x02; // Color enable
    enable |= 0x04; // Proximity enable
    enable |= 0x40; // Gesture enable
    APDS9960_WriteByte(APDS9960_ENABLE, enable);

    sensor->isInitialized = true;
    sensor->lastUpdate = millis();

    Serial.println("APDS-9960 initialized successfully");
    return true;
}

// Чтение данных о цвете
void APDS9960_ReadColor(APDS9960_Sensor *sensor)
{
    uint8_t buffer[8];
    I2C_ReadBytes(APDS9960_ADDR, APDS9960_CDATAL, buffer, 8);

    sensor->clear = (buffer[1] << 8) | buffer[0];
    sensor->red = (buffer[3] << 8) | buffer[2];
    sensor->green = (buffer[5] << 8) | buffer[4];
    sensor->blue = (buffer[7] << 8) | buffer[6];

    // Применение фильтров
    sensor->filteredClear = runningAverage(&sensor->clearFilter, 5, sensor->clear);
    sensor->filteredRed = runningAverage(&sensor->redFilter, 5, sensor->red);
    sensor->filteredGreen = runningAverage(&sensor->greenFilter, 5, sensor->green);
    sensor->filteredBlue = runningAverage(&sensor->blueFilter, 5, sensor->blue);
}

// Чтение данных о приближении
void APDS9960_ReadProximity(APDS9960_Sensor *sensor)
{
    sensor->proximity = APDS9960_ReadByte(APDS9960_PDATA);
    sensor->filteredProximity = runningAverage(&sensor->proximityFilter, 8, sensor->proximity);
}

// Чтение жестов (упрощенная версия)
void APDS9960_ReadGesture(APDS9960_Sensor *sensor)
{
    uint8_t status = APDS9960_ReadByte(APDS9960_GSTATUS);

    if (status & 0x01) { // Есть данные жестов
        uint8_t fifo_level = APDS9960_ReadByte(APDS9960_GFLVL);

        if (fifo_level > 0) {
            uint8_t buffer[128];
            I2C_ReadBytes(APDS9960_ADDR, 0xFC, buffer, fifo_level * 4);

            // Упрощенная логика определения жестов
            // В реальном проекте здесь должна быть более сложная логика
            int UD_ratio = 0;
            int LR_ratio = 0;

            for (int i = 0; i < fifo_level * 4; i += 4) {
                int U = buffer[i] + buffer[i + 1];
                int D = buffer[i + 2] + buffer[i + 3];
                int L = buffer[i] + buffer[i + 2];
                int R = buffer[i + 1] + buffer[i + 3];

                UD_ratio += (U - D);
                LR_ratio += (L - R);
            }

            if (abs(UD_ratio) > abs(LR_ratio)) {
                if (UD_ratio > 0) {
                    sensor->gesture = 1;
                    sensor->gestureName = "UP";
                } else {
                    sensor->gesture = 2;
                    sensor->gestureName = "DOWN";
                }
            } else {
                if (LR_ratio > 0) {
                    sensor->gesture = 3;
                    sensor->gestureName = "LEFT";
                } else {
                    sensor->gesture = 4;
                    sensor->gestureName = "RIGHT";
                }
            }
        }
    } else {
        sensor->gesture = 0;
        sensor->gestureName = "NONE";
    }
}

// Обновление всех данных датчика
void APDS9960_Update(APDS9960_Sensor *sensor)
{
    if (!sensor->isInitialized)
        return;

    APDS9960_ReadColor(sensor);
    APDS9960_ReadProximity(sensor);
    APDS9960_ReadGesture(sensor);

    sensor->lastUpdate = millis();
}
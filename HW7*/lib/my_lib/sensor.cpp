#include "sensor.h"

static float temp = 0;
static float hum = 0;
static bool sensor_ready_flag = false;

void sensor_read(void)
{
    // Чтение температуры
    Wire.beginTransmission(GY21_ADDRESS);
    Wire.write(TEMP_CMD);
    if (Wire.endTransmission() == 0) {
        delay(SENSOR_DELAY);
        Wire.requestFrom(GY21_ADDRESS, 2);
        if (Wire.available() >= 2) {
            uint16_t rawTemp = Wire.read() << 8 | Wire.read();
            temp = (TEMP_MULTIPLIER * rawTemp / TEMP_DIVISOR) + TEMP_OFFSET;
        }
    }

    // Чтение влажности
    Wire.beginTransmission(GY21_ADDRESS);
    Wire.write(HUM_CMD);
    if (Wire.endTransmission() == 0) {
        delay(SENSOR_DELAY);
        Wire.requestFrom(GY21_ADDRESS, 2);
        if (Wire.available() >= 2) {
            uint16_t rawHum = Wire.read() << 8 | Wire.read();
            hum = (HUM_MULTIPLIER * rawHum / HUM_DIVISOR) + HUM_OFFSET;
        }
    }
}

bool sensor_ready(void)
{
    return sensor_ready_flag;
}

void sensor_set_ready(bool state)
{
    sensor_ready_flag = state;
}

float sensor_get_temp(void)
{
    return temp;
}

float sensor_get_hum(void)
{
    return hum;
}
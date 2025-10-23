
#include "SensorsI2C.h"

// ==================== GY-521 (MPU-605)  ====================
// Универсальная функция чтения IMU датчика
void readIMU(uint8_t deviceAddr, int16_t *acx, int16_t *acy, int16_t *acz,
             int16_t *gyx, int16_t *gyy, int16_t *gyz, int16_t *tmp)
{
    uint8_t buffer[14];

    // Читаем 14 байтов начиная с регистра 0x3B
    I2C_ReadBytes(deviceAddr, 0x3B, buffer, 14);

    // Преобразуем байты в 16-битные значения
    // акселерометр
    *acx = (buffer[0] << 8) | buffer[1];
    *acy = (buffer[2] << 8) | buffer[3];
    *acz = (buffer[4] << 8) | buffer[5];
    // температура
    *tmp = (buffer[6] << 8) | buffer[7];
    // гироскоп
    *gyx = (buffer[8] << 8) | buffer[9];
    *gyy = (buffer[10] << 8) | buffer[11];
    *gyz = (buffer[12] << 8) | buffer[13];
}

// Перевод из "сырых значений" в физические величины
//----------------------------------------------------

// Перевод акселерометра в g (ускорение свободного падения)
// MPU-6050 по умолчанию работает в диапазоне ±2g
// Чувствительность: 16384 LSB/g (Least Significant Bits per g)
// Это значит: 1g = 16384 "сырых единиц"
float accelToG(int16_t rawValue)
{
    return rawValue / ACCEL_SENSITIVITY;
}

// Перевод гироскопа в градусы/секунду
// Гироскоп по умолчанию: ±250°/сек
// Чувствительность: 131 LSB/°/сек
float gyroToDegPerSec(int16_t rawValue)
{
    return rawValue / GYRO_SENSITIVITY;
}

// Вычисление углов наклона из акселерометра
void accelToTiltAngles(int16_t rawX, int16_t rawY, int16_t rawZ, float *angleX, float *angleY)
{
    float gx = accelToG(rawX);
    float gy = accelToG(rawY);
    float gz = accelToG(rawZ);

    // Угол по X (вокруг оси X) - тангаж
    *angleX = atan2(gy, gz) * 180.0f / M_PI;

    // Угол по Y (вокруг оси Y) - крен
    *angleY = atan2(-gx, sqrt(gy * gy + gz * gz)) * 180.0f / M_PI;
}

// Вычисление температуры в °C
float tempToCelsius(int16_t rawValue)
{
    return rawValue / TEMP_SENSITIVITY + TEMP_OFFSET;
}

// ==================== TMP102  ==============================
// Для датчика температуры TMP102
float readTemperature()
{
    uint8_t buffer[2];
    I2C_ReadBytes(TMP102_ADDR, 0x00, buffer, 2);

    int16_t raw = (buffer[0] << 4) | (buffer[1] >> 4);
    return raw * 0.0625;
}

// ==================== BH1750  ==============================
// Для датчика освещенности BH1750
uint16_t readLight()
{
    uint8_t buffer[2];
    I2C_WriteByte(BH1750_ADDR, 0x10, 0); // Команда измерения
    delay(180);
    I2C_ReadBytes(BH1750_ADDR, 0x00, buffer, 2);

    return (buffer[0] << 8) | buffer[1];
}
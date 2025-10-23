#include "MPU6050.h"

// Инициализация датчика - теперь возвращает bool
bool MPU6050_Init(MPU6050_Sensor *sensor, uint8_t address)
{
    sensor->address = address;
    sensor->lastUpdate = millis();

    // Проверка наличия датчика
    Wire.beginTransmission(sensor->address);
    byte error = Wire.endTransmission();

    if (error != 0) {
        Serial.print("MPU6050 not found at address 0x");
        Serial.println(sensor->address, HEX);
        sensor->isInitialized = false;
        return false;
    }

    // Пробуждение MPU-6050
    I2C_WriteByte(sensor->address, 0x6B, 0);

    // Проверка, что датчик ответил
    delay(10);
    Wire.beginTransmission(sensor->address);
    error = Wire.endTransmission();

    if (error != 0) {
        Serial.println("MPU6050 initialization failed");
        sensor->isInitialized = false;
        return false;
    }

    // Инициализация фильтра Калмана с кастомными параметрами
    initKalman(&sensor->kfX, 0.1f, 0.1f, 0);
    initKalman(&sensor->kfY, 0.1f, 0.1f, 0);

    // Первое чтение для инициализации фильтров
    MPU6050_ReadRaw(sensor);

    sensor->isInitialized = true;
    Serial.print("MPU6050 at 0x");
    Serial.print(sensor->address, HEX);
    Serial.println(" initialized successfully");
    return true;
}

// Остальные функции остаются без изменений
void MPU6050_ReadRaw(MPU6050_Sensor *sensor)
{
    uint8_t buffer[14];
    I2C_ReadBytes(sensor->address, 0x3B, buffer, 14);

    sensor->AcX = (buffer[0] << 8) | buffer[1];
    sensor->AcY = (buffer[2] << 8) | buffer[3];
    sensor->AcZ = (buffer[4] << 8) | buffer[5];
    sensor->Tmp = (buffer[6] << 8) | buffer[7];
    sensor->GyX = (buffer[8] << 8) | buffer[9];
    sensor->GyY = (buffer[10] << 8) | buffer[11];
    sensor->GyZ = (buffer[12] << 8) | buffer[13];
}

// Перевод акселерометра в g
float accelToG(int16_t rawValue)
{
    return rawValue / 16384.0f; // ACCEL_SENSITIVITY
}

// Перевод гироскопа в градусы/секунду
float gyroToDegPerSec(int16_t rawValue)
{
    return rawValue / 131.0f; // GYRO_SENSITIVITY
}

// Вычисление углов наклона из акселерометра
void accelToTiltAngles(int16_t rawX, int16_t rawY, int16_t rawZ, float *angleX, float *angleY)
{
    float gx = accelToG(rawX);
    float gy = accelToG(rawY);
    float gz = accelToG(rawZ);

    *angleX = atan2(gy, gz) * 180.0f / M_PI;
    *angleY = atan2(-gx, sqrt(gy * gy + gz * gz)) * 180.0f / M_PI;
}

// Вычисление температуры в °C
float tempToCelsius(int16_t rawValue)
{
    return rawValue / 340.0f + 36.53f; // TEMP_SENSITIVITY и TEMP_OFFSET
}

// Обновление данных датчика
void MPU6050_Update(MPU6050_Sensor *sensor, float dt)
{
    if (!sensor->isInitialized)
        return;

    // Чтение сырых данных
    MPU6050_ReadRaw(sensor);

    // Перевод в физические величины
    sensor->accelX = accelToG(sensor->AcX);
    sensor->accelY = accelToG(sensor->AcY);
    sensor->accelZ = accelToG(sensor->AcZ);
    sensor->gyroX = gyroToDegPerSec(sensor->GyX);
    sensor->gyroY = gyroToDegPerSec(sensor->GyY);
    sensor->gyroZ = gyroToDegPerSec(sensor->GyZ);
    sensor->temperature = tempToCelsius(sensor->Tmp);

    // Вычисление углов из акселерометра
    float accelAngleX, accelAngleY;
    accelToTiltAngles(sensor->AcX, sensor->AcY, sensor->AcZ, &accelAngleX, &accelAngleY);

    // Применение фильтров
    sensor->accelX = runningAverage(&sensor->accelXFilter, 8, sensor->accelX);
    sensor->accelY = runningAverage(&sensor->accelYFilter, 8, sensor->accelY);
    sensor->accelZ = runningAverage(&sensor->accelZFilter, 8, sensor->accelZ);
    sensor->gyroX = runningAverage(&sensor->gyroXFilter, 10, sensor->gyroX);
    sensor->gyroY = runningAverage(&sensor->gyroYFilter, 10, sensor->gyroY);
    sensor->gyroZ = runningAverage(&sensor->gyroZFilter, 10, sensor->gyroZ);

    // Фильтрация углов фильтром Калмана
    sensor->angleX = kalmanFilter(&sensor->kfX, accelAngleX, dt);
    sensor->angleY = kalmanFilter(&sensor->kfY, accelAngleY, dt);

    // Обновление времени
    sensor->lastUpdate = millis();
}
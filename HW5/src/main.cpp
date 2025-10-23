#include "MPU6050.h"
#include "SimpleI2C.h"
#include <Arduino.h>

// Создаем экземпляры датчиков
MPU6050_Sensor mpu1;

// Переменные для расчета времени
unsigned long lastTime = 0;

void setup()
{
    Serial.begin(9600);
    Serial.println("setup");

    // Инициализация I2C
    I2C_Init();

    // Инициализация датчиков
    MPU6050_Init(&mpu1, 0x68);

    lastTime = millis();
}

void loop()
{
    // Расчет времени deltaTime
    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // Обновление данных обоих датчиков
    MPU6050_Update(&mpu1, dt);

    // Вывод данных первого датчика
    Serial.print("logo1:");
    Serial.println(mpu1.temperature);

    Serial.print(">AccelerometerX:");
    Serial.println(mpu1.accelX);
    Serial.print(">AccelerometerY:");
    Serial.println(mpu1.accelY);
    Serial.print(">AccelerometerZ:");
    Serial.println(mpu1.accelZ);

    Serial.print(">GyroscopeX:");
    Serial.println(mpu1.gyroX);
    Serial.print(">GyroscopeY:");
    Serial.println(mpu1.gyroY);
    Serial.print(">GyroscopeZ:");
    Serial.println(mpu1.gyroZ);

    Serial.print(">AngleX:");
    Serial.println(mpu1.angleX);
    Serial.print(">AngleY:");
    Serial.println(mpu1.angleY);

    delay(50);
}
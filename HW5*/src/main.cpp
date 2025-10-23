
#include "APDS9960_Simple.h"
#include "MPU6050.h"
#include "SimpleI2C.h"
#include <Arduino.h>

// Датчики
MPU6050_Sensor mpu1;
APDS9960_Sensor apds;

// Время
unsigned long lastTime = 0;

void setup()
{
    delay(2000);
    Serial.begin(9600);
    Serial.println("=== Dual Sensor System ===");
    Serial.println("GY-521 + APDS-9960");

    // Инициализация I2C
    I2C_Init();

    // Инициализация датчиков
    bool mpuOK = MPU6050_Init(&mpu1, 0x68);
    bool apdsOK = APDS9960_SimpleInit(&apds);

    if (mpuOK && apdsOK) {
        Serial.println("All sensors initialized successfully!");
    } else {
        Serial.println("Some sensors failed to initialize!");
    }

    lastTime = millis();
}

void loop()
{
    // Расчет времени
    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // Обновление данных датчиков
    MPU6050_Update(&mpu1, dt);
    APDS9960_SimpleUpdate(&apds);

    // Вывод некоторых данных GY-521
    // if (mpu1.isInitialized) {
    Serial.print(">1_ACC_X:");
    Serial.println(mpu1.accelX);
    Serial.print(">ACC_Y:");
    Serial.println(mpu1.accelY);
    Serial.print(">GYRO_Z:");
    Serial.println(mpu1.gyroZ);
    Serial.print(">ANGLE_X:");
    Serial.println(mpu1.angleX);
    // }

    // Вывод данных APDS-9960 - ТОЛЬКО ОТФИЛЬТРОВАННЫЕ ЗНАЧЕНИЯ
    // if (apds.isInitialized) {
    Serial.print(">PROX:");
    Serial.println(apds.filteredProximity);
    Serial.print(">COLOR_R:");
    Serial.println(apds.filteredRed);
    Serial.print(">COLOR_G:");
    Serial.println(apds.filteredGreen);
    Serial.print(">COLOR_B:");
    Serial.println(apds.filteredBlue);
    Serial.print(">LIGHT:");
    Serial.println(apds.filteredClear);
    // }

    // Отладочная информация (раз в 2 секунды)
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 2000) {
        lastDebug = millis();
        Serial.println("=== DEBUG ===");
        Serial.print("RAW - Prox: ");
        Serial.print(apds.proximity);
        Serial.print(" Red: ");
        Serial.print(apds.red);
        Serial.print(" Clear: ");
        Serial.println(apds.clear);
        Serial.print("FILTERED - Prox: ");
        Serial.print(apds.filteredProximity);
        Serial.print(" Red: ");
        Serial.print(apds.filteredRed);
        Serial.print(" Clear: ");
        Serial.println(apds.filteredClear);
    }

    delay(150);
}
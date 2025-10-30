#include "display.h"
#include "sensor.h"
#include "timers.h"

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    display_init();
    timers_init();

    Serial.println("ST7789 + GY-21 Ready");
    Serial.println("Display: 1000ms, Sensor: 2000ms");
}

void loop()
{
    if (sensor_ready()) {
        sensor_read();
        Serial.print("Sensor: T=");
        Serial.print(sensor_get_temp());
        Serial.print("C H=");
        Serial.print(sensor_get_hum());
        Serial.println("%");
    }

    if (display_ready()) {
        display_update();
    }

    delay(10);
}
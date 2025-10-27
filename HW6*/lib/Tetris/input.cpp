#include "input.h"

byte mypush(void)
{
    unsigned long tpr = millis();
    byte res = NotPush;

    if (!digitalRead(pin_SW)) {
        if (TimeOfPush == 0)
            TimeOfPush = tpr;
        else if (tpr - TimeOfPush > DurationOfLongPush && !myfl) {
            TimeOfPush = 0;
            myfl = true;
            return LongPush;
        }
    } else {
        if (TimeOfPush > 0 && !myfl)
            res = ShortPush;
        TimeOfPush = 0;
        myfl = false;
    }
    return res;
}

void timerInterrupt()
{
    encoder.scanState();
}
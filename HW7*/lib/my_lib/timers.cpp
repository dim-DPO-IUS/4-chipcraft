#include "timers.h"

void timers_init(void)
{
    cli();

    // Настройка Timer1 для дисплея
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = TIMER1_OCR_VALUE;
    TCCR1B |= TIMER1_MODE;
    TCCR1B |= TIMER1_PRESCALER;
    TIMSK1 |= TIMER1_INTERRUPT;

    // Настройка Timer2 для датчика
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    OCR2A = TIMER2_OCR_VALUE;
    TCCR2A |= TIMER2_MODE;
    TCCR2B |= TIMER2_PRESCALER;
    TIMSK2 |= TIMER2_INTERRUPT;

    sei();
}

ISR(TIMER1_COMPA_vect)
{
    display_set_ready(true);
}

ISR(TIMER2_COMPA_vect)
{
    sensor_set_ready(true);
}
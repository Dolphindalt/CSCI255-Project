/**
 * @author Kaleb Bausch
 * @version 12/5/2018
 */
#include <msp430.h>
#include "motor.h"

#define DC (40000)
#define RC (20000)

void motor_init()
{
    P2DIR |= 0x04 | 0x02;
    TA1CCR0 = 60000;                     // period

    TA1CCTL1 = OUTMOD_7;

    TA1CCTL2 = OUTMOD_7;

    TA1CTL = TASSEL_2 + MC_1;   // selects clock source and counts to value
}

void right()
{
    P2SEL |= 0x02 | 0x04;
    TA1CCR1 = RC;
    TA1CCR2 = DC;
    __delay_cycles(500000L);
    P2SEL &=~(0x02 | 0x04);
}

void backward()
{
    P2SEL |= 0x02;
    TA1CCR1 = RC;
    TA1CCTL2 = DC;
    __delay_cycles(500000L);
    P2SEL &=~(0x02);
}

void forward()
{
    P2SEL |= 0x04;
    TA1CCR1 = DC;
    TA1CCTL2 = RC;
    __delay_cycles(500000L);
    P2SEL &=~(0x04);
}

void left()
{
    P2SEL |= 0x02 | 0x04;
    TA1CCR1 = RC;
    TA1CCR2 = DC;
    __delay_cycles(500000L);
    P2SEL &=~(0x02 | 0x04);
}

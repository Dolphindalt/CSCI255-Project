/**
 * @author Dalton Caron
 * @version 11/9/2018
 */
#include "controller.h"
#include <msp430.h>

#define POLL_COMMAND 0x400302

typedef enum mode { WAITING, SENDING, RECIEVING } mode_t;

typedef struct handler
{
    mode_t current_status;
    unsigned int gc;
} handler_t;

handler_t gbch;

void controller_init()
{
    CCTL0 = CCIE;                   // CCR0 interrupt enabled
    TA0CTL = TASSEL_2 + MC_1;       // 1 mHz cpu and counting up to CCR0
    CCR0 = 6;                       // 1 mHz/s = 1 cycle/us
    P1DIR |= 0x01;
    gbch.current_status = WAITING;
    gbch.gc = 0;
    P1OUT |= 0x01;
}

// GBC expects 1 us low and 3 us high for a high edge
static void send_high()
{
    P1OUT &=~(0x01);
    P1OUT |= 0x01;
    __delay_cycles(3);
}

// GBC expects 3 us low and 1 us high for a low edge
static void send_low()
{
        P1OUT &=~(0x01);
        __delay_cycles(3);
        P1OUT |= 0x01;
}

/**
 * The micro controller must poll the gamecube controller after every
 * six micro seconds to determine the controller's state.
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void PollingTimer()
{
    switch(gbch.current_status)
    {
    case WAITING:
        gbch.gc++;
        if(gbch.gc == 6)
            gbch.current_status = SENDING;
        break;
    case SENDING:
        gbch.gc = 23;
        P1DIR |= 0x01;
        while(gbch.gc >= 0)
            if((POLL_COMMAND >> gbch.gc--) & 0x01)
                send_high();
            else
                send_low();
        P1OUT |= 0x01; // stop bit
        gbch.current_status = RECIEVING;
        break;
    case RECIEVING:
        gbch.gc = 63;
        P1DIR &=~(0x01);
        while(gbch.gc >= 0)
            controller_data[gbch--] |= (P1IN & 0x01);
        gbch.gc = 0;
        gbch.current_status = WAITING;
        break;
    default:
        break;
    }
}
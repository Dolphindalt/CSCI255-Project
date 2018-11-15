/**
 * @author Dalton Caron
 * @version 11/9/2018
 */
#include "controller.h"
#include <msp430.h>

extern void gbc_tx(const byte_t *buffer, byte_t len);
extern void gbc_rx(byte_t *buffer, byte_t len);
extern edge_test();
extern brute_force();

int a = 1;

void controller_init()
{
    CCTL0 = CCIE;                   // CCR0 interrupt enabled
    TA0CTL = TASSEL_1 + MC_1;       // 1 mHz cpu and counting up to CCR0
    CCR0 = 6;                       // 1 mHz/s = 1 cycle/us
    P1DIR |= 0x01;
    P1OUT |= 0x01;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void PollingTimer()
{
    byte_t poll_command[] = { 0x40, 0x03, 0x03 };
    gbc_tx(poll_command, sizeof(poll_command));
    //gbc_rx(poll_command, sizeof(poll_command));
    __delay_cycles(1024);
}

/**
 * @author Dalton Caron
 * @version 11/17/2018
 */
#include "controller.h"
#include <msp430.h>

extern byte_t gbc_rw(const byte_t *tx, byte_t len_tx, byte_t *rx, byte_t len_rx);
extern edge_test();
extern brute_force();

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
    byte_t poll_command[] = { 0x40, 0x03, 0x03 }, output_buffer[64];
    byte_t read = gbc_rw(poll_command, sizeof(poll_command), output_buffer, sizeof(output_buffer));
}

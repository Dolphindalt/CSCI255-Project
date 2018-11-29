/**
 * @author Dalton Caron
 * @version 11/28/2018
 */
#include "controller.h"
#include <msp430.h>

#define GBC_BYTE_BUFFER_SIZE 0x08

extern byte_t gbc_rw(const byte_t *tx, byte_t len_tx, byte_t *rx, byte_t len_rx);

void controller_init()
{
    CCTL0 = CCIE;                   // CCR0 interrupt enabled
    TA0CTL = TASSEL_1 + MC_1;       // 1 mHz cpu and counting up to CCR0
    CCR0 = 96;                      // 16 mHz/s = 16 cycle/us
    P1DIR |= 0x01;
    P1OUT |= 0x01;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void PollingTimer()
{
    byte_t poll_command[] = { 0x40, 0x03, 0x02 }, output_buffer[GBC_BYTE_BUFFER_SIZE];
    volatile unsigned int i = 0;
    while(i < GBC_BYTE_BUFFER_SIZE)
           output_buffer[i++] = 0;
    volatile byte_t read = gbc_rw(poll_command, sizeof(poll_command), output_buffer, sizeof(output_buffer)); // the compiler is annopying. We will only be here for 500us tops
    if(read != GBC_BYTE_BUFFER_SIZE) return; // disregard results if we didn't read everything
    volatile unsigned int buttons = 0;
    buttons |= output_buffer[0];
    buttons = buttons << 8;
    buttons |= output_buffer[1];
    gbc.buttons = buttons;
    gbc.joy1X = output_buffer[2];
    gbc.joy1Y = output_buffer[3];
    gbc.joy2X = output_buffer[4];
    gbc.joy2Y = output_buffer[5];
    gbc.left = output_buffer[6];
    gbc.right = output_buffer[7];
}

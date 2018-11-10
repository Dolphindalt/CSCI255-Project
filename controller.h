/**
 * @author Dalton Caron
 * @version 11/9/2018
 *
 * This header file and associated source folder are for interfacing with the
 * nintendo gamecube controller from the MSP 430.
 *
 * P1.0 -> Bi-directional data line
 *
 * Example usage:
 * if(GCCIN & GBC_A) {
 *      // A is pressed
 * }
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#define GBC_START 0x1000
#define GBC_Z 0x0010
#define GBC_A 0x0100
#define GBC_B 0x0200
#define GBC_Y 0x0800
#define GBC_X 0x0400
#define GBC_DPAD_UP 0x0008
#define GBC_DPAD_LEFT 0x0001
#define GBC_DPAD_RIGHT 0x0002
#define GBC_DPAD_DOWN 0x0004
#define GBC_LEFT 0x0040
#define GBC_RIGHT 0x0020

typedef unsigned char byte_t; // 8 bits
typedef unsigned int word_t; // 16 bits

// this attribute packed is an old trick from OS development
typedef struct __attribute__((__packed__)) controller_data {
    word_t buttons; // use the gcc bitmasks defined above to check button state
    byte_t joy1X; // the joy1 that is not yellow
    byte_t joy1Y;
    byte_t joy2X; // joy2 is the yellow one
    byte_t joy2Y;
    byte_t left; // left button
    byte_t right; // right button
} controller_data_t;

#define GCCIN controller_data
controller_data_t controller_data; // the define above is an alias for my partners

// Sets up timer interrupts used to satisfy the controller's protocol.
void controller_init();

#endif

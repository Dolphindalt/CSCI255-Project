#ifndef GBC_MACROS_H
#define GBC_MACROS_H

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

typedef unsigned int word_t;
typedef unsigned char byte_t;

typedef struct __attribute__((__packed__)) controller_data {
    word_t buttons; // use the bitmasks defined above to check button state
    byte_t joy1X; // the joy1 that is not yellow
    byte_t joy1Y;
    byte_t joy2X; // joy2 is the yellow one
    byte_t joy2Y;
    byte_t left; // left button
    byte_t right; // right button
} controller_data_t;

void controller_init();

#endif

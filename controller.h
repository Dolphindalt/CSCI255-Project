/**
 * @author Dalton Caron
 * @version 11/28/2018
 *
 * * * * PREFACE * * * *
 *
 * Welcome to the user guide and venting paragraph. As the creator of this thing, I would
 * like to say that this is the either the most terrible or greatest thing I have ever
 * created. I have always wanted to use assembly for something practical, and never did
 * I think that this class would provide such an opportunity. Such a stupid challenge;
 * throughout all the time spent here, I am very satisfied with this end product. Maybe
 * these files could be used in future projects with the controller hardware? That would
 * be hell and a hell of a time.
 *
 * * * * TECHNNICAL GUIDE * * * *
 *
 * The Nintendo Gamecube Controller. It has six wires, which are documented in the software
 * guide below. This section of the guide is going to talk more about what is going on
 * between the controller and computer, and the code.
 *
 * The gamecube controller does nothing while hooked up to pins at the start. The DATA line
 * is pulled up, so the DATA line is constantly high. Communication between the controller
 * and MSP430 is initialized by a 24 bit command. The command is as follows:
 *
 * 0b 0100 0000 0000 0011 0000 0010
 * 0x 40 03 02
 *
 * The last bit in this command can be set high to enable the rumble motor. It is worth
 * noting that all gamecube controller commands end with a high stop bit. So after this
 * command, 1 is also transmitted.
 *
 * After the command is sent, the controller responds with a 64 bit message containing
 * the state of the buttons for the controller. A table explaining follows:
 *
 * Byte 0   0   0   0   Start   Y       X       B       A
 * Byte 1   1   L   R   Z       D Up    D Down  D Right D Left
 * Byte 2   Joystick X
 * Byte 3   Joystick Y
 * Byte 4   C Stick X
 * Byte 5   C Stick Y
 * Byte 6   Left Button
 * Byte 7   Right Button
 *
 * The controller is very fast, so we had to overclock to 16MHz. A low bit is represented
 * by 3us low followed by 1us high. A high bit is represented as 1us low followed by 3us
 * high. There may be a delay between the command sending and controller responding, so
 * we took precautions in case a delay were to occur in this program.
 *
 * For more detailed information on the hardware and software, please refer to the sections
 * positioned below.
 *
 * * * * SOFTWARE GUIDE * * * *
 *
 * Reading input from the game cube controller is really a simple task.
 *
 * BEG CODE * * * * * * * * * * * * * * * * * * * * * * *
 * #include "controller.h"
 * ...
 * unsigned int result = gbc.buttons & GBC_A;
 * END CODE * * * * * * * * * * * * * * * * * * * * * * *
 *
 * The example code snippet above bit masks the GBC_A macro with the buttons variable
 * in the controller_data_t struct. If the result of the mask is 1, the button is pressed.
 * If 0, the button is released.
 *
 * There are quite a few macros for the gbc.buttons variable, and they are defined below.
 *
 * For all the other variables of the controller_data_t struct, they represent axis values
 * for their respective parts. Each axis travels in only one direction on an x and y plane,
 * so you can use both axis for a single joystick to determine its rotation and magnitude.
 * Unfortunately, our microcontroller does not support floating point numbers, so you will
 * have to make do with the x and y values.
 *
 * * * * HARDWARE GUIDE * * * *
 *
 * The physical hook up of the controller really is not that bad. Below is a table of the
 * wires on the controller and what they are suppose to be hooked up to.
 *
 * Color    Function
 * Yellow   5V power for rumble
 * Red      DATA line, 3.43V with pull up resistor
 * Green    Ground
 * White    Ground
 * Blue     3.43V logic supply
 *
 * Yeah, that DATA line needs a pull up resistor. 1kOhm seems to work just fine.
 *
 * * * * MISC * * * *
 *
 * It is note worthy to mention that we never quite got the rumble motor to function.
 * Rumble is activated by changing making the last bit of the poll command high. Despite
 * our efforts, we were unable to activate it.
 *
 * There may be more commands that we are unaware of. Maybe we will discover them one day.
 */
#ifndef GBC_MACROS_H
#define GBC_MACROS_H

#define GBC_START 0x10000
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

// these data types are for my sanity
typedef unsigned int word_t;
typedef unsigned char byte_t;

typedef struct controller_data {
    word_t buttons; // use the bitmasks defined above to check button state
    byte_t joy1X; // the joy1 that is not yellow
    byte_t joy1Y;
    byte_t joy2X; // joy2 is the yellow one
    byte_t joy2Y;
    byte_t left; // left button
    byte_t right; // right button
} controller_data_t;

// stores the state of the controller for all clients to see and use
controller_data_t gbc;

// simply initializes pins for the controller DATA line and interrupts for polling
void controller_init();

#endif

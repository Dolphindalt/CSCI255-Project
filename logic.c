#include "logic.h"
#include "music.h"
#include "controller.h"
#include "motor.h"

void logic_loop()
{
    if(gbc.buttons == 0xFFFF)
    {

    }
    else if(gbc.buttons & GBC_A)
	{
		playMeglovania();
		__delay_cycles(500000L);
	}
	else if(gbc.buttons & GBC_RIGHT)
	{
		playImperialMarch();
		__delay_cycles(500000L);
	}
	else if(gbc.buttons & GBC_B)
	{
		playDubstepFart();
		__delay_cycles(500000L);
	}
	else if(gbc.buttons & GBC_DPAD_LEFT)
	{
	    left();
	}
	else if(gbc.buttons & GBC_DPAD_RIGHT)
	{
	    right();
	}
	else if(gbc.buttons & GBC_DPAD_UP)
	{
	    forward();
	}
	else if(gbc.buttons & GBC_DPAD_DOWN)
	{
	    backward();
	}
}

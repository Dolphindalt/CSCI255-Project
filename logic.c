#include "logic.h"
#include "music.h"
#include "controller.h"

void logic_loop()
{
	if(gbc.buttons & GBC_A)
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
}

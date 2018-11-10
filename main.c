#include <msp430.h> 
#include "controller.h"

int main()
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	controller_init();          // init gamecube controller

	_BIS_SR(GIE);               // enable interrupts

	for(;;)
	{

	}
}

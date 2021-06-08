#include "main.h"
#include "buttons.h"
#include "gpio.h"

static button_state_t ButtonState = Nothing;

button_state_t button_processing(void)
{
static uint16_t portstate;
portstate = (GPIOA->IDR>>8)&0x0F;

switch(portstate)
{
    case ShortPressUp:
    	ButtonState=ShortPressUp;
        break;
    case Nothing:
    	ButtonState=Nothing;
		break;
    default:

        break;
}
	  return ButtonState;
}

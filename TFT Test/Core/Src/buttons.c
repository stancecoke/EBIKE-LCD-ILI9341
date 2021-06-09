#include "main.h"
#include "buttons.h"
#include "gpio.h"
#include "string.h"

static button_state_t ButtonState = Nothing;
static button_state_t LastButtonState = Nothing;
static uint16_t StartTimes[15];
static uint32_t Now;
static uint8_t LongPressFlag[15];
TIM_HandleTypeDef htim1;

button_state_t button_processing(void)
{
static uint16_t portstate;
portstate = (GPIOA->IDR>>8)&0x0F;

switch(portstate)
{
    case ShortPressUp:
    	ButtonState=ShortPressUp;
    	if(LastButtonState!=ShortPressUp&&!LongPressFlag[ShortPressUp])StartTimes[ButtonState] =__HAL_TIM_GET_COUNTER(&htim1);

        break;
    case ShortPressDown:
    	ButtonState=ShortPressDown;
    	if(LastButtonState!=ShortPressDown&&!LongPressFlag[ShortPressDown])StartTimes[ButtonState] =__HAL_TIM_GET_COUNTER(&htim1);

        break;
    case ShortPressPower:
    	ButtonState=ShortPressPower;
    	if(LastButtonState!=ShortPressPower&&!LongPressFlag[ShortPressPower])StartTimes[ButtonState] =__HAL_TIM_GET_COUNTER(&htim1);

        break;
    case ShortPressLight:
    	ButtonState=ShortPressLight;
    	if(LastButtonState!=ShortPressLight&&!LongPressFlag[ShortPressLight])StartTimes[ButtonState] =__HAL_TIM_GET_COUNTER(&htim1);

        break;
    case ShortUpAndDown:
    	ButtonState=ShortUpAndDown;
    	if(LastButtonState!=ShortUpAndDown&&!LongPressFlag[ShortUpAndDown])StartTimes[ButtonState] =__HAL_TIM_GET_COUNTER(&htim1);

        break;
    case Nothing:
    	ButtonState=Nothing;
    	memset(StartTimes, 0, 15*sizeof(StartTimes[0]));
    	memset(LongPressFlag, 0, 15*sizeof(LongPressFlag[0]));

		break;
    default:
    	ButtonState=Unknown;
        break;
}


	Now =__HAL_TIM_GET_COUNTER(&htim1);
	for(int i = 0; i<16; i++){
	if(StartTimes[i]&&ButtonState==i){
		if (Now<StartTimes[i]&&!LongPressFlag[i]){
			if(((Now+60000)-StartTimes[i])>15000){

				LongPressFlag[i]=1;
			}
			else if((Now-StartTimes[i])>15000){

				LongPressFlag[i]=1;
			}
		}

		if(LongPressFlag[i])ButtonState=i+16;

	}
	else StartTimes[i] = 0;
	}

	LastButtonState=ButtonState;

	  return ButtonState;
}

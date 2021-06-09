#include "main.h"
#include "buttons.h"
#include "gpio.h"

static button_state_t ButtonState = Nothing;
static button_state_t LastButtonState = Nothing;
static uint16_t StartTimeButtonUp;
static uint16_t StartTimeButtonDown;
static uint32_t Now;
static uint8_t LongPressFlag=0;
TIM_HandleTypeDef htim1;

button_state_t button_processing(void)
{
static uint16_t portstate;
portstate = (GPIOA->IDR>>8)&0x0F;

switch(portstate)
{
    case ShortPressUp:
    	ButtonState=ShortPressUp;
    	if(LastButtonState!=ShortPressUp&&!LongPressFlag)StartTimeButtonUp =__HAL_TIM_GET_COUNTER(&htim1);

        break;
    case ShortPressDown:
    	ButtonState=ShortPressDown;
    	if(LastButtonState!=ShortPressUp)StartTimeButtonDown =__HAL_TIM_GET_COUNTER(&htim1);

        break;
    case Nothing:
    	ButtonState=Nothing;

    	StartTimeButtonUp = 0;
    	LongPressFlag=0;
		break;
    default:
    	ButtonState=Unknown;
        break;
}


	Now =__HAL_TIM_GET_COUNTER(&htim1);
	if(StartTimeButtonUp&&ButtonState==ShortPressUp){
		if (Now<StartTimeButtonUp&&!LongPressFlag){
			if(((Now+60000)-StartTimeButtonUp)>15000){

				LongPressFlag=1;
			}
			else if((Now-StartTimeButtonUp)>15000){

				LongPressFlag=1;
			}
		}

		if(LongPressFlag)ButtonState=LongPressUp;

	}
	else StartTimeButtonUp = 0;

	LastButtonState=ButtonState;

	  return ButtonState;
}

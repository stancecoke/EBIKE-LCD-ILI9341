#include "main.h"
#include "buttons.h"

static button_state_t ButtonState = Nothing;

button_state_t button_processing(void)
{

	  if(!HAL_GPIO_ReadPin (Up_Button_GPIO_Port, Up_Button_Pin))
	  {

		  ButtonState = ShortPressUp;
	  }
	  else
	  {
		  ButtonState = Nothing;
	  }
	  return ButtonState;
}

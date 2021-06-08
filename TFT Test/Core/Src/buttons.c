#include "main.h"

uint16_t button_processing(void)
{
	static int state;
	  if(!HAL_GPIO_ReadPin (Up_Button_GPIO_Port, Up_Button_Pin))
	  {

		  state = 1;
	  }
	  else
	  {
		  state = 0;
	  }
	  return state;
}

/*
 * buttons.h
 *
 *  Created on: Jun 8, 2021
 *      Author: gaswerke
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_
typedef enum {Nothing, ShortPressPower, ShortPressLight, ShortPressUp, ShortPressDown} button_state_t;
button_state_t button_processing(void);

#endif /* INC_BUTTONS_H_ */

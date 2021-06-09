/*
 * buttons.h
 *
 *  Created on: Jun 8, 2021
 *      Author: gaswerke
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_
typedef enum {
	Nothing = 15,
	ShortPressPower = 14,
	ShortPressLight = 13,
	ShortPressUp = 11,
	ShortPressDown = 7,
	ShortUpAndDown = 3,
	ShortPowerAndDown = 6,
	ShortPowerAndUp = 10,
	ShortLightAndDown = 9,
	ShortLightAndUp = 5,
	LongPressUp = ShortPressUp+16,
	LongPressDown = ShortPressDown+16,
	LongPressPower = ShortPressPower+16,
	LongPressLight = ShortPressLight+16,
	LongUpAndDown = ShortUpAndDown+16,
	Unknown} button_state_t;
button_state_t button_processing(void);

#endif /* INC_BUTTONS_H_ */

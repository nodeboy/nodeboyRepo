/*
 * key.h
 *
 *  Created on: Nov 26, 2013
 *      Author: konlese
 */

#ifndef KEY_H_
#define KEY_H_

typedef enum KEY_STATUS
{
	KEY_NONE,
	KEY_ENTER,
	KEY_CANCEL,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_CALL,
	KEY_MESSAGE
}Key_status;
Key_status keyval = KEY_NONE,keyvalbak = KEY_NONE;

#endif /* KEY_H_ */

/*
 * key.c
 *
 *  Created on: Nov 26, 2013
 *      Author: konlese
 */
void keyscanhandler(void)
{
	unsigned char tempkey = KEY_NONE;
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14) == 0)
	{
		tempkey = MENU_KEY;
	}
	else if(GPIO_ReadInputDataBit( GPIOD, GPIO_Pin_15) == 0 )
	{
		tempkey = SELE_UP_KEY;
	}
	else if(GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_6) == 0 )
	{
		tempkey = SELE_DOWN_KEY;
	}
	else if(GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_7) == 0 )
	{
		tempkey = ENTER_KEY;
	}
	if(tempkey)
	{
		if(tempkey == keyvalbak)
		{

			if(keyvalcount !=0xffff)
				keyvalcount++;
			if((keyvalcount ==300)&& (keyvalbak == ENTER_KEY))
			{
				keyval = PRIN_KEY;
			}
		}
		else
		{
			keyvalcount = 1;
			keyvalbak = tempkey;
		}
	}
	else
	{
		if(keyvalcount >5)
		{
			if((keyvalcount >300)&& (keyvalbak == ENTER_KEY))
			{
				if(keyval == KEY_NONE)
				{
					keyval = KEY_NONE;
				}
			}
			else
			{
				keyval = keyvalbak;
			}
			keyvalcount =0;
		}
		keyvalcount = 0;
		keyvalbak = 0;
	}

}



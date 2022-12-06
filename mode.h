// exploration and battle mode 

/*
#ifndef _MODEH_
#define _MODEH_
#include "main.h"
#include "Sensors.h"
#include "UART.h"
#include "interrupts.h"
#include "Battery.h"
#include "LED.h"
#include "stdio.h"


UART_HandleTypeDef huart1;


int number_press = 0;
int mode_flag = 0; // if mood_flag == 0 exploration mode, if mode_flag == 1 battle mode

void Exploration(int mode_flag, int SOS_flag, int tx)
{
	
	int t2 = uwTick;
	while(1)
	{
		
		if ( SOS_flag == 0)
		{
			Device_reading(reading);
			ExplorationLED();
			check_SOS(mode_flag, reading, score, SOS_flag, count);// check for SOS_flag 
		}
		if (uwTick - t2 >= 1000 && (SOS_flag == 1 || SOS_flag == 0 ))
		{
			uart_print(mode_flag, SOS_flag, reading); // to print message according to the mode and sos status
			t2 = uwTick;
		}
		else if (SOS_flag == 1 ) // warning blink 
		{
			WarningLED();
		}
		if (number_press > 0)
		{
			break;
		}
		
	}
}

void Battlemood(int mode_flag, int SOS_flag, int tx)
{
	int t5;
	while(1)
	{
		t5 = uwTick;
		if( SOS_flag == 0)
		{
			Device_reading(reading);
			if (uwTick - t5 >= 1000 && (SOS_flag == 1 || SOS_flag == 0 ))
			{
				uart_print(mode_flag, SOS_flag, reading); // to print message according to the mode and sos status
				t5 = uwTick;
			}
			check_SOS(mode_flag, reading, score, SOS_flag, count);// check for SOS_flag
			BattleLED();
			if (uwTick - t5 >= 1000 )
			{
				uart_print(mode_flag, SOS_flag, reading);
			}
			if (uwTick - tFlux > 5000) // Fire every 5s.
			{ 
				FluxerFire(); // Fire. -2 Battery. Only can fire if battery >= 2
			}
		}
		if (SOS_flag == 1)
		{
			WarningLED();
			if(uwTick - t5 >= 1000)
			{
				uart_print(mode_flag, SOS_flag, reading);
			}
		}
		if(number_press > 0)
		{
			break;
		}
	}
}

#endif
*/

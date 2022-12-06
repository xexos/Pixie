  /******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * (c) EE2028 Teaching Team
  ******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "main.h"
#include "Sensors.h"
#include "UART.h"
#include "interrupts.h"
#include "mode.h"
#include "Battery.h"
#include "LED.h"


UART_HandleTypeDef huart1;


int number_press = 0;
void clear_SOS(void);
int mode_flag = 0; // if mood_flag == 0 exploration mode, if mode_flag == 1 battle mode
extern void initialise_monitor_handles(void);
void check_SOS(int mode_flag, int count);
int tx;// to calculate interrupt time due to pb
int SOS_flag = 0; //if flag == 0 not warning state, if flag == 1 warning state


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == BUTTON_EXTI13_Pin)
	{
		number_press ++;
		char UARTBufferk[64];
		sprintf(UARTBufferk, "number of press : %d \r", number_press);
		HAL_UART_Transmit(&huart1, (uint8_t*)UARTBufferk, strlen(UARTBufferk),0xFFFF);
		tx = uwTick;

	}
	if(GPIO_Pin == LSM6DSL_INT1_EXTI11_Pin)
	{

		c3++;
		if(c3 < 2)
		{
			count++;
		}
		char UARTBufferl[64];
		sprintf(UARTBufferl, "free fall \r");
		HAL_UART_Transmit(&huart1, (uint8_t*)UARTBufferl, strlen(UARTBufferl),0xFFFF);

	}
	if(GPIO_Pin == LPS22HB_INT_DRDY_EXTI0_Pin)
	{
		c1++;
		if (c1 < 2)
		{
			count++;
		}
	}
}





int main()
{
	HAL_Init();
	SensorINIT();
	acceinterrupt();
	initialise_monitor_handles();
	pressureint();
	UART1_Init();
	MX_GPIO_Init();
	clear_SOS();
	Device_reading(reading);
	check_SOS(mode_flag,count);
	int Ty = 0;

	while(1)
	{

		if(number_press >= 1 )
		{
			if( number_press == 1 && uwTick - tx > 1000)
			{
				number_press = 0;
				if(mode_flag == 1 && SOS_flag == 0)
				{
					BatteryCharge();
				}
				else if(SOS_flag == 1)
				{
					clear_SOS();
				}
			}
			else if(number_press > 1 && uwTick - tx < 1000)
			{
				number_press = 0;
				if(mode_flag == 0 && SOS_flag == 0)
				{
					mode_flag = 1;
					char UARTBuffer[64];
					sprintf(UARTBuffer, "---CHANGE MODE: Battle MODE---\r\n");
					HAL_UART_Transmit(&huart1, (uint8_t*)UARTBuffer, strlen(UARTBuffer),0xFFFF);

				}
				else if (mode_flag == 1 && SOS_flag == 0)
				{
					mode_flag = 0;
					char UARTBuffers[64];
					sprintf(UARTBuffers, "---CHANGE MODE: Exploration MODE---\r\n");
					HAL_UART_Transmit(&huart1, (uint8_t*)UARTBuffers, strlen(UARTBuffers),0xFFFF);

				}
				else if(SOS_flag == 1)
				{
				}
			}
		}



		if(mode_flag == 0)
		{
			check_SOS(mode_flag,count);
			if(SOS_flag == 0)
			{
				if(uwTick - Ty > 1000)
				{
					Device_reading(reading);
					uart_print(mode_flag, SOS_flag, reading);
					check_SOS(mode_flag,count);
					Ty = uwTick;

				}
				ExplorationLED();
			}
			else if (SOS_flag == 1)
			{
				WarningLED();
				if(uwTick - Ty >1000)
				{
					uart_print(mode_flag, SOS_flag, reading);
					Ty = uwTick;
				}
			}
		}
		else if (mode_flag == 1)
		{
			check_SOS(mode_flag, count);
			if (SOS_flag == 0)
			{
				BattleLED();
				if(uwTick - Ty > 1000)
				{
					Device_reading(reading);
					uart_print(mode_flag,SOS_flag, reading);
					check_SOS(mode_flag, count);
					Ty = uwTick;

				}
				FluxerFire();
			}
			else if (SOS_flag == 1)
			{
				WarningLED();
				if(uwTick - Ty > 1000)
				{
					uart_print(mode_flag,SOS_flag, reading);
					Ty = uwTick;
				}
			}
		}
	}
}



void check_SOS(int mode_flag, int count)
{
	if(mode_flag == 0 && count > 1)
	{
		SOS_flag = 1;
	}
	else if (mode_flag == 1 && count > 0)
	{
		SOS_flag = 1;
	}
	else
	{
	}
}

void clear_SOS(void)
{
	SOS_flag = 0;
	count = 0;
	c0 = 0;
	c1 = 0;
	c2 = 0;
	c3 = 0;
	c4 = 0;
	c5 = 0;
}





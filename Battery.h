#ifndef _BATTERYH_
#define _BATTERYH_

#include "main.h"
#include "stdio.h"
#include "string.h"

UART_HandleTypeDef huart1;

int Battery = 10; // Initial battery value. 0 - 10




uint32_t tFlux = 0; // For firing fluxer every 5s (in main.c)

void BatteryPrint(){
	char BattPrint[32];
	sprintf(BattPrint, "Battery: {%d}/10 \r\n", Battery);
	HAL_UART_Transmit(&huart1, (uint8_t*)BattPrint, strlen(BattPrint),0xFFFF);

}

void FluxerFire(){
	char FluxPrint[32];
	
	if(uwTick - tFlux > 5000)
	{
		if (Battery >= 2){

			Battery -= 2;
			sprintf(FluxPrint, "--- Fluxer Fired: Boom --- \r\n");
			HAL_UART_Transmit(&huart1, (uint8_t*)FluxPrint, strlen(FluxPrint),0xFFFF);
			BatteryPrint();
			tFlux = uwTick;
		}
		else
		{
			sprintf(FluxPrint, "--- Not Enough Battery --- \r\n");
			HAL_UART_Transmit(&huart1, (uint8_t*)FluxPrint, strlen(FluxPrint),0xFFFF);
			tFlux = uwTick;
			x = 90
		}

	}
}

void BatteryCharge(){

	if (Battery < 9) { // Charge

		Battery++;
	}
	else if (Battery == 9) { // Charge to 10 (Full), not 11.

		Battery = 10;
	}
}

#endif

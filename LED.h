#ifndef _LEDH_
#define _LEDH_
#include "main.h"
#include "stdio.h"
#include "string.h"

#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01.h"

uint32_t tLED = 0; // For Systick interrupt

void ExplorationLED(void) { // Always on. Called once every time we enter Exploration Mode.

	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
	BSP_LED_On(LED2_Pin);
}

void WarningLED(void) { // 3 Hz Blink


	if (uwTick - tLED >= 333) {

		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
		BSP_LED_Toggle(LED2_Pin);
		tLED = uwTick;
	}
}

void BattleLED(void) { // 1 Hz Blink

	if (uwTick - tLED >= 1000) {

		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
		BSP_LED_Toggle(LED2_Pin);

		tLED = uwTick;
	}
}


#endif

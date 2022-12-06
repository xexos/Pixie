#ifndef _UARTH_
#define _UARTH_
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "Sensors.h"

UART_HandleTypeDef huart1;


static void UART1_Init(void)
{
	/* Pin configuration for UART. BSP_COM_Init() can do this automatically */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Configuring UART1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
      while(1);
    }
}

void uart_print(int mode_flag, int SOS_flag, float reading[])
{
	char buffer[240];
	char new_line[] = "\r\n";
	char percent[] = " %";
	char warning_msg[] = "WARNING mode: SOS \r\n";
	char battle_msg[] = "Battle mode: SOS \r\n";
	//reading[7]; to store sensor readings {T0|P1|H2|A3|G4|M5}
	if (mode_flag == 0 && SOS_flag == 0) // for exploration without warning
	{
		sprintf(buffer, "G: %.2f rad/s, H: %.2f %s, P: %.2f hPa, M: %.2f μT %s",reading[4], reading[2],percent,reading[1], reading[5], new_line);
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer),0xFFFF);
	}
	else if ( mode_flag == 0 && SOS_flag == 1) // for exploration in warning
	{
		sprintf(buffer,"%s %s", warning_msg, new_line);
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer),0xFFFF);

	}
	else if ( mode_flag == 1 && SOS_flag == 0 )
	{
		sprintf(buffer, "G: %.2f rad/s, H: %.2f %s, P: %.2f hPa, M: %.2f μT, T: %.2f °C, A: %.2f m/s^2  %s",reading[4], reading[2], percent,reading[1], reading[5], reading[0], reading[3], new_line);
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer),0xFFFF);
	}
	else
	{
		sprintf(buffer,"%s %s", battle_msg, new_line);
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer),0xFFFF);
	}
}
#endif




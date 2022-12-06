#ifndef _SENSORSH_
#define _SENSORSH_
#include "main.h"
#include "stdio.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_tsensor.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_accelero.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_gyro.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_hsensor.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_magneto.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_psensor.h"
#include "math.h"
#include "interrupts.h"

int c0 = 0;
int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;
int c5 = 0;



float reading[7];// to store sensor readings {T0|P1|H2|A3|G4|M5}

void SensorINIT() //installaion of sensors 
{

	BSP_GYRO_Init();
	BSP_ACCELERO_Init();
	BSP_TSENSOR_Init();
	BSP_HSENSOR_Init();
	BSP_PSENSOR_Init();
	BSP_MAGNETO_Init();
}



void Device_reading(float reading[]) //readings stored inside of an array 
{
	// accelerometer reading
	int16_t accel_data_i16[3] = { 0 };// array to store the x, y and z readings.
	BSP_ACCELERO_AccGetXYZ(accel_data_i16);		// read accelerometer
	// the function above returns 16 bit integers which are 100 * acceleration_in_m/s2. Converting to float to print the actual acceleration.
	reading[3]= (float)accel_data_i16[2] / 100.0f;
	if (reading [3] < 0)
	{
		c3++;
		if(c3 == 1)
		{
			count++;
		}

	}
	// temperature sensor reading
	reading[0] = BSP_TSENSOR_ReadTemp();
	if (reading [0] > 38)
	{
		c0++;
		if(c0 == 1)
		{
			count++;
		}

	}
	
	// pressure sensor reading
	reading[1] = BSP_PSENSOR_ReadPressure();
	if(reading [1] > 2000)
	{
		c1++;
		if(c1 == 1)
		{
			count ++;
		}
	}
	
	// humidity sensor reading
	reading[2] = BSP_HSENSOR_ReadHumidity();
	if (reading[2] > 100)
	{
		c2++;
		if (c2 == 1)
		{
			count++;
		}
	}
	
	//gyroscope reading
	float gyro_data[3];
	float gyro_data_i16[3] = { 0 };
	BSP_GYRO_GetXYZ(gyro_data_i16);
	gyro_data[0] = (float)gyro_data_i16[0] / 10000.0f;
	gyro_data[1] = (float)gyro_data_i16[1] / 10000.0f;
	gyro_data[2] = (float)gyro_data_i16[2] / 10000.0f;
	reading[4] = sqrt(((gyro_data[2])*(gyro_data[2])) + ((gyro_data[1])*(gyro_data[1])) + ((gyro_data[0])*(gyro_data[0])));
	if (reading [4] > 30)
	{
		c4++;
		if(c4 == 1)
		{
			count++;
		}

	}
	
	// magnetometer reading
	float mag_data[3];
	int16_t mag_data_i16[3] = { 0 };
	BSP_MAGNETO_GetXYZ(mag_data_i16);
	mag_data[0] = (float)mag_data_i16[0] / 100.0f;
	mag_data[1] = (float)mag_data_i16[1] / 100.0f;
	mag_data[2] = (float)mag_data_i16[2] / 100.0f;
	reading[5] = sqrt(((mag_data[0])*(mag_data[0])) + ((mag_data[1])*(mag_data[1])) + ((mag_data[2])*(mag_data[2])));
	if (reading [5] > 20)
	{
		c5++;
		if(c5 == 1)
		{
			count++;
		}
	}
}

void acceinterrupt(void)
{

	uint8_t temp1,temp2, temp3,temp4, temp5;

	temp1 = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_CTRL1_XL);
	uint8_t x = 0x60;
	temp1 &= ~ x;
	temp1 |= x;
	SENSOR_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,LSM6DSL_ACC_GYRO_CTRL1_XL, temp1);

	temp2 = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_TAP_CFG1);
	uint8_t y = 0x90;
	temp2 &= ~ y;
	temp2 |= y;
	SENSOR_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,LSM6DSL_ACC_GYRO_TAP_CFG1, temp2);

	temp3 = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,LSM6DSL_ACC_GYRO_WAKE_UP_DUR);
	uint8_t z = 0x00;
	temp3 &= ~ z;
	temp3 |= z;
	SENSOR_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,LSM6DSL_ACC_GYRO_WAKE_UP_DUR, temp3);

	temp4 = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_FREE_FALL);
	uint8_t a = 0x33;
	temp4 &= ~ a;
	temp4 |= a;
	SENSOR_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,LSM6DSL_ACC_GYRO_CTRL1_XL, temp4);

	temp5 = SENSOR_IO_Read(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DSL_ACC_GYRO_MD1_CFG);
	uint8_t b = 0x10;
	temp5 &= ~ b;
	temp5 |= b;
	SENSOR_IO_Write(LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,LSM6DSL_ACC_GYRO_MD1_CFG, temp5);

}

void pressureint(void)
{
	uint8_t temp1, temp2, temp3, temp4;

	temp1 = SENSOR_IO_Read(LPS22HB_I2C_ADDRESS,LPS22HB_CTRL_REG3);
	uint8_t a= 0x01;
	temp1 &= ~a;
	temp1 |= a;
	SENSOR_IO_Write(LPS22HB_I2C_ADDRESS,LPS22HB_CTRL_REG3, temp1);

	temp3 = SENSOR_IO_Read(LPS22HB_I2C_ADDRESS, LPS22HB_THS_P_LOW_REG);
	uint8_t c = 0x20;
	temp3 &= 0x00;
	temp3 |= c;
	SENSOR_IO_Write(LPS22HB_I2C_ADDRESS,LPS22HB_THS_P_LOW_REG, temp3);

	temp4 = SENSOR_IO_Read(LPS22HB_I2C_ADDRESS, LPS22HB_THS_P_HIGH_REG);
	uint8_t d = 0x00;
	temp4 &= d;
	SENSOR_IO_Write(LPS22HB_I2C_ADDRESS,LPS22HB_THS_P_HIGH_REG, temp4);

	temp2 = SENSOR_IO_Read(LPS22HB_I2C_ADDRESS,LPS22HB_INTERRUPT_CFG_REG);
	uint8_t e = 0x89;
	temp2 &= ~e;
	temp2 |= e;
	SENSOR_IO_Write(LPS22HB_I2C_ADDRESS,LPS22HB_INTERRUPT_CFG_REG, temp2);
}


static void MX_GPIO_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE(); //Enable AHB2 Bus for GPIOB
	__HAL_RCC_GPIOC_CLK_ENABLE(); //ENable AHB2 Bus for GPIOC
	__HAL_RCC_GPIOD_CLK_ENABLE(); //ENable AHB2 Bus for GPIOC

	HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_RESET); // Reset the LED2_Pin as 0
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// Configuration of LED2_Pin (GPIO-B Pin-14) as GPIO output
	GPIO_InitStruct.Pin = LED2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Configuration of BUTTON_EXTI13_Pin (G{IO-C Pin-13)as AF
	GPIO_InitStruct.Pin = BUTTON_EXTI13_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//configuration of accelerometer
	GPIO_InitStruct.Pin = LSM6DSL_INT1_EXTI11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);



	//configuration of pressure sensor
	GPIO_InitStruct.Pin = LPS22HB_INT_DRDY_EXTI0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);




	// Enable NVIC EXTI line 10,11,13
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0x0F, 0x00);

}




#endif

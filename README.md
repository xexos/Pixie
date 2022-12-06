# PIXIE

EE2028 Final Project

Introduction 

In this project, we aim to use the STM32 B-L475E-IOT01A board to design a prototype of a system inside Pixie. The system has two modes implemented: EXPLORATION and BATTLE. On the board, there is an accelerometer, gyroscope, magnetometer, temperature sensor, pressure sensor, and humidity sensor. When the readings from these sensors exceed certain thresholds, Pixie will enter the WARNING state. A single press of the push button can allow it to get out of the WARNING state. While in EXPLORATION or BATTLE mode, a double press switches Pixie into the other mode. Sensor values are regularly uploaded to Cyrix’s Lab (Tera Term).

Both the Exploration mode and battle mode have Normal state mode and warning mode.
Under exploration mode in normal state mode, the sensors send the reading of the devices every 1s and the LED light is turned on (without blinking).
The device enters a warning mood when two of its devices are out of range. In exploration mode, "If any TWO readings exceed a certain threshold", even if the violations of threshold DO NOT have to take place simultaneously. As long as any sensor reading once violated the threshold, a log of the violation is recorded. "If any TWO readings exceed a certain threshold", even if they don't take place simultaneously. Even When the next reading returns to the normal range, a violation log is still recorded as long as any sensor reading has violated the threshold more than once. And if two of such logs from TWO DISTINCT sensors have been recorded, a warning state is activated. During this time the LED light blinks every 3s. A message “Warning state: SOS” is sent across UART during the warning state.

Under battle mode in a normal state the led is blinking at 1Hz. Pixie fires the fluxer every 5 seconds. As it fires the fluxer its battery level decreases by 2 levels. It has a max battery level of 10 and a minimum of 0.
Under battle mode, if any of the devices exceeds its threshold it enters warning mode. Under warning mode, the device stops polling and sends a message “Battle mode: SOS” every second.

The range for devices to enter warning mode
1. accelerometer less than 0 m/s^2 along the z-axis
2. temperature above 38 degrees Celcius
3. pressure above 2hpa
4. humidity above 100%
5. magnetometer reading above 20 tesla
6 . gyroscope measuring angular acceleration having a reading above 30rad/s

Under the project folder (Src), we have Battery.h, LED.h, Sensors.h, UART.h, interrupts. h and main. c.
Pixie makes use of devices such as an accelerometer, gyroscope, magnetometer, temperature sensor, pressure sensor, and humidity sensor. Installation of the devices is done under the file named Sensors. h. Inside Sensor.h there are functions named SensorInit, Device_reading, acceinterrupt, pressureint and MX_GPIO_Init. SensorInit is used to activate the devices. Device_reading is used to collect the reading of the devices and send it over to UART. We used a polling method to check device readings. The int C1, C2, C3, C4, and C5 are used to keep track of whether devices have gone out of the range while the int count keeps track of the number of devices has gone out of range to activate warning mode. Int count only increases when int C1, C2, C3, C4, and C5 are equal to one which indicates that any of the sensor readings had once violated the threshold, even if the next round of reading drops back to the normal range, a log of violation is being recorded.

Using NVIC interrupt procedure We introduced free-fall interrupt and pressure interrupt inside the board using the acceinterrupt, and pressureint functions while MX_GPIO_Init was used to instal ABH2 bus and GPIO-interrupts.

Inside the Battery.h, BatteryPrint function is used to send the level of the battery through UART. The FluxerFire function fires the Fluxer. This function fires the fluxer every 5 seconds and decreases the battery level by two levels. battery charge function is used to charge the battery. Every single press of the push button activates the BatteryCharge function which helps to increase the battery level by 1 level.

LED.h file is used to configure the led. The exploration function is called to keep the LED turned on in exploration mode while WarningLED toggles the led at 3Hz and BattleLED at 1Hz.

UART .h file is used to install UART and to send device readings to TERA Term. uart_print function ensures that under exploration mode in normal state device readings of gyroscope, magnetometer, pressure sensor and humidity sensors are sent across uart. In the warning state “warning mode: SOS” is sent. While under battle mode in normal state device readings of the temperature sensor, pressure sensor, humidity sensor, accelerometer, gyroscope and magnetometer are printed in the following order. while in the warning state “Battle mode: SOS” is sent.

The main program file is main. c where all the functions implemented in individual libraries are called to run the program on our stm32 board.
HAL_GPIO_EXTI_Callback function deals with the NVIC hardware interrupt. BUTTON_EXTI13_Pin is allocated for the push button while
LSM6DSL_INT1_EXTI11_Pin and LPS22HB_INT_DRDY_EXTI0_Pin for accelerometers and pressure sensors.

Inside main.c mode_flag represents exploration or battle mode. 0 for exploration mode and 1 for battle mode. while SOS_flag is used for state mode. 0 for the normal state and 1 for the warning state.

Int number_press keeps count of the number of presses of the push button. If number_press == 1 under 1s it is considered a single press. For a single press if Pixie is in battle mode under a normal state a single press will call the BatteryCharge function under the battery. h to charge the battery. Else if under warning mode a single press will clear the warning state by calling clear_SOS function. It sets back SOS_flag, count and c0, c1, c2, c3, c4, and c5 to zero and put the pixie back to its normal state. If number_press == 2 under 1s it is considered a double press. Double press means a mode change. If the pixie is in exploration mode and normal state it will change to battle mode while if the pixie is under battle mode and normal state a double press will change the pixie to exploration mode. Double press in Warning state in any mode is ignored. NO MODE CHANGE TAKES PLACE WITH DOUBLE PRESS.

Int count is an integer called from the sensor.h file which keeps track of devices out of range. check_SOS function changes the SOS_flag (state condition) depending on the integer count and mode_flag (mode condition) following the mode and state condition logic regarding the device reading explained above.
The main function calls all the necessary functions and executes all the logic explained above related to mode and mode, state, device reading and interrupt using the push button.

Enhancement We have implemented the interrupts for LSM6DSL and LPS22HB sensors. For the LSM6DSL sensor, we have configured it to detect free fall, which triggers the interrupt. When a free fall happens we set the WARNING flag to 1, regardless of the current mode. For LPS22HB, it is configured such that when the pressure goes below 2 hPa (0.2 kPa), an interrupt is triggered. This will also set the WARNING to 1. The configurations are made in the functions acceinterrupt() and pressureint() which are declared in Sensors. h.

Challenges Initially, we had trouble detecting single and double button presses. We used a variable named pressed to store the number of presses. The problem came when we did not reset the variable back to 0 after using it to trigger certain events. We solved this by making sure that we reset the variable to 0 after every time it is used. The use of a flow chart also helped to implement the base logic of single and double press. The implementation of sensor interrupts was also very tough. A good understanding of the data sheet and the code are needed to implement them.

Conclusion Overall, this project is very interesting and useful. We learnt how to interface the sensors with the board, set up interrupts and debug the C code. These lessons will be useful to us as we are very keen on learning more about embedded systems and IoT technologies

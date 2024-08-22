//
// Created by urdmi on 03.08.2024.
//

#ifndef STM32L4CMSIS_DEVICE_H
#define STM32L4CMSIS_DEVICE_H

#include "mcu.h"
#include "inttypes.h"

#define PIN_SWD MCU__PORT_A_PIN_13
#define PIN_SWO MCU__PORT_A_PIN_14

#define DEVICE_LED_RED          MCU__PORT_B_PIN_1
#define DEVICE_LED_GREEN        MCU__PORT_B_PIN_0
#define DEVICE_LED_BLUE         MCU__PORT_B_PIN_5

#define DEVICE_BUTTON_1          MCU__PORT_C_PIN_4
#define DEVICE_BUTTON_2          MCU__PORT_D_PIN_0
#define DEVICE_BUTTON_3          MCU__PORT_D_PIN_1

#define DEVICE_USART1_TX         MCU__PORT_A_PIN_9
#define DEVICE_USART1_RX         MCU__PORT_A_PIN_10
#define DEVICE_USART1_CTS        MCU__PORT_A_PIN_11
#define DEVICE_USART1_RTS        MCU__PORT_A_PIN_12

void device__gpio_init(void);

#endif //STM32L4CMSIS_DEVICE_H

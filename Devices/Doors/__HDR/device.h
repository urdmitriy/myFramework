//
// Created by urdmi on 03.08.2024.
//

#ifndef DEVICE_H
#define DEVICE_H

#include "mcu.h"
#include "inttypes.h"

#define PIN_SWD MCU__PORT_A_PIN_13
#define PIN_SWO MCU__PORT_A_PIN_14

#define DEVICE_PWR_WIFI         MCU__PORT_B_PIN_9
#define DEVICE_PWR_GSM          MCU__PORT_B_PIN_8

#define DEVICE_LED_WIFI         MCU__PORT_B_PIN_5
                                                 
#define DEVICE_PIN_SETUP        MCU__PORT_A_PIN_0
#define DEVICE_PIN_RELAY        MCU__PORT_B_PIN_1
#define DEVICE_PIN_DS18B20      MCU__PORT_A_PIN_6
                                                  
#define DEVICE_USART_GSM        MCU__USART_6
#define DEVICE_USART_GSM_TX     MCU__PORT_A_PIN_11
#define DEVICE_USART_GSM_RX     MCU__PORT_A_PIN_12

#define DEVICE_USART_WIFI       MCU__USART_2
#define DEVICE_USART_WIFI_TX    MCU__PORT_A_PIN_2
#define DEVICE_USART_WIFI_RX    MCU__PORT_A_PIN_3

#define DEVICE_USART_DEBUG      MCU__USART_1
#define DEVICE_USART_DEBUG_TX   MCU__PORT_A_PIN_9

void device__gpio_init(void);

#endif //DEVICE_H

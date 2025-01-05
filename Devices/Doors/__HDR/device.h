//
// Created by urdmi on 03.08.2024.
//

#ifndef DEVICE_H
#define DEVICE_H

#include "mcu.h"
#include "inttypes.h"

#define HSE_MHZ 25
#define HSI_MHZ 16
#define FREQ_MCU_HI_MHZ 84
#define FREQ_MCU_LO_MHZ 4

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

#define DEVICE_USART_LOG        MCU__USART_1
#define DEVICE_USART_LOG_TX     MCU__PORT_A_PIN_9

void device__gpio_init(void);
void device__relay_switch();

#endif //DEVICE_H

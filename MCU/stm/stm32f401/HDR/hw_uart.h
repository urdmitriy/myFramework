//
// Created by urdmi on 20.08.2024.
//

#ifndef STM32WB55_TEST_HW_UART_H
#define STM32WB55_TEST_HW_UART_H
#include "inttypes.h"

typedef enum {
    HW_UART__EVENT_INIT,
    HW_UART__EVENT_OPEN,
    HW_UART__EVENT_DATA_TX_COMPLETE,
    HW_UART__EVENT_DATA_RX,
    HW_UART__EVENT_DATA_RX_COMPLETE,
    HW_UART__EVENT_DATA_ORE,
}hw_uart__event_e;

typedef void (*hw_uart__event_handler_t)(uint8_t uart_id, hw_uart__event_e event);

void hw_uart__init(uint8_t uart_id);
void hw_uart__open(uint8_t uart_id, uint32_t baudrate, hw_uart__event_handler_t event_handler);
void hw_uart__tx_irq_en(uint8_t uart_id);
void hw_uart__rx_irq_en(uint8_t uart_id);
uint8_t hw_uart__tx(uint8_t uart_id, uint8_t data);
uint8_t hw_uart__rx(uint8_t uart_id);

#endif //STM32WB55_TEST_HW_UART_H

//
// Created by urdmi on 20.08.2024.
//

#ifndef STM32WB55_TEST_UART_H
#define STM32WB55_TEST_UART_H

#include "list.h"
#include "inttypes.h"

typedef enum {
    UART__EVENT_DATA_TX_COMPLETE,
}uart__event_e;

typedef void (*uart__call_back_t)(uint8_t uart_id, uart__event_e event);

void uart__init(list__item_t* list_head);
void uart__open(uint8_t uart_id);
void uart__tx(uint8_t uart_id, char* data, uart__call_back_t uart_cb);

#endif //STM32WB55_TEST_UART_H

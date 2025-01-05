//
// Created by urdmi on 20.08.2024.
//

#ifndef UART_H
#define UART_H

#include "list.h"
#include "inttypes.h"

typedef enum {
    UART__EVENT_DATA_TX_COMPLETE,
    UART__EVENT_DATA_RX_COMPLETE,
}uart__event_e;

typedef void (*uart__call_back_t)(uint8_t uart_id, uart__event_e event, char *buf, uint16_t len);

void uart__init(list__item_t* list_head);
void uart__open(uint8_t uart_id);
void uart__tx(uint8_t uart_id, char* data, uart__call_back_t uart_cb);
void uart__rx(uint8_t uart_id, char* data, uint32_t timeout_ms, uint32_t time_idle_ms, uint32_t len, uart__call_back_t uart_cb);
uint8_t uart__is_busy(uint8_t uart_id);

#endif //UART_H

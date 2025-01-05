//
// Created by urdmi on 04.01.2025.
//

#include "log.h"
static uint8_t log_uart_id;

void log__init(uint8_t uart_id){
    uart__open(uart_id);
    log_uart_id = uart_id;
}

void log__print(char* message){
    uart__tx(log_uart_id, message, 0);
}
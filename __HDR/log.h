//
// Created by urdmi on 04.01.2025.
//

#ifndef LOG_H
#define LOG_H
#include "uart.h"

void log__init(uint8_t uart_id);
void log__print(char* message);

#endif //LOG_H

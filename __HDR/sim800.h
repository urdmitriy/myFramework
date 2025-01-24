//
// Created by urdmi on 06.01.2025.
//

#ifndef SIM800_H
#define SIM800_H

#include "stm32f4xx.h"

typedef enum {
    SIM800__STATE_BEGIN,
    SIM800__STATE_TEST,
    SIM800__STATE_WAIT_TEST_ANSWER,
    SIM800__STATE_INIT,
    SIM800__STATE_WAIT_INIT,
    SIM800__STATE_REGISTER_NETWORK_WAIT, //AT+CCALR?
    SIM800__STATE_GET_OPSOS_NAME, //"AT+COPS?");
    SIM800__STATE_GET_SIGNAL_LEVEL, //"AT+CSQ"
    SIM800__STATE_OPEN_GPRS_SESSION,
    SIM800__STATE_WAIT_IP_ADDRESS,
    SIM800__STATE_TEST_CONNECTION,
    SIM800__STATE_WORK,
    SIM800__STATE_RESTART,
} sim800__state_e;

typedef void (*cb_gsm_t)(sim800__state_e);
void sim800_init(uint8_t uart_id, cb_gsm_t cb) ;
void sim800_cout();

#endif //SIM800_H

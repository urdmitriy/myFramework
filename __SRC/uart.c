//
// Created by urdmi on 20.08.2024.
//

#include "mcu.h"
#include "uart.h"
#include "hw_uart.h"
#include "list.h"
#include "inttypes.h"

#define SIZE_BUF 256
typedef enum {
    UART__FSM_STATE_IDLE,
    UART__FSM_STATE_OPEN,
    UART__FSM_STATE_WAIT_OPEN,
    UART__FSM_STATE_READY,
    UART__FSM_STATE_TX_DATA,
    UART__FSM_STATE_TX_DATA_WAIT_COMPLETE,
}uart_fsm_state_e;

typedef struct {
    struct {
        char data[SIZE_BUF];
        uint16_t data_count;
        uint16_t current_tx;
        uart__call_back_t cb;
    }tx;
    struct {
        char data[SIZE_BUF];
        uint16_t data_count;
        uint16_t current_rx;
        uart__call_back_t cb;
    }rx;
    uart_fsm_state_e uart_fsm_state;
} uart__data_t;

static void uart__cout(void);
static void uart__event_handler(uint8_t uart_id, hw_uart__event_e event);
void uart__cb(uint8_t uart_id, uart__event_e event);

volatile static uart__data_t uart__data[MCU__USART_COUNT];

void uart__init(list__item_t* list_head){
    for (int i = 0; i < MCU__USART_COUNT; ++i) {
        hw_uart__init(i);
        uart__open(i);
    }

    list__include(list_head, uart__cout);

}

static void uart__cout(void){
    for (int i = 0; i < MCU__USART_COUNT; ++i) {
        switch (uart__data[i].uart_fsm_state) {
            case UART__FSM_STATE_IDLE:
                break;

            case UART__FSM_STATE_OPEN:
                uart__data[i].uart_fsm_state = UART__FSM_STATE_WAIT_OPEN;
                hw_uart__open(i, uart__event_handler);
                break;

            case UART__FSM_STATE_WAIT_OPEN:
            case UART__FSM_STATE_READY:
                break;

            case UART__FSM_STATE_TX_DATA:
                if (uart__data[i].tx.data[uart__data[i].tx.current_tx] != '\0' && uart__data[i].tx.current_tx < SIZE_BUF) {
                    uart__data[i].uart_fsm_state = UART__FSM_STATE_TX_DATA_WAIT_COMPLETE;
                    hw_uart__tx(i, uart__data[i].tx.data[uart__data[i].tx.current_tx], uart__event_handler);
                    uart__data[i].tx.current_tx++;
                } else {
                    uart__data[i].uart_fsm_state = UART__FSM_STATE_READY;
                    uart__data[i].tx.cb(i, UART__EVENT_DATA_TX_COMPLETE);
                    uart__data[i].tx.current_tx = 0;
                }
                break;
            case UART__FSM_STATE_TX_DATA_WAIT_COMPLETE:
                break;
        }
    }
}

static void uart__event_handler(uint8_t uart_id, hw_uart__event_e event) {
    switch (event) {
        case HW_UART__EVENT_OPEN:
            uart__data[uart_id].uart_fsm_state = UART__FSM_STATE_READY;
            uart__tx(uart_id, "Open port success\n\r", uart__cb);
            break;

        case HW_UART__EVENT_DATA_TX_COMPLETE:
            uart__data[uart_id].uart_fsm_state = UART__FSM_STATE_TX_DATA;
            break;
    }
}
void uart__open(uint8_t uart_id) {
    if (uart__data[uart_id].uart_fsm_state == UART__FSM_STATE_IDLE) {
        uart__data[uart_id].uart_fsm_state = UART__FSM_STATE_OPEN;
    }
}

void uart__tx(uint8_t uart_id, char* data, uart__call_back_t uart_cb){
    if (uart__data[uart_id].uart_fsm_state != UART__FSM_STATE_READY) return;
    uart__data[uart_id].tx.cb = uart_cb;
    uart__data[uart_id].tx.data_count = 0;
    while (uart__data[uart_id].tx.data_count < SIZE_BUF && *(data + uart__data[uart_id].tx.data_count) != '\0'){
        uart__data[uart_id].tx.data[uart__data[uart_id].tx.data_count] = *(data + uart__data[uart_id].tx.data_count);
        uart__data[uart_id].tx.data_count++;
    }
    uart__data[uart_id].tx.data[uart__data[uart_id].tx.data_count] = '\0';
    uart__data[uart_id].uart_fsm_state = UART__FSM_STATE_TX_DATA;
}

void uart__cb(uint8_t uart_id, uart__event_e event){
    return;
}
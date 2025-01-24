//
// Created by urdmi on 20.08.2024.
//

#include "mcu.h"
#include "uart.h"
#include "hw_uart.h"
#include "inttypes.h"
#include "timer.h"
#include "stm32f4xx.h"
#include "string.h"

#define SIZE_BUF        256
#define BAUDRATE        115200

typedef enum {
    UART__FSM_STATE_IDLE,
    UART__FSM_STATE_OPEN,
    UART__FSM_STATE_WAIT_OPEN,
    UART__FSM_STATE_WORK,
}uart_fsm_state_e;

typedef struct {
    struct {
        uint8_t data[SIZE_BUF];
        uint16_t data_count;
        uint16_t current_tx;
        uart__call_back_t cb;
    }tx;
    struct {
        uint8_t data[SIZE_BUF];
        uint16_t data_count;
        uint32_t len;
        uint32_t timeout;
        uint32_t time_idle;
        uint64_t time_last_byte;
        uart__call_back_t cb;
    }rx;
    struct {
        uint8_t rx_data:1;
        uint8_t tx_data:1;
        uint8_t tx_wait_data_send:1;
    } flags;
    uart_fsm_state_e uart_fsm_state;
} uart__data_t;

static void uart__cout(void);
static void uart__event_handler(uint8_t uart_id, hw_uart__event_e event);

static uart__data_t uart__data[MCU__USART_COUNT];

void uart__init(list__item_t* list_head){
    for (int i = 0; i < MCU__USART_COUNT; ++i) {
        hw_uart__init(i);
    }
    list__include(list_head, uart__cout);
}

static void uart__cout(void) {
    for (int i = MCU__USART_1; i < MCU__USART_COUNT; ++i) {
        switch (uart__data[i].uart_fsm_state) {
            case UART__FSM_STATE_IDLE:
                break;

            case UART__FSM_STATE_OPEN:
                uart__data[i].uart_fsm_state = UART__FSM_STATE_WAIT_OPEN;
                hw_uart__open(i, BAUDRATE, uart__event_handler);
                break;

            case UART__FSM_STATE_WAIT_OPEN:
                break;

            case UART__FSM_STATE_WORK:
                //если есть чего принять
                if (uart__data[i].flags.rx_data || uart__data[i].rx.data_count != 0) {

                    uint64_t cur_time = timer__systick_get();

                    if (uart__data[i].flags.rx_data) {
                        uint8_t rx_byte = hw_uart__rx(i);
                        uart__data[i].rx.data[uart__data[i].rx.data_count] = rx_byte;
                        uart__data[i].rx.data_count++;
                        uart__data[i].flags.rx_data = 0;
                        uart__data[i].rx.time_last_byte = cur_time;
                    }

                    uint8_t ent_rcv = 0;
                    if ((uart__data[i].rx.time_idle != 0 && (cur_time - uart__data[i].rx.time_last_byte) >= uart__data[i].rx.time_idle) && uart__data[i].rx.data_count > 0) { //если задержка
                        ent_rcv = 1;
                    } else if (uart__data[i].rx.len != 0 && uart__data[i].rx.data_count >= uart__data[i].rx.len) { //если n байт уже получено
                        ent_rcv = 1;
                    } else if (uart__data[i].rx.timeout != 0 && uart__data[i].rx.timeout >= timer__systick_get()) { //если время истекло
                        ent_rcv = 1;
                    } else {
                        hw_uart__rx_irq_en(i);
                    }
                    if (ent_rcv) {
                        uart__data[i].rx.data[uart__data[i].rx.data_count] = 0;
                        uart__data[i].rx.cb(i, UART__EVENT_DATA_RX_COMPLETE, uart__data[i].rx.data, uart__data[i].rx.data_count);
                        memset(uart__data[i].rx.data, 0, uart__data[i].rx.data_count);
                        uart__data[i].rx.data_count = 0;
                    }
                }

                //если есть чего отправить
                if (uart__data[i].flags.tx_data && uart__data[i].flags.tx_wait_data_send == 0) {
                    if (uart__data[i].tx.data[uart__data[i].tx.current_tx] != '\0' &&
                        uart__data[i].tx.current_tx < SIZE_BUF) {
                        uart__data[i].flags.tx_wait_data_send = 1;
                        if (hw_uart__tx(i, uart__data[i].tx.data[uart__data[i].tx.current_tx]))
                            uart__data[i].tx.current_tx++;
                    } else {
                        uart__data[i].tx.current_tx = 0;
                        uart__data[i].flags.tx_data = 0;
                        if (uart__data[i].tx.cb != 0)
                            uart__data[i].tx.cb(i, UART__EVENT_DATA_TX_COMPLETE, uart__data[i].tx.data, uart__data[i].tx.data_count);
                    }
                }
                break;
            default:
                break;
        }
    }
}

static void uart__event_handler(uint8_t uart_id, hw_uart__event_e event) {
    switch (event) {
        case HW_UART__EVENT_OPEN:
            uart__data[uart_id].uart_fsm_state = UART__FSM_STATE_WORK;
            break;

        case HW_UART__EVENT_DATA_TX_COMPLETE:
            uart__data[uart_id].flags.tx_wait_data_send = 0;
            break;

        case HW_UART__EVENT_DATA_RX:
            uart__data[uart_id].flags.rx_data = 1;
            break;

    }
}

void uart__open(uint8_t uart_id) {
    if (uart__data[uart_id].uart_fsm_state == UART__FSM_STATE_IDLE) {
        uart__data[uart_id].uart_fsm_state = UART__FSM_STATE_OPEN;
    }
}

void uart__tx(uint8_t uart_id, uint8_t* data, uart__call_back_t uart_cb){
    if (uart__data[uart_id].uart_fsm_state != UART__FSM_STATE_WORK) return;
    uart__data[uart_id].flags.tx_data = 1;
    uart__data[uart_id].tx.cb = uart_cb;
    uart__data[uart_id].tx.data_count = 0;
    //копируем данные в буфер
    while (uart__data[uart_id].tx.data_count < SIZE_BUF && *(data + uart__data[uart_id].tx.data_count) != '\0'){
        uart__data[uart_id].tx.data[uart__data[uart_id].tx.data_count] = *(data + uart__data[uart_id].tx.data_count);
        uart__data[uart_id].tx.data_count++;
    }
    //добавляем конец строки
    uart__data[uart_id].tx.data[uart__data[uart_id].tx.data_count] = '\0';
    hw_uart__tx_irq_en(uart_id);
}

void uart__rx(uint8_t uart_id, uint8_t* data, uint32_t timeout_ms, uint32_t time_idle_ms, uint32_t len, uart__call_back_t uart_cb){
    if (uart__data[uart_id].uart_fsm_state != UART__FSM_STATE_WORK) return;

    if (timeout_ms != 0)
        uart__data[uart_id].rx.timeout = timer__systick_get() + timeout_ms;
    else
        uart__data[uart_id].rx.timeout = 0;

    if (len != 0)
        uart__data[uart_id].rx.len = len;
    else
        uart__data[uart_id].rx.len = 0;

    if (time_idle_ms != 0)
        uart__data[uart_id].rx.time_idle = time_idle_ms;
    else
        uart__data[uart_id].rx.time_idle = 0;

    uart__data[uart_id].rx.cb = uart_cb;
    hw_uart__rx_irq_en(uart_id);
}

uint8_t uart__is_busy(uint8_t uart_id) {
    return uart__data[uart_id].flags.tx_data ;
}
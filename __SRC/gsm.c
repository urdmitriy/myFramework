//
// Created by urdmi on 06.01.2025.
//

#include "gsm.h"
#include "uart.h"
#include "target.h"
#include "device.h"
#include "log.h"
#ifdef SIM800_USE
    #include "sim800.h"
#endif

static uint8_t gsm_uart_id = 0xFF;
timer__t timer_power_on;

typedef enum {
    GSM__STATE_POWER_OFF,
    GSM__STATE_POWER_ON,
    GSM__STATE_WAIT_POWER_ON,
    GSM__STATE_WORK,
    GSM__STATE_RESTART,
    GSM__STATE_WAIT_POWER_OFF,
} gsm_state_e;

static gsm_state_e gsm_state;

static void gsm__cout();
static void gsm__set_state(gsm_state_e state);
static void gsm__power_on_cb(void);
static void gsm__restart_cb(void);
static void gsm__hw_cmd_handler(sim800_state_e state);


void gsm__init(uint8_t uart_id, list__item_t *list_head) {
    uart__open(uart_id);
    gsm_uart_id = uart_id;
    list__include(list_head, gsm__cout);
}

static void gsm__cout(){
    switch (gsm_state) {
        case GSM__STATE_POWER_OFF:
            gsm__set_state(GSM__STATE_POWER_ON);
            break;
        case GSM__STATE_POWER_ON:
            device__switch_power_gsm(1);
            gsm__set_state(GSM__STATE_WAIT_POWER_ON);
            sim800_init(gsm__hw_cmd_handler);
            timer__start(&timer_power_on, 3000, TIMER__MODE_ONE_PULSE, gsm__power_on_cb);
            break;
        case GSM__STATE_WAIT_POWER_ON:
            break;
        case GSM__STATE_WORK:
            sim800_cout();
            break;
        case GSM__STATE_RESTART:
            device__switch_power_gsm(0);
            gsm__set_state(GSM__STATE_WAIT_POWER_OFF);
            timer__start(&timer_power_on, 5000, TIMER__MODE_ONE_PULSE, gsm__restart_cb);
            break;
        case GSM__STATE_WAIT_POWER_OFF:
            break;
    }
}

static void gsm__power_on_cb(void) {
    gsm__set_state(GSM__STATE_WORK);
}

static void gsm__restart_cb(void) {
    gsm__set_state(GSM__STATE_POWER_ON);
}

static void gsm__set_state(gsm_state_e state){
    gsm_state = state;
    switch (state) {
        case GSM__STATE_POWER_OFF:
            log__print("GSM set state POWER OFF\n\r");
            break;
        case GSM__STATE_POWER_ON:
            log__print("GSM set state POWER ON\n\r");
            break;
        case GSM__STATE_WAIT_POWER_ON:
            log__print("GSM set state WAIT POWER ON\n\r");
            break;
        case GSM__STATE_WORK:
            log__print("GSM set state WORK\n\r");
            break;
        case GSM__STATE_RESTART:
            log__print("GSM set state RESTART\n\r");
            break;
        case GSM__STATE_WAIT_POWER_OFF:
            log__print("GSM set state WAIT POWER OFF\n\r");
            break;
    }
}

void gsm__hw_cmd_handler(sim800_state_e state) {
    switch (state) {
        case SIM800_STATE_RESTART:
            gsm__set_state(GSM__STATE_RESTART);
            break;
        default:
            break;
    }
}
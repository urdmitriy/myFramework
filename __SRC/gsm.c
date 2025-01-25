//
// Created by urdmi on 06.01.2025.
//

#include "gsm.h"
#include "uart.h"
#include "target.h"
#include "device.h"
#include "log.h"
#include "stdio.h"
#ifdef SIM800_USE
    #include "sim800.h"
#endif

#define DELAY_MCU_ON                10
#define DELAY_GSM_ON                500

typedef enum {
    GSM__STATE_MCU_ON,
    GSM__STATE_WAIT_MCU,
    GSM__STATE_POWER_OFF,
    GSM__STATE_POWER_ON,
    GSM__STATE_WAIT_POWER_ON,
    GSM__STATE_WORK,
    GSM__STATE_RESTART,
    GSM__STATE_WAIT_POWER_OFF,
} gsm_state_e;

typedef struct {
    uint64_t number;
    uint64_t time;
}last_call_t;

static uint8_t gsm_uart_id = 0xFF;
static timer__t timer_power_on;
static last_call_t last_call;


static gsm_state_e gsm_state;

static void gsm__cout();
static void gsm__set_state(gsm_state_e state);
static void gsm__power_on_cb(void);
static void gsm__restart_cb(void);
static void gsm__hw_cmd_handler(sim800__state_e state);
static void gsm__call_handler(uint64_t number);

void gsm__init(uint8_t uart_id, list__item_t *list_head) {
    uart__open(uart_id);
    gsm_uart_id = uart_id;
    list__include(list_head, gsm__cout);
}

static void gsm__cout(){
    switch (gsm_state) {
        case GSM__STATE_MCU_ON:
            gsm__set_state(GSM__STATE_WAIT_MCU);
            timer__start(&timer_power_on, DELAY_MCU_ON, TIMER__MODE_ONE_PULSE, gsm__power_on_cb);
            break;
        case GSM__STATE_WAIT_MCU:
            break;
        case GSM__STATE_POWER_OFF:
            gsm__set_state(GSM__STATE_POWER_ON);
            break;
        case GSM__STATE_POWER_ON:
            gsm__set_state(GSM__STATE_WAIT_POWER_ON);
            device__switch_power_gsm(1);
            sim800_init(gsm_uart_id, gsm__hw_cmd_handler, gsm__call_handler);
            timer__start(&timer_power_on, DELAY_GSM_ON, TIMER__MODE_ONE_PULSE, gsm__power_on_cb);
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
    switch (gsm_state) {
        case GSM__STATE_WAIT_MCU:
            gsm__set_state(GSM__STATE_POWER_OFF);
            break;
        case GSM__STATE_WAIT_POWER_ON:
            gsm__set_state(GSM__STATE_WORK);
            break;
        default:
            break;
    }
}

static void gsm__restart_cb(void) {
    gsm__set_state(GSM__STATE_POWER_ON);
}

static void gsm__set_state(gsm_state_e state){
    gsm_state = state;
    char message[128] = {"GSM set state "};
    switch (state) {
        case GSM__STATE_MCU_ON:
            strcat(message, "MCU ON\n\r");
            break;
        case GSM__STATE_WAIT_MCU:
            strcat(message, "WAIT MCU\n\r");
            break;
        case GSM__STATE_POWER_OFF:
            strcat(message, "POWER OFF\n\r");
            break;
        case GSM__STATE_POWER_ON:
            strcat(message, "POWER ON\n\r");
            break;
        case GSM__STATE_WAIT_POWER_ON:
            strcat(message, "WAIT POWER ON\n\r");
            break;
        case GSM__STATE_WORK:
            strcat(message, "WORK\n\r");
            break;
        case GSM__STATE_RESTART:
            strcat(message, "RESTART\n\r");
            break;
        case GSM__STATE_WAIT_POWER_OFF:
            log__print("GSM set state WAIT POWER OFF\n\r");
            break;
        default:
            strcat(message, "UNKNOWN\n\r");
    }
    log__print(message);
}

void gsm__hw_cmd_handler(sim800__state_e state) {
    switch (state) {
        case SIM800__STATE_RESTART:
            gsm__set_state(GSM__STATE_RESTART);
            break;
        default:
            break;
    }
}

static void gsm__call_handler(uint64_t number) {

    uint64_t time = timer__systick_get();

    char message[64];
    sprintf(message, "Incoming call from %llu\n\r", number);
    log__print(message);

    if (last_call.number == number && (time - last_call.time) < 10000) {
        device__relay_switch();
        sim800__end_call();
    }
    last_call.number = number;
    last_call.time = time;
}
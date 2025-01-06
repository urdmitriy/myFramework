//
// Created by urdmi on 06.01.2025.
//

#include "sim800.h"
#include "timer.h"
#include "log.h"

static cb_gsm_t cb_gsm= 0;
static uint8_t sim800_wait = 0;
static timer__t timer_sim800_cout;
static void sim800_timer_wait_cb(void);
static void sim800_set_state(sim800_state_e state);

static sim800_state_e sim800_state;

void sim800_init(cb_gsm_t cb) {
    cb_gsm = cb;
}
void sim800_cout() {
    if (!sim800_wait) {
        switch (sim800_state) {
            case SIM800_STATE_BEGIN:
                sim800_set_state(SIM800_STATE_TEST);
                break;
            case SIM800_STATE_TEST:
                break;
            case SIM800_STATE_RESTART:
                log__print("SIM800 waiting RESTART...\n\r");
                break;
        }
        sim800_wait = 1;
        timer__start(&timer_sim800_cout, 1000, TIMER__MODE_ONE_PULSE, sim800_timer_wait_cb);
    }
}

static void sim800_timer_wait_cb(void) {
    sim800_wait = 0;
}

static void sim800_set_state(sim800_state_e state) {
    sim800_state = state;
    switch (state) {
        case SIM800_STATE_BEGIN:
            log__print("SIM800 set state BEGIN\n\r");
            break;
        case SIM800_STATE_TEST:
            log__print("SIM800 set state TEST\n\r");
            break;
        case SIM800_STATE_RESTART:
            log__print("SIM800 set state RESTART\n\r");
            sim800_state = SIM800_STATE_BEGIN;
            break;
    }
    cb_gsm(state);
}
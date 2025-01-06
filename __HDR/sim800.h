//
// Created by urdmi on 06.01.2025.
//

#ifndef SIM800_H
#define SIM800_H

typedef enum {
    SIM800_STATE_BEGIN,
    SIM800_STATE_TEST,
    SIM800_STATE_RESTART,
} sim800_state_e;

typedef void (*cb_gsm_t)(sim800_state_e);
void sim800_init(cb_gsm_t cb);
void sim800_cout();

#endif //SIM800_H

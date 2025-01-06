//
// Created by urdmi on 08.08.2024.
//

#ifndef TIMER_H
#define TIMER_H

#include "list.h"
#include "inttypes.h"

typedef enum {
    TIMER__MODE_STOP,
    TIMER__MODE_ONE_PULSE,
    TIMER__MODE_CONTINUE_PULSE,
} timer__mode_e;

typedef void (*timer__cb_t)(void );

typedef struct {
    uint64_t start_time_ms;
    uint64_t time_ms;
    timer__mode_e mode;
    timer__cb_t timer_cb;
}timer__t;



void timer__start(timer__t* timer, uint64_t time_ms, timer__mode_e mode, timer__cb_t timer_cb);
void timer__init(list__item_t* list_head);
uint64_t timer__systick_get(void);

#endif //TIMER_H

//
// Created by urdmi on 08.08.2024.
//

#include "timer.h"
#include "hw_timer.h"

static void timer__cout(void);

void timer__init(list__item_t* list_head){
    hw_timer__init();
    list__include(list_head, timer__cout);
}

static void timer__cout(void) {
    return;
}

uint64_t timer__systick_get(void) {
    return hw_timer__systick_get();
}

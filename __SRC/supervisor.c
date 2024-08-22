//
// Created by urdmi on 03.08.2024.
//

#include "supervisor.h"
#include "hw_supervisor.h"

static void supervisor__cout(void);

void supervisor__init(list__item_t* list_head) {
    hw_supervisor__init();
    list__include(list_head, supervisor__cout);
}

static void supervisor__cout(void) {
    hw_supervisor__iwdg_reset();
}

uint32_t supervisor__freq_get(void){
    return hw_supervisor__freq_get();
}


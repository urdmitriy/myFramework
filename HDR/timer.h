//
// Created by urdmi on 08.08.2024.
//

#ifndef STM32L4CMSIS_TIMER_H
#define STM32L4CMSIS_TIMER_H

#include "list.h"
#include "inttypes.h"

void timer__init(list__item_t* list_head);
uint64_t timer__systick_get(void);

#endif //STM32L4CMSIS_TIMER_H

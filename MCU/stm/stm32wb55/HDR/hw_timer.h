//
// Created by urdmi on 08.08.2024.
//

#ifndef STM32L4CMSIS_HW_TIMER_H
#define STM32L4CMSIS_HW_TIMER_H

#include "inttypes.h"

void hw_timer__init(void);
uint64_t hw_timer__systick_get(void);

#endif //STM32L4CMSIS_HW_TIMER_H

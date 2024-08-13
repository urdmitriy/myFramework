//
// Created by urdmi on 03.08.2024.
//

#ifndef STM32L4CMSIS_SUPERVISOR_H
#define STM32L4CMSIS_SUPERVISOR_H
#include "list.h"
#include "inttypes.h"

void supervisor__init(list__item_t* list_head);

uint32_t supervisor__freq_get(void);

#endif //STM32L4CMSIS_SUPERVISOR_H

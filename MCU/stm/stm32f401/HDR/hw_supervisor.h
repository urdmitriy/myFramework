//
// Created by urdmi on 03.08.2024.
//

#ifndef STM32L4CMSIS_HW_SUPERVISOR_H
#define STM32L4CMSIS_HW_SUPERVISOR_H
#include "inttypes.h"
#include "device.h"

void hw_supervisor__init(void);
uint32_t hw_supervisor__freq_get(void);
void hw_supervisor__iwdg_reset(void);

#endif //STM32L4CMSIS_HW_SUPERVISOR_H

//
// Created by urdmi on 03.08.2024.
//

#ifndef STM32L4CMSIS_GPIO_H
#define STM32L4CMSIS_GPIO_H
#include "list.h"
#include "inttypes.h"
#include "hw_gpio.h"

//mode:
// 0b00000000
//         ^^ - port
//       ^^   - pin


void gpio__init(list__item_t* list_head);
static void gpio__cout(void);
void gpio__pin_init(uint32_t pin, hw_gpio__settings_t* settings);
void gpio__pin_state_set(uint32_t port_pin, hw__gpio_state_e state);
hw__gpio_state_e gpio__pin_state_get(uint32_t port_pin);
#endif //STM32L4CMSIS_GPIO_H

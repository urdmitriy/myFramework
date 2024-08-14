//
// Created by urdmi on 03.08.2024.
//

#ifndef STM32L4CMSIS_HW_GPIO_H
#define STM32L4CMSIS_HW_GPIO_H

#include "stm32wb55xx.h"

typedef enum {
    HW_GPIO__MODE_INPUT,
    HW_GPIO__MODE_OUTPUT,
    HW_GPIO__MODE_ANALOG,
    HW_GPIO__MODE_ALTERNATIVE,
} hw__gpio_mode_e;

typedef enum {
    HW_GPIO__SPEED_LOW,
    HW_GPIO__SPEED_MEDIUM,
    HW_GPIO__SPEED_HIGH,
    HW_GPIO__SPEED_VERY_HIGH,
} hw__gpio_speed_e;

typedef enum {
    HW_GPIO__PULL_NONE,
    HW_GPIO__PULL_UP,
    HW_GPIO__PULL_DOWN,
} hw__gpio_pull_e;

typedef struct {
    hw__gpio_mode_e mode;
    hw__gpio_speed_e speed;
    hw__gpio_pull_e pull;
} hw_gpio__settings_t;

typedef enum {
    HW_GPIO__STATE_LOW,
    HW_GPIO__STATE_HIGH,
} hw__gpio_state_e;

void hw_gpio__init(void);
void hw_gpio__pin_init(uint32_t pin, hw_gpio__settings_t* settings);
void hw_gpio__pin_state_set(uint32_t port_pin, hw__gpio_state_e state);
hw__gpio_state_e hw_gpio__pin_state_get(uint32_t port_pin);
#endif //STM32L4CMSIS_HW_GPIO_H

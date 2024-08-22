//
// Created by urdmi on 03.08.2024.
//
#include "gpio.h"
#include "hw_gpio.h"
#include "hw_timer.h"
#include "device.h"
#include "uart.h"

static uint64_t time_on, time_off;
void uart__event_handler(uart__event_e event);

void gpio__init(list__item_t* list_head){
    hw_gpio__init();
    device__gpio_init();
    list__include(list_head, gpio__cout);
}

static void gpio__cout(void){
    if (hw_timer__systick_get() > (time_on + 1000)){
        gpio__pin_state_set(DEVICE_LED_GREEN, HW_GPIO__STATE_HIGH);

        time_on = hw_timer__systick_get();
        time_off = time_on + 50;
    }

    if (gpio__pin_state_get(DEVICE_LED_GREEN) == HW_GPIO__STATE_HIGH && (hw_timer__systick_get() > time_off)){
        gpio__pin_state_set(DEVICE_LED_GREEN, HW_GPIO__STATE_LOW);
    }
}


void gpio__pin_init(uint32_t pin, hw_gpio__settings_t* settings){
    hw_gpio__pin_init(pin, settings);
}

void gpio__pin_state_set(uint32_t port_pin, hw__gpio_state_e state){
    hw_gpio__pin_state_set(port_pin, state);
}
hw__gpio_state_e gpio__pin_state_get(uint32_t port_pin){
    return hw_gpio__pin_state_get(port_pin);
}
//
// Created by urdmi on 03.08.2024.
//
#include "gpio.h"
#include "hw_gpio.h"
#include "hw_timer.h"
#include "device.h"

static uint64_t time_on, time_off;

void gpio__init(list__item_t* list_head){
    hw_gpio__init();

    //SWD:
    hw_gpio__settings_t pin_setting;
    pin_setting.mode = HW_GPIO__MODE_ALTERNATIVE;
    gpio__pin_init(PIN_SWD, &pin_setting);
    gpio__pin_init(PIN_SWO, &pin_setting);

    //PIN LED:
    pin_setting.mode = HW_GPIO__MODE_OUTPUT;
    pin_setting.speed = HW_GPIO__SPEED_LOW;

    gpio__pin_init(PIN_LED, &pin_setting);

    list__include(list_head, gpio__cout);
}

static void gpio__cout(void){
    if (hw_timer__systick_get() > (time_on + 1000)){
        gpio__pin_state_set(PIN_LED, HW_GPIO__STATE_HIGH);

        time_on = hw_timer__systick_get();
        time_off = time_on + 50;
    }

    if (gpio__pin_state_get(PIN_LED) == HW_GPIO__STATE_HIGH && (hw_timer__systick_get() > time_off)){
        gpio__pin_state_set(PIN_LED, HW_GPIO__STATE_LOW);
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
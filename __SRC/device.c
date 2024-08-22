//
// Created by urdmi on 03.08.2024.
//

#include "device.h"
#include "gpio.h"

void device__gpio_init(void){

    hw_gpio__settings_t pin_setting;

    //leds:
    pin_setting.mode = HW_GPIO__MODE_OUTPUT;
    pin_setting.speed = HW_GPIO__SPEED_LOW;

    gpio__pin_init(DEVICE_LED_RED, &pin_setting);
    gpio__pin_init(DEVICE_LED_GREEN, &pin_setting);
    gpio__pin_init(DEVICE_LED_BLUE, &pin_setting);

    //buttons
    pin_setting.mode = HW_GPIO__MODE_INPUT;
    pin_setting.pull = HW_GPIO__PULL_UP;
    gpio__pin_init(DEVICE_BUTTON_1, &pin_setting);
    gpio__pin_init(DEVICE_BUTTON_2, &pin_setting);
    gpio__pin_init(DEVICE_BUTTON_3, &pin_setting);
}

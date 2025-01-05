//
// Created by urdmi on 03.08.2024.
//

#include "device.h"
#include "gpio.h"
#include "timer.h"
#include "log.h"

#define TIME_RELAY_ON_DELAY 300

static timer__t timer_relay_delay;

void device__gpio_init(void){

    hw_gpio__settings_t pin_setting;

    //leds:
    pin_setting.mode = HW_GPIO__MODE_OUTPUT;
    pin_setting.speed = HW_GPIO__SPEED_LOW;
    gpio__pin_init(DEVICE_LED_WIFI, &pin_setting);

    //outputs:
    pin_setting.mode = HW_GPIO__MODE_OUTPUT;
    pin_setting.speed = HW_GPIO__SPEED_LOW;
    gpio__pin_init(DEVICE_PWR_GSM, &pin_setting);
    gpio__pin_init(DEVICE_PWR_WIFI, &pin_setting);
    gpio__pin_init(DEVICE_PIN_RELAY, &pin_setting);

    //inputs
    pin_setting.mode = HW_GPIO__MODE_INPUT;
    pin_setting.pull = HW_GPIO__PULL_UP;
    gpio__pin_init(DEVICE_PIN_SETUP, &pin_setting);
}

static void timer_relay_on_cb () {
    gpio__pin_state_set(DEVICE_PIN_RELAY, HW_GPIO__STATE_LOW);
}

void device__relay_switch(){
    gpio__pin_state_set(DEVICE_PIN_RELAY, HW_GPIO__STATE_HIGH);
    log__print("Relay switch!\n\r");
    timer__start(&timer_relay_delay, TIME_RELAY_ON_DELAY, TIMER__MODE_ONE_PULSE, timer_relay_on_cb);
}

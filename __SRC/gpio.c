//
// Created by urdmi on 03.08.2024.
//

#include "gpio.h"
#include "hw_gpio.h"
#include "hw_timer.h"
#include "device.h"
#include "timer.h"
#include "uart.h"

static  timer__t timer_wifi_led;

void timer_wifi_led_on_cb();
void timer_wifi_led_off_cb();

void timer_wifi_led_off_cb() {
    gpio__pin_state_set(DEVICE_LED_WIFI, HW_GPIO__STATE_HIGH);
    timer__start(&timer_wifi_led, 10, TIMER__MODE_ONE_PULSE, timer_wifi_led_on_cb);
}

void timer_wifi_led_on_cb () {
    gpio__pin_state_set(DEVICE_LED_WIFI, HW_GPIO__STATE_LOW);
    timer__start(&timer_wifi_led, 500, TIMER__MODE_ONE_PULSE, timer_wifi_led_off_cb);
}

void gpio__init(list__item_t* list_head){
    hw_gpio__init();
    device__gpio_init();
    list__include(list_head, gpio__cout);

    gpio__pin_state_set(DEVICE_LED_WIFI, HW_GPIO__STATE_HIGH);
    timer__start(&timer_wifi_led, 5000, TIMER__MODE_ONE_PULSE, timer_wifi_led_on_cb);
}

static void gpio__cout(void){

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
//
// Created by urdmi on 03.08.2024.
//
#include "stm32wb55xx.h"
#include "hw_gpio.h"
#include "mcu.h"

static GPIO_TypeDef* hw_gpio__port_get(uint32_t port_pin);

void hw_gpio__init(void){
    return;
}

static GPIO_TypeDef* hw_gpio__port_get(uint32_t port_pin) {
    uint8_t port = port_pin & 0x07;
    GPIO_TypeDef *gpio_port;

    switch (port) {
        case MCU__GPIO_PORT_A:
            gpio_port = GPIOA;
            break;
        case MCU__GPIO_PORT_B:
            gpio_port = GPIOB;
            break;
        case MCU__GPIO_PORT_C:
            gpio_port = GPIOC;
            break;
		case MCU__GPIO_PORT_D:
            gpio_port = GPIOD;
            break;
		case MCU__GPIO_PORT_E:
            gpio_port = GPIOE;
            break;
    }
    return gpio_port;
}

void hw_gpio__pin_init(uint32_t port_pin, hw_gpio__settings_t* settings) {

    uint8_t pin = (port_pin >> 3) & 0x0F;

    GPIO_TypeDef* gpio_port = hw_gpio__port_get(port_pin);

    switch ((uint32_t)gpio_port) {
        case (uint32_t)GPIOA:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
            break;
        case (uint32_t)GPIOB:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
            break;
        case (uint32_t)GPIOC:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
            break;
		case (uint32_t)GPIOD:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
            break;
		case (uint32_t)GPIOE:
            RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
            break;
    }

	gpio_port->MODER    &= ~(3 << pin * 2);
	gpio_port->OSPEEDR  &= ~(3 << pin * 2);
	gpio_port->PUPDR    &= ~(3 << pin * 2);
	
    switch (settings->mode) {
        case HW_GPIO__MODE_INPUT:
            if (settings->pull == HW_GPIO__PULL_UP) {
                gpio_port->PUPDR |= (1 << pin * 2);
            } else if (settings->pull == HW_GPIO__PULL_DOWN) {
                gpio_port->PUPDR |= (2 << pin * 2);
            }
            break;

        case HW_GPIO__MODE_OUTPUT:
			
            gpio_port->MODER |= (1 << pin * 2);
            if (settings->speed == HW_GPIO__SPEED_MEDIUM) {
                gpio_port->OSPEEDR |= (1 << pin * 2);
            } else if (settings->speed == HW_GPIO__SPEED_HIGH) {
                gpio_port->OSPEEDR |= (2 << pin * 2);
            } else if (settings->speed == HW_GPIO__SPEED_VERY_HIGH) {
                gpio_port->OSPEEDR |= (3 << pin * 2);
            }
            break;

        case HW_GPIO__MODE_ALTERNATIVE:
            gpio_port->MODER |= (2 << pin * 2);
            break;

        case HW_GPIO__MODE_ANALOG:
            gpio_port->MODER |= (3 << pin * 2);
            break;
    }
}

void hw_gpio__pin_state_set(uint32_t port_pin, hw__gpio_state_e state) {

    uint8_t pin = (port_pin >> 3) & 0x0F;
    GPIO_TypeDef* gpio_port = hw_gpio__port_get(port_pin);

    if (state == HW_GPIO__STATE_HIGH) {
        gpio_port->ODR |= (1<<pin);
    } else {
        gpio_port->ODR &= ~(1<<pin);
    }
}

hw__gpio_state_e hw_gpio__pin_state_get(uint32_t port_pin){

    uint8_t pin = (port_pin >> 3) & 0x0F;
    GPIO_TypeDef* gpio_port = hw_gpio__port_get(port_pin);

    uint32_t result = gpio_port->IDR & (1<<pin);
    return result == 0 ? HW_GPIO__STATE_LOW : HW_GPIO__STATE_HIGH;
}
//
// Created by urdmi on 20.08.2024.
//

#include "hw_uart.h"
#include "hw_gpio.h"
#include "device.h"
#include "hw_supervisor.h"

void USART1_IRQHandler(void);
static USART_TypeDef* hw_usart__itf_get(uint32_t uart_id);
hw_uart__event_handler_t hw_uart__event_handler;

void hw_uart__init(uint8_t uart_id) {
    USART_TypeDef* usart = hw_usart__itf_get(uart_id);

    hw_gpio__settings_t gpio_settings;
    gpio_settings.mode = HW_GPIO__MODE_ALTERNATIVE;

    if (usart == USART1) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

        hw_gpio__pin_init(DEVICE_USART1_TX, &gpio_settings);
        hw_gpio__pin_init(DEVICE_USART1_RX, &gpio_settings);

        GPIOA->AFR[1] |= (7 << GPIO_AFRH_AFSEL9_Pos);
        GPIOA->AFR[1] |= (7 << GPIO_AFRH_AFSEL10_Pos);
    }
}

void hw_uart__open(uint8_t uart_id, hw_uart__event_handler_t event_handler){
    USART_TypeDef* usart = hw_usart__itf_get(uart_id);
    hw_uart__event_handler = event_handler;
    usart->CR1 = 0;

    uint16_t div = hw_supervisor__freq_get()/115200/16;
    uint16_t div_d = (uint16_t)(hw_supervisor__freq_get()/115200 % 16);
    usart->BRR = (div << 4) | div_d;

    usart->CR1 |= (USART_CR1_UE); //USART enable

    NVIC_EnableIRQ(USART1_IRQn);
    event_handler(uart_id, HW_UART__EVENT_OPEN);
}

void hw_uart__tx(uint8_t uart_id, char data, hw_uart__event_handler_t event_handler){
    USART_TypeDef* usart = hw_usart__itf_get(uart_id);
    usart->CR1 |= (USART_CR1_TE | USART_CR1_TXEIE);
    usart->TDR = data;

}

void USART1_IRQHandler(void) {
    if (USART1->ISR & USART_ISR_TC) {
        USART1->ICR |= USART_ICR_TCCF;
    }  else if (USART1->ISR & USART_ISR_TXE) {
        hw_uart__event_handler(0, HW_UART__EVENT_DATA_TX_COMPLETE);
        USART1->CR1 &= ~(USART_CR1_TXEIE);
    }
}

static USART_TypeDef* hw_usart__itf_get(uint32_t uart_id) {
    USART_TypeDef *usart_itf;

    switch (uart_id) {
        case MCU__USART_1:
            usart_itf = USART1;
            break;

    }
    return usart_itf;
}
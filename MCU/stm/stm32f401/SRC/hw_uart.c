//
// Created by urdmi on 20.08.2024.
//

#include "hw_uart.h"
#include "hw_gpio.h"
#include "device.h"
#include "hw_supervisor.h"

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART6_IRQHandler(void);
void USART_IRQHandler(USART_TypeDef* usart);

static USART_TypeDef* hw_usart__itf_get(uint32_t uart_id);

hw_uart__event_handler_t hw_uart__event_handler;

void hw_uart__init(uint8_t uart_id) {
    hw_gpio__settings_t gpio_settings;
    gpio_settings.mode = HW_GPIO__MODE_ALTERNATIVE;

    if (uart_id == DEVICE_USART_GSM) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        hw_gpio__pin_init(DEVICE_USART_GSM_TX, &gpio_settings);
        hw_gpio__pin_init(DEVICE_USART_GSM_RX, &gpio_settings);
        GPIOA->AFR[1] |= (8 << GPIO_AFRH_AFSEL11_Pos);
        GPIOA->AFR[1] |= (8 << GPIO_AFRH_AFSEL12_Pos);
    } else if (uart_id == DEVICE_USART_WIFI) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        hw_gpio__pin_init(DEVICE_USART_WIFI_TX, &gpio_settings);
        hw_gpio__pin_init(DEVICE_USART_WIFI_RX, &gpio_settings);
        GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos);
        GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL3_Pos);
    } else if (uart_id == DEVICE_USART_LOG) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        hw_gpio__pin_init(DEVICE_USART_LOG_TX, &gpio_settings);
        GPIOA->AFR[1] |= (7 << GPIO_AFRH_AFSEL9_Pos);
    }
}

void hw_uart__open(uint8_t uart_id, uint32_t baudrate, hw_uart__event_handler_t event_handler){
    USART_TypeDef* usart = hw_usart__itf_get(uart_id);
    hw_uart__event_handler = event_handler;
    usart->CR1 = 0;

    uint16_t div = hw_supervisor__freq_get()/baudrate/16;
    uint16_t div_d = (uint16_t)(hw_supervisor__freq_get()/baudrate % 16);
    usart->BRR = (div << 4) | div_d;
    usart->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE); //USART enable

    switch (uart_id) {
        case MCU__USART_1:
            NVIC_EnableIRQ(USART1_IRQn);
            break;
        case MCU__USART_2:
            NVIC_EnableIRQ(USART2_IRQn);
            break;
        case MCU__USART_6:
            NVIC_EnableIRQ(USART6_IRQn);
            break;
    }

    event_handler(uart_id, HW_UART__EVENT_OPEN);
}

void hw_uart__tx_irq_en(uint8_t uart_id){
    USART_TypeDef* usart = hw_usart__itf_get(uart_id);
    usart->CR1 |= USART_CR1_TXEIE;
}

void hw_uart__rx_irq_en(uint8_t uart_id){
    USART_TypeDef* usart = hw_usart__itf_get(uart_id);
    usart->CR1 |= USART_CR1_RXNEIE;
}

uint8_t hw_uart__tx(uint8_t uart_id, char data){
    USART_TypeDef* usart = hw_usart__itf_get(uart_id);
        usart->CR1 |= USART_CR1_TXEIE;
        usart->DR = data;
        return 1;
}

char hw_uart__rx(uint8_t uart_id){
    USART_TypeDef* usart = hw_usart__itf_get(uart_id);
    return usart->DR;
}

void USART1_IRQHandler(void) {
    USART_IRQHandler(USART1);
}

void USART2_IRQHandler(void) {
    USART_IRQHandler(USART2);
}

void USART6_IRQHandler(void) {
    USART_IRQHandler(USART6);
}

void USART_IRQHandler(USART_TypeDef* usart) {
    if (usart->SR & USART_SR_TC) {
        usart->SR &= ~USART_SR_TC;
        usart->CR1 &= ~(USART_CR1_TCIE);
    }

    if (usart->SR & USART_SR_TXE) {
        hw_uart__event_handler(0, HW_UART__EVENT_DATA_TX_COMPLETE);
        usart->CR1 &= ~(USART_CR1_TXEIE);
    }

    if (usart->SR & USART_SR_RXNE) {
        hw_uart__event_handler(0, HW_UART__EVENT_DATA_RX);
        usart->CR1 &= ~(USART_CR1_RXNEIE);
    }
}

static USART_TypeDef* hw_usart__itf_get(uint32_t uart_id) {
    USART_TypeDef *usart_itf;

    switch (uart_id) {
        case MCU__USART_1:
            usart_itf = USART1;
            break;
        case MCU__USART_2:
            usart_itf = USART2;
            break;
        case MCU__USART_6:
            usart_itf = USART6;
            break;
    }
    return usart_itf;
}
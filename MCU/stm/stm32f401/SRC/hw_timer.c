//
// Created by urdmi on 08.08.2024.
//

#include "hw_timer.h"
#include "hw_supervisor.h"
#include "stm32f4xx.h"

static uint64_t systick = 0;

void TIM1_UP_TIM10_IRQHandler(void);

void hw_timer__init(void){
    RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
    TIM10->DIER |= TIM_DIER_UIE;
    TIM10->CNT = 0;
	uint32_t freq = hw_supervisor__freq_get();
    TIM10->PSC = (uint16_t)(freq / 1000000 - 1);
    TIM10->ARR = 1000 - 1;

    TIM10->CR1 |= TIM_CR1_CEN; //Counter enable
    NVIC_EnableIRQ( TIM1_UP_TIM10_IRQn);
}

uint64_t hw_timer__systick_get(void){
    return systick;
}

void TIM1_UP_TIM10_IRQHandler(void){
	TIM10->SR &= ~ TIM_SR_UIF;
    systick++;
}

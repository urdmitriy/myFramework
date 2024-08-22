//
// Created by urdmi on 08.08.2024.
//

#include "hw_timer.h"
#include "hw_supervisor.h"
#include "stm32wb55xx.h"

static uint64_t systick = 0;

void TIM1_UP_TIM16_IRQHandler(void);

void hw_timer__init(void){
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
    TIM16->DIER |= TIM_DIER_UIE;
    TIM16->CNT = 0;
	uint32_t freq = hw_supervisor__freq_get();
    TIM16->PSC = (uint16_t)(freq / 1000000 - 1);
    TIM16->ARR = 1000 - 1;

    TIM16->CR1 |= TIM_CR1_CEN; //Counter enable
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

    SysTick->LOAD = 80000000/1000-1;
    SysTick->VAL = 80000000/1000-1;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;

}

uint64_t hw_timer__systick_get(void){
    return systick;
}

void TIM1_UP_TIM16_IRQHandler(void){
	TIM16->SR &= ~ TIM_SR_UIF;
    systick++;
}

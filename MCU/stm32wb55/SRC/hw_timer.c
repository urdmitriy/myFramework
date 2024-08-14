//
// Created by urdmi on 08.08.2024.
//

#include "hw_timer.h"
#include "hw_supervisor.h"
#include "stm32wb55xx.h"

static uint64_t systick = 0;

void TIM6_DAC_IRQHandler(void);

void hw_timer__init(void){
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CNT = 0;
	uint32_t freq = hw_supervisor__freq_get();
    TIM2->PSC = (uint16_t)(freq / 1000000 - 1);
    TIM2->ARR = 1000 - 1;

    TIM2->CR1 |= TIM_CR1_CEN; //Counter enable
    NVIC_EnableIRQ(TIM2_IRQn);

    SysTick->LOAD = 80000000/1000-1;
    SysTick->VAL = 80000000/1000-1;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;

}

uint64_t hw_timer__systick_get(void){
    return systick;
}

void TIM6_DAC_IRQHandler(void){
	TIM2->SR &= ~ TIM_SR_UIF;
    systick++;
}

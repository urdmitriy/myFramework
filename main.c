//
// Created by urdmi on 03.08.2024.
//

#include "list.h"
#include "supervisor.h"
#include "timer.h"
#include "gpio.h"
#include "target.h"

static list__item_t list_app;

static void init(void);
static void run_app(void);
void SysTick_Handler(void);
void HardFault_Handler(void);

int main(void) {

    init();
	__enable_irq();
    NVIC_EnableIRQ(HardFault_IRQn);

    while (1) {

       run_app();
        GPIOA->ODR &= ~GPIO_ODR_OD8;

    }
    return 0;
}

static void init(void) {
    list__init(&list_app);
#ifdef SUPERVISOR_USE
    supervisor__init(&list_app);
#endif
#ifdef TIMER_USE
    timer__init(&list_app);
#endif
#ifdef GPIO_USE
    gpio__init(&list_app);
#endif
}

static void run_app(void) {
    list__item_t* cur_app = &list_app;
    do {
		cur_app = cur_app->next_list;
        if (cur_app->app != 0)
            cur_app->app();
    } while (cur_app->next_list != cur_app);
}

void SysTick_Handler(void) {
    __NOP();
}
void HardFault_Handler(void){
	__NOP();
}

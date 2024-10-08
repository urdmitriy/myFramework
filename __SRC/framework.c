//
// Created by urdmi on 20.08.2024.
//

#include "framework.h"
#include "list.h"
#include "supervisor.h"
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "target.h"
#include "mcu.h"

static list__item_t list_app;

void framework__init(void) {
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
#ifdef UART_USE
    uart__init(&list_app);
#endif
    __enable_irq();
    NVIC_EnableIRQ(HardFault_IRQn);
}

void framework__app(void) {
    list__item_t* cur_app = &list_app;
    do {
        cur_app = cur_app->next_list;
        if (cur_app->app != 0)
            cur_app->app();
    } while (cur_app->next_list != cur_app);
}
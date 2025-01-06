//
// Created by urdmi on 20.08.2024.
//

#include "device.h"
#include "framework.h"
#include "supervisor.h"
#include "timer.h"
#include "uart.h"
#include "log.h"
#include "target.h"
#include "gsm.h"

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
#if defined(LOG_USE)
    #ifndef DEVICE_USART_LOG
    #error "Not defined DEVICE_USART_LOG"
    #endif
    log__init(DEVICE_USART_LOG, &list_app);
    log__print("Device start\n\r");
#endif

#ifdef SIM800_USE
    #ifndef DEVICE_USART_GSM
    #error "Not defined DEVICE_USART_GSM"
    #endif
    gsm__init(DEVICE_USART_GSM, &list_app);
#endif
}

void framework__app(void) {
    list__item_t* cur_app = &list_app;
    do {
        cur_app = cur_app->next_list;
        if (cur_app->item_ptr != 0) {
            app_cout cur_cout = (app_cout)cur_app->item_ptr;
            cur_cout();
        }

    } while (cur_app->next_list != cur_app);
}
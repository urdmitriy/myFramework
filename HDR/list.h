//
// Created by urdmi on 04.08.2024.
//

#ifndef STM32L4CMSIS_LIST_H
#define STM32L4CMSIS_LIST_H

typedef void (*app_cout)(void);

typedef struct list__item_t{
    struct list__item_t* next_list;
    struct list__item_t* prev_list;
    app_cout app;
}list__item_t;

void list__init(list__item_t* list_head);
void list__include(list__item_t* list_head, app_cout app);

#endif //STM32L4CMSIS_LIST_H

//
// Created by urdmi on 04.08.2024.
//

#ifndef LIST_H
#define LIST_H

typedef void (*app_cout)(void);

typedef struct list__item_t{
    struct list__item_t* next_list;
    struct list__item_t* prev_list;
    void* item_ptr;
}list__item_t;

void list__init(list__item_t* list_head);
void list__include(list__item_t* list_head, void* app);
void list__exclude(list__item_t* list_head, list__item_t* item);

#endif //LIST_H

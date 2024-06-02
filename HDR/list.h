//
// Created by urdmi on 02.06.2024.
//

#ifndef LIST_H
#define LIST_H

typedef struct list__item_t {
    struct list__item_t* prev_list;
    struct list__item_t* next_list;
}list__item_t;

typedef enum {
    LIST_HEAD,
    LIST_TAIL
}list__order_e;

void* list__init(void* list);
void* list__add_item(void* list_item, void* new_item, list__order_e order);
#endif //LIST_H

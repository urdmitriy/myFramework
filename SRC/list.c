//
// Created by urdmi on 02.06.2024.
//

#include "list.h"

void* list__init(void* list){
    ((list__item_t*)list)->prev_list = ((list__item_t*)list)->next_list = list;
    return list;
}

void* list__add_item(void* list_item, void* new_item, list__order_e order){
    if (list_item == 0 || new_item == 0)
        return 0;

    switch (order) {
        case LIST_HEAD:{
            ((list__item_t*) new_item)->prev_list = ((list__item_t*) list_item);
            ((list__item_t*) new_item)->next_list = ((list__item_t*) list_item)->next_list;
            ((list__item_t*) list_item)->next_list = ((list__item_t*) new_item);
            ((list__item_t*) new_item)->next_list->prev_list = ((list__item_t*) new_item);     }
            break;
        case LIST_TAIL:{
            ((list__item_t*) new_item)->next_list = ((list__item_t*) list_item);
            ((list__item_t*) new_item)->prev_list = ((list__item_t*) list_item)->prev_list;
            ((list__item_t*) list_item)->prev_list = ((list__item_t*) new_item);
            ((list__item_t*) new_item)->prev_list->next_list = ((list__item_t*) new_item);
        }
            break;
        default:
            break;
    }
}
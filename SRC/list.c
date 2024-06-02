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

void* list__exclude(void* list_item, list__order_e order){
    list__item_t* item = 0;
    if (list_item != 0 && ((list__item_t*) list_item)->next_list != ((list__item_t*) list_item)) {
        switch (order) {
            case LIST_HEAD:
                item = ((list__item_t*) list_item)->next_list;
                break;
            case LIST_TAIL:
                item = ((list__item_t*) list_item)->prev_list;
                break;
            case LIST_THIS:
                item = ((list__item_t*) list_item);
                break;
        }
        ((list__item_t*) item)->prev_list->next_list = ((list__item_t*) item)->next_list;
        ((list__item_t*) item)->next_list->prev_list = ((list__item_t*) item)->prev_list;
        ((list__item_t*) item)->next_list = ((list__item_t*) item)->prev_list = 0;
    }
    return item;
}

void* list__foreach(void *list, void *prev_item, list__order_e order) {
    if (prev_item == 0)
        prev_item = ((list__item_t*) list);
    switch ((int) order) {
        case LIST_HEAD:
            return ((list__item_t*) prev_item)->next_list == list ? 0 : ((list__item_t*) prev_item)->next_list;
        case LIST_TAIL:
            return ((list__item_t*) prev_item)->prev_list == list ? 0 : ((list__item_t*) prev_item)->prev_list;
    }
    return 0;
}

int list__is_empty(void *list) {
    if (((list__item_t*) list)->prev_list == (list__item_t*) list)
        return 1;
    return 0;
}

int list__count(void *list) {
    int i = 0;
    for (list__item_t* item = list; (item = item->next_list) != list; i++);
    return i;
}
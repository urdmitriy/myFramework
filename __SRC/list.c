//
// Created by urdmi on 04.08.2024.
//

#include "list.h"
#include "stdlib.h"

void list__init(list__item_t* list_head){
    list_head->next_list = list_head;
    list_head->prev_list = list_head;
}

void list__include(list__item_t* list_head, app_cout app){
    list__item_t* cur_list_item = list_head;
    while (cur_list_item != cur_list_item->next_list) {
        cur_list_item = cur_list_item->next_list;
    }

    list__item_t* new_item_list = malloc(sizeof (list__item_t));

    cur_list_item->next_list = new_item_list;
    new_item_list->next_list = new_item_list;
    new_item_list->prev_list = cur_list_item;
    new_item_list->app = app;
}

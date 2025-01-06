//
// Created by urdmi on 04.08.2024.
//

#include "list.h"
#include "stdlib.h"
#include "stm32f4xx.h"

void list__init(list__item_t* list_head){
    list_head->next_list = list_head;
    list_head->prev_list = list_head;
}

void list__include(list__item_t *list_head, void *app) {
    if (list_head != 0 && app != 0) {
        list__item_t *cur_list_item = list_head;
        while (cur_list_item != cur_list_item->next_list) {
            cur_list_item = cur_list_item->next_list;
        }

        list__item_t *new_item_list = malloc(sizeof(list__item_t));
        if (new_item_list != 0) {
            cur_list_item->next_list = new_item_list;
            new_item_list->next_list = new_item_list;
            new_item_list->prev_list = cur_list_item;
            new_item_list->item_ptr = app;
        }
    }
}

void list__exclude(list__item_t* list_head, list__item_t* item){
    list__item_t* cur_list_item = list_head;
    while (cur_list_item != cur_list_item->next_list) {
        cur_list_item = cur_list_item->next_list;
        if (cur_list_item == item) {
            if (cur_list_item->next_list != cur_list_item) {
                cur_list_item->prev_list->next_list = cur_list_item->next_list;
                cur_list_item->next_list->prev_list = cur_list_item->prev_list;
            } else {
                cur_list_item->prev_list->next_list = cur_list_item->prev_list;
            }
            free(cur_list_item);
            break;
        }
    }
}
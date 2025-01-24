//
// Created by urdmi on 08.08.2024.
//

#include "timer.h"
#include "hw_timer.h"

static list__item_t list_timer;

static void timer__cout(void);

void timer__init(list__item_t* list_head){
    hw_timer__init();
    list__include(list_head, timer__cout);
    list__init(&list_timer);
}

static void timer__cout(void) {
//    fignya!;
    if (&list_timer == list_timer.next_list) return; //если список пустой - выходим
    list__item_t* cur_item_list_timer = &list_timer;
    do {
        cur_item_list_timer = cur_item_list_timer->next_list;
        timer__t *cur_timer = (timer__t*)(cur_item_list_timer->item_ptr);
        uint64_t cur_time = timer__systick_get();

        if (cur_timer != 0 && (cur_time - cur_timer->start_time_ms) >= cur_timer->time_ms) {
            if (cur_timer->timer_cb != 0)
                cur_timer->timer_cb();
            if (cur_timer->mode == TIMER__MODE_ONE_PULSE) {
                list__exclude(&list_timer, cur_item_list_timer);
                break;
            } else {
                cur_timer->start_time_ms = timer__systick_get();
            }
        }
    } while (cur_item_list_timer != cur_item_list_timer->next_list);
}

uint64_t timer__systick_get(void) {
    return hw_timer__systick_get();
}

void timer__start(timer__t* timer, uint64_t time_ms, timer__mode_e mode, timer__cb_t timer_cb) {
    timer->start_time_ms = timer__systick_get();
    timer->time_ms = time_ms;
    timer->mode = mode;
    timer->timer_cb = timer_cb;
    list__include(&list_timer, (void*)timer);
}

void timer__stop(timer__t* timer) {
    list__exclude(&list_timer, (void*)timer);
}
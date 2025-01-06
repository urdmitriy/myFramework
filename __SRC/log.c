//
// Created by urdmi on 04.01.2025.
//

#include "log.h"
#include "timer.h"
#include "stdlib.h"
#include "string.h"

#define MAX_LEN_MESSAGE     200

static uint8_t log_uart_id = 0xFF;
static list__item_t list_messages;
static uint8_t count_messages_wait = 0;

static void log__cout();

void log__init(uint8_t uart_id, list__item_t* list_head){
    uart__open(uart_id);
    log_uart_id = uart_id;
    list__init(&list_messages);
    list__include(list_head, log__cout);
}

void log__print(char* message){
    if (count_messages_wait < 10) {
        uint8_t len = strlen(message);
        if (len <= MAX_LEN_MESSAGE) {
            char* message_ptr = malloc(len + 1);
            if (message_ptr != 0){
                strcpy(message_ptr, message);
                list__include(&list_messages, message_ptr);
                count_messages_wait++;
            }
        }
    }
}

static void log__cout(){
    if (log_uart_id != 0xFF && !uart__is_busy(log_uart_id)) {
        if (list_messages.next_list->item_ptr != 0){
            uart__tx(log_uart_id, list_messages.next_list->item_ptr, 0);
            free(list_messages.next_list->item_ptr);
            list__exclude(&list_messages, list_messages.next_list);
            count_messages_wait--;
        } else { // если голова списка, на всякий случай очищаем счетчик сообщений в очереди, т.к. список пустой
            count_messages_wait = 0;
        }
    }
}
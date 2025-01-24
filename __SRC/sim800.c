//
// Created by urdmi on 06.01.2025.
//

#include "sim800.h"
#include "timer.h"
#include "log.h"
#include "uart.h"
#include "stdio.h"
#include "malloc.h"
#include "string.h"

#define TIMEOUT_COMMAND_MS 5000
typedef enum {
    SIM800__CMD_NA,
    SIM800__CMD_HELLO, //AT
    SIM800__CMD_HOW_ARE_YOU, //ATI
    SIM800__CMD_AON_ON, //AT+CLIP=1
    SIM800__CMD_SMS_TEXT_MODE, //AT+CMGF=1
}sim800__cmd_e;

typedef struct {
    uint16_t command_id;
    char* command;
}sim800__command_t;

static cb_gsm_t cb_gsm= 0;
static uint8_t sim800__wait = 0;
static timer__t timer_sim800_cout;
static timer__t timer_sim800_command;
static timer__t timer_sim800_test;
static sim800__state_e sim800_state;
static uint8_t gsm_uart_id = 0xFF;
static void uart__cmd_cb(uint8_t uart_id, uart__event_e event, uint8_t* buf, uint16_t len);
static uint8_t rx_buf[128];
static list__item_t list_commands;
static uint8_t count_commands_wait = 0;
static sim800__cmd_e current_command_id = SIM800__CMD_NA;

static void sim800__timer_wait_cb(void);
static void uart__rx_cb(uint8_t uart_id, uart__event_e event, uint8_t* buf, uint16_t len);
static void sim800__set_state(sim800__state_e state);
static void sim800__send_cmd(sim800__cmd_e command_id, char* parametr );
static void sim800__timer_command_cb(void);
static void sim800__command_cout(void);
static void sim800__command_cb_handler(void);
static void sim800__calls_handler(char* message);

void sim800_init(uint8_t uart_id, cb_gsm_t cb) {
    gsm_uart_id = uart_id;
    cb_gsm = cb;
    list__init(&list_commands);
}

void sim800_cout() {
    if (!sim800__wait) {
        switch (sim800_state) {
            case SIM800__STATE_BEGIN:
                uart__rx(gsm_uart_id, rx_buf, 0, 10, 0, uart__rx_cb);
                sim800__set_state(SIM800__STATE_TEST);
                break;
            case SIM800__STATE_TEST:
                sim800__set_state(SIM800__STATE_WAIT_TEST_ANSWER);
                sim800__send_cmd(SIM800__CMD_HELLO, 0);
                break;
            case SIM800__STATE_WAIT_TEST_ANSWER:
                break;
            case SIM800__STATE_INIT:
                sim800__send_cmd(SIM800__CMD_HOW_ARE_YOU, 0);
                sim800__send_cmd(SIM800__CMD_AON_ON, 0);
                sim800__send_cmd(SIM800__CMD_SMS_TEXT_MODE, 0);
                sim800__set_state(SIM800__STATE_WAIT_INIT);
                break;
            case SIM800__STATE_WAIT_INIT:
                break;
            case SIM800__STATE_REGISTER_NETWORK_WAIT:
                break;
            case SIM800__STATE_GET_OPSOS_NAME:
                break;
            case SIM800__STATE_GET_SIGNAL_LEVEL:
                break;
            case SIM800__STATE_OPEN_GPRS_SESSION:
                break;
            case SIM800__STATE_WAIT_IP_ADDRESS:
                break;
            case SIM800__STATE_TEST_CONNECTION:
                break;
            case SIM800__STATE_WORK:
                break;
            case SIM800__STATE_RESTART:
                log__print("SIM800 waiting RESTART...\n\r");
                break;

        }
        sim800__wait = 1;
        timer__start(&timer_sim800_cout, 3000, TIMER__MODE_ONE_PULSE, sim800__timer_wait_cb);
    }
    sim800__command_cout();
}

static void sim800__timer_wait_cb(void) {
    sim800__wait = 0;
}

static void sim800__set_state(sim800__state_e state) {
    sim800_state = state;
    char message[128] = {"SIM800 set state "};
    switch (state) {
        case SIM800__STATE_BEGIN:
            strcat(message, "BEGIN\n\r");
            break;
        case SIM800__STATE_TEST:
            strcat(message, "TEST\n\r");
            break;
        case SIM800__STATE_WAIT_TEST_ANSWER:
            strcat(message, "WAIT TEST ANSWER\n\r");
            break;
        case SIM800__STATE_INIT:
            strcat(message, "INIT\n\r");
            break;
        case SIM800__STATE_WAIT_INIT:
            strcat(message, "WAIT INIT\n\r");
            break;
        case SIM800__STATE_REGISTER_NETWORK_WAIT:
            strcat(message, "WAIT REGISTER IN NETWORK\n\r");
            break;
        case SIM800__STATE_GET_OPSOS_NAME:
            strcat(message, "GET OPSOS NAME\n\r");
            break;
        case SIM800__STATE_GET_SIGNAL_LEVEL:
            strcat(message, "GET SIGNAL LEVEL\n\r");
            break;
        case SIM800__STATE_OPEN_GPRS_SESSION:
            strcat(message, "OPEN GPRS SESSION\n\r");
            break;
        case SIM800__STATE_WAIT_IP_ADDRESS:
            strcat(message, "WAIT IP ADDRESS\n\r");
            break;
        case SIM800__STATE_TEST_CONNECTION:
            strcat(message, "TEST CONNECTION\n\r");
            break;
        case SIM800__STATE_WORK:
            strcat(message, "WORK\n\r");
            break;
        case SIM800__STATE_RESTART:
            strcat(message, "RESTART\n\r");
            sim800_state = SIM800__STATE_BEGIN;
            break;
        default:
            strcat(message, "UNKNOWN\n\r");
    }
    log__print(message);
    cb_gsm(state);
}

static void sim800__send_cmd(sim800__cmd_e command_id, char* parametr ) {

    char command[128];
    switch (command_id) {
        case SIM800__CMD_HELLO:
            strcpy(command, "AT");
            break;
        case SIM800__CMD_HOW_ARE_YOU:
            strcpy(command, "ATI");
            break;
        case SIM800__CMD_AON_ON:
            strcpy(command, "AT+CLIP=1");
            break;
        case SIM800__CMD_SMS_TEXT_MODE:
            strcpy(command, "AT+CMGF=1");
            break;
        default:
            return;
    }
    if (parametr != 0){
        strcat(command, parametr);
    }
    strcat(command, "\n\r");

    if (count_commands_wait < 20) {
        uint8_t len = strlen(command);

        char *command_ptr = malloc(len + 1);
        sim800__command_t* sim800Command_ptr = malloc(sizeof (sim800__command_t));
        sim800Command_ptr->command_id = command_id;
        sim800Command_ptr->command = command_ptr;
        if (command_ptr != 0 && sim800Command_ptr != 0) {
            strcpy(command_ptr, command);
            list__include(&list_commands, sim800Command_ptr);
            count_commands_wait++;
        }
    }
}

static void uart__cmd_cb(uint8_t uart_id, uart__event_e event, uint8_t* buf, uint16_t len){

}

static void uart__rx_cb(uint8_t uart_id, uart__event_e event, uint8_t *buf, uint16_t len) {

    uint8_t *message = buf;
    uint8_t count_zeros = 0;
    while (*message == 0 && count_zeros < 20) {
        message++;
        count_zeros++;
    }

    if (strstr((char*)message,"+CLIP:")) {
        sim800__calls_handler((char*)message);
    } else if (strstr((char*)message, "OK") || strstr((char*)message, "ERROR")) {
        sim800__command_cb_handler();
    }

    log__print(message);
    log__print("\n\r");
    uart__rx(gsm_uart_id, buf, 0, 10, 0, uart__rx_cb);
}

static void sim800__calls_handler(char* message){

    log__print("---=== this is call handler ===---");
}

static void sim800__command_cb_handler(void) {
    timer__stop(&timer_sim800_command);

    switch (sim800_state) {
        case SIM800__STATE_WAIT_TEST_ANSWER:
            sim800__set_state(SIM800__STATE_INIT);
            break;
        case SIM800__STATE_WAIT_INIT:
            if (current_command_id == SIM800__CMD_SMS_TEXT_MODE)
                sim800__set_state(SIM800__STATE_REGISTER_NETWORK_WAIT);
            break;
        default:
            break;
    }
    current_command_id = SIM800__CMD_NA;
}

static void sim800__timer_command_cb(void) {
    current_command_id = SIM800__CMD_NA;
}

static void sim800__command_cout(){
    if (current_command_id == SIM800__CMD_NA) {;
        if (list_commands.next_list->item_ptr != 0){
            timer__start(&timer_sim800_command, TIMEOUT_COMMAND_MS, TIMER__MODE_ONE_PULSE, sim800__timer_command_cb);
            sim800__command_t* command = (sim800__command_t*)list_commands.next_list->item_ptr;
            current_command_id = command->command_id;

            char message[128];
            sprintf(message, "Send command %s", command->command);
            log__print(message);

            uart__tx(gsm_uart_id, (uint8_t*)command->command, uart__cmd_cb);

            free(command->command);
            free(command);
            list__exclude(&list_commands, list_commands.next_list);
            count_commands_wait--;
        } else { // если голова списка, на всякий случай очищаем счетчик сообщений в очереди, т.к. список пустой
            count_commands_wait = 0;
        }
    }
}


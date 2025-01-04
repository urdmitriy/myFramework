//
// Created by urdmi on 03.08.2024.
//

#include "framework.h"
#include "gpio.h"

void HardFault_Handler();

int main(void) {

    framework__init();

    while (1) {
        framework__app();
    }
    return 0;
}

void HardFault_Handler() {
    __NOP();
}

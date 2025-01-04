//
// Created by urdmi on 03.08.2024.
//

#include "hw_supervisor.h"
#include "stm32f4xx.h"

typedef enum {
    hw_supervisor_speed_low,
    hw_supervisor_speed_high,
}hw_supervisor_speed_t;

static uint32_t current_freq;

static void hw_supervisor__sys_clock_setting(hw_supervisor_speed_t speed);
static void hw_supervisor__iwgd_init(void);

void hw_supervisor__init(void) {
    hw_supervisor__sys_clock_setting(hw_supervisor_speed_high);
#ifndef DEBUG
	hw_supervisor__iwgd_init();
#endif
}

static void hw_supervisor__sys_clock_setting(hw_supervisor_speed_t speed) {
    RCC->CFGR &= ~RCC_CFGR_SW_0; //System clock switch
    RCC->CFGR &= ~RCC_CFGR_SW_1; //00: HSI oscillator selected as system clock, 01: HSE oscillator selected as system clock, 10: PLL selected as system clock

    RCC->CR &= RCC_CR_PLLON; //Disable the PLL by setting PLLON to 0
    while (RCC->CR & RCC_CR_PLLRDY); //Wait until PLLRDY is cleared. The PLL is now fully stopped.
	
	RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0); //1: HSI oscillator ready

    FLASH->ACR &= ~FLASH_ACR_PRFTEN;
    FLASH->ACR |= FLASH_ACR_PRFTEN;
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_7WS;

//    Change the desired parameter.

    RCC->PLLCFGR &= ~(3 << RCC_PLLCFGR_PLLSRC_Pos);
    RCC->PLLCFGR |= ((RCC_PLLCFGR_PLLSRC) << RCC_PLLCFGR_PLLSRC_Pos); //00: No clock sent to PLL PLLSAI1, 01: MSI clock selected as PLL PLLSAI1 clock entry, 10: HSI16 clock selected as PLL PLLSAI1 clock entry, 11: HSE clock selected as PLL PLLSAI1 clock entry

    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN_0|RCC_PLLCFGR_PLLN_1|RCC_PLLCFGR_PLLN_2|RCC_PLLCFGR_PLLN_3|
            RCC_PLLCFGR_PLLN_4|RCC_PLLCFGR_PLLN_5|RCC_PLLCFGR_PLLN_6|RCC_PLLCFGR_PLLN_7|RCC_PLLCFGR_PLLN_8); //Main PLL multiplication factor for VCO

    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_0|RCC_PLLCFGR_PLLM_1|RCC_PLLCFGR_PLLM_2|RCC_PLLCFGR_PLLM_3|
                      RCC_PLLCFGR_PLLM_4|RCC_PLLCFGR_PLLM_5);

    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP_0|RCC_PLLCFGR_PLLP_1); //Main PLL (PLL) division factor for main system clock
    //Division factor for the main PLL and audio PLL (PLLSAI1) input clock
    //00: PLLP = 2; 01: PLLP = 4; 10: PLLP = 6; 11: PLLP = 8

    if (speed == hw_supervisor_speed_high) {
        RCC->PLLCFGR |= 8 << RCC_PLLCFGR_PLLM_Pos; //Main PLL (PLL) division factor for main system clock
        RCC->PLLCFGR |= 84 << RCC_PLLCFGR_PLLN_Pos;
        current_freq = 84000000;
    } else if (speed == hw_supervisor_speed_low) {
        RCC->PLLCFGR |= 8 << RCC_PLLCFGR_PLLM_Pos; //Main PLL (PLL) division factor for main system clock
        RCC->PLLCFGR |= 4 << RCC_PLLCFGR_PLLN_Pos;
        current_freq = 4000000;
    }

    RCC->CR |= RCC_CR_PLLON; //Enable the PLL again by setting PLLON to 1.
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    RCC->CFGR &= ~RCC_CFGR_HPRE_0; //AHB prescaler
    RCC->CFGR &= ~RCC_CFGR_HPRE_1; //0xxx: SYSCLK not divided, 1000: SYSCLK divided by 2, 1001: SYSCLK divided by 4, 1010: SYSCLK divided by , 1011: SYSCLK divided by 16
    RCC->CFGR &= ~RCC_CFGR_HPRE_2; //1100: SYSCLK divided by 64, 1101: SYSCLK divided by 128, 1110: SYSCLK divided by 256, 1111: SYSCLK divided by 512
    RCC->CFGR &= ~RCC_CFGR_HPRE_3;

    RCC->CFGR &= ~RCC_CFGR_PPRE1_0; //APB high-speed prescaler (APB1)
    RCC->CFGR &= ~RCC_CFGR_PPRE1_1; //0xx: HCLK not divided, 100: HCLK divided by 2, 101: HCLK divided by 4, 110: HCLK divided by 8, 111: HCLK divided by 16
    RCC->CFGR |= RCC_CFGR_PPRE1_2;

    RCC->CFGR &= ~RCC_CFGR_PPRE2_0; //APB high-speed prescaler (APB2)
    RCC->CFGR &= ~RCC_CFGR_PPRE2_1; //0xx: HCLK not divided, 100: HCLK divided by 2, 101: HCLK divided by 4, 110: HCLK divided by 8, 111: HCLK divided by 16
    RCC->CFGR &= ~RCC_CFGR_PPRE2_2;
	
    RCC->CFGR &= ~RCC_CFGR_SW_0; //System clock switch
    RCC->CFGR |= RCC_CFGR_SW_1; //00: HSI oscillator selected as system clock, 01: HSE oscillator selected as system clock, 10: PLL selected as system clock
}

static void hw_supervisor__iwgd_init(void) {
    IWDG->KR = 0xCCCC; //Writing the key value 0xCCCC starts the watchdog (except if the hardware watchdog option is selected)
	IWDG->KR = 0x5555; //write enable
    IWDG->PR |= IWDG_PR_PR_0; //Prescaler divider 32000 / 256 =
    IWDG->PR |= IWDG_PR_PR_1; //000: divider /4, 001: divider /8,010: divider /16,011: divider /32,100: divider /64,101: divider /128,110: divider /256,111: divider /256
    IWDG->PR |= IWDG_PR_PR_2;
    IWDG->RLR = 0x0271; //5s
    while (IWDG->SR)
    IWDG->KR = 0x0000; //write protect
}

uint32_t hw_supervisor__freq_get(void){
    return current_freq;
}

void hw_supervisor__iwdg_reset(void){
#ifndef DEBUG
    IWDG->KR = 0xAAAA; //reload
#endif
    return;
}

#include <stdint.h>
#include "stm32f103xb.h"

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;
extern uint32_t _scustom_buffer;
extern uint32_t _ecustom_buffer;
extern int main(void);

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    dst = &_sbss;
    while (dst < &_ebss)
    {
        *dst++ = 0;
    }

    main();

    while (1)
        ;
}

void Default_Handler(void)
{
    while (1)
        ;
}

void Reset_Handler(void);
void Default_Handler(void);

/* System Exceptions */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* External Interrupts (Cần dùng cái nào thì khai báo cái đó) */
void WWDG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
/* Bạn có thể thêm các Handler khác tương tự nếu cần */

__attribute__((section(".isr_vector"))) void (*const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack), /* 0. Top of Stack */
    Reset_Handler,              /* 1. Reset */
    NMI_Handler,                /* 2. NMI */
    HardFault_Handler,          /* 3. Hard Fault */
    MemManage_Handler,          /* 4. MPU Fault */
    BusFault_Handler,           /* 5. Bus Fault */
    UsageFault_Handler,         /* 6. Usage Fault */
    0,
    0,
    0,
    0,                /* 7-10. Reserved */
    SVC_Handler,      /* 11. SVCall */
    DebugMon_Handler, /* 12. Debug Monitor */
    0,                /* 13. Reserved */
    PendSV_Handler,   /* 14. PendSV */
    SysTick_Handler,  /* 15. SysTick */

    /* External Interrupts */
    WWDG_IRQHandler,      /* 16. Window Watchdog */
    PVD_IRQHandler,       /* 17. PVD */
    Default_Handler,      /* 18. TAMPER */
    Default_Handler,      /* 19. RTC */
    Default_Handler,      /* 20. FLASH */
    Default_Handler,      /* 21. RCC */
    EXTI0_IRQHandler,     /* 22. EXTI Line 0 */
    EXTI1_IRQHandler,     /* 23. EXTI Line 1 */
    Default_Handler,      /* 24. EXTI Line 2 */
    Default_Handler,      /* 25. EXTI Line 3 */
    Default_Handler,      /* 26. EXTI Line 4 */
    Default_Handler,      /* 27. DMA1 Channel 1 */
    Default_Handler,      /* 28. DMA1 Channel 2 */
    Default_Handler,      /* 29. DMA1 Channel 3 */
    Default_Handler,      /* 30. DMA1 Channel 4 */
    Default_Handler,      /* 31. DMA1 Channel 5 */
    Default_Handler,      /* 32. DMA1 Channel 6 */
    Default_Handler,      /* 33. DMA1 Channel 7 */
    ADC1_2_IRQHandler,    /* 34. ADC1 & ADC2 */
    Default_Handler,      /* 35. USB HP / CAN TX */
    Default_Handler,      /* 36. USB LP / CAN RX0 */
    Default_Handler,      /* 37. CAN RX1 */
    Default_Handler,      /* 38. CAN SCE */
    Default_Handler,      /* 39. EXTI Line 9..5 */
    Default_Handler,      /* 40. TIM1 Break */
    Default_Handler,      /* 41. TIM1 Update */
    Default_Handler,      /* 42. TIM1 Trigger / COM */
    Default_Handler,      /* 43. TIM1 CC */
    TIM2_IRQHandler,      /* 44. TIM2 (IRQ28) */
    TIM3_IRQHandler,      /* 45. TIM3 */
    Default_Handler,      /* 46. TIM4 */
    Default_Handler,      /* 47. I2C1 EV */
    Default_Handler,      /* 48. I2C1 ER */
    Default_Handler,      /* 49. I2C2 EV */
    Default_Handler,      /* 50. I2C2 ER */
    Default_Handler,      /* 51. SPI1 */
    Default_Handler,      /* 52. SPI2 */
    USART1_IRQHandler,    /* 53. USART1 (IRQ37) <--- ĐÃ THÊM Ở ĐÂY */
    Default_Handler,      /* 54. USART2 */
    Default_Handler,      /* 55. USART3 */
    EXTI15_10_IRQHandler, /* 56. EXTI Line 15..10 */
    /* ... tiếp tục các IRQ khác ... */
};
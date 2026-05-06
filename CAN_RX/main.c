#include "stm32f103xb.h"
#include <stdint.h>
#include "Can.h"

uint32_t FilterID = 0x456;
Can_RxMessageType RxMsg;
Can_TxMessageType TxMsg[] = {
    [SENSOR_DATA] = {.id = 0x7F8,
                     .len = 8,
                     .data = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8}},
    [CONTROL_DATA] = {.id = 0x7F9,
                      .len = 8,
                      .data = {0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10}}};

void delay(volatile uint32_t count)
{
    while (count--)
        ;
}

int main(void)
{
    Can_Init(RELEASE_MODE);
    CAN1->IER |= (1 << 1);
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    Can_Filter_Config(FilterID);
    while (1)
    {
        Can_Read(&RxMsg);
    }
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    if (CAN1->RF0R & 0x03) // FMP0 > 0
    {
        Can_Read(&RxMsg);
    }
}
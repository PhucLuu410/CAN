#include "stm32f103xb.h"
#include <stdint.h>
#include "Can.h"

uint16_t FilterID = 0x7F9;
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

    Can_Filter_Config(FilterID);
    while (1)
    {
        Can_Write(&TxMsg[CONTROL_DATA]);
    }
}
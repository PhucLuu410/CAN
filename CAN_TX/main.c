#include "stm32f103xb.h"
#include "Can.h"

volatile uint32_t rx_id = 0x123;
volatile uint8_t rx_buf[8];

void delay(volatile uint32_t t)
{
    while (t--)
        __NOP();
}

int main(void)
{
    Can_Init();

    uint8_t tx[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    while (1)
    {
        Can_Write(0x123, tx, 8);

        delay(1000000);

        if (Can_Read((uint32_t *)&rx_id, (uint8_t *)rx_buf))
        {
        }
    }
}
#include "stm32f103xb.h"
#include <stdint.h>
#include "Can.h"

volatile uint32_t rx_id_val;
volatile uint8_t rx_buffer[8];
volatile uint32_t rx_count = 0;

void delay(volatile uint32_t t)
{
    while (t--)
        __NOP();
}

int main(void)
{
    Can_Init();

    uint8_t payload[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint32_t myID = 0x123;

    while (1)
    {
        Can_Write(myID, payload, 8);
        payload[0]++;
        delay(1000000);
    }
}
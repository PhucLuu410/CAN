#include "stm32f103xb.h"
#include <stdint.h>
#include "Can.h"

// Biến quan sát dữ liệu nhận được
volatile uint32_t rx_id_val;
volatile uint8_t rx_buffer[8];
volatile uint32_t rx_count = 0;

int main(void)
{
    Can_Init();
    Can_Filter_Config();

    while (1)
    {

        if ((CAN1->RF0R & 0x03) != 0)
        {
            Can_Read((uint32_t *)&rx_id_val, (uint8_t *)rx_buffer);
            rx_count++;
        }
    }
}
#include "stm32f103xb.h"
#include <stdint.h>
#include "Can.h"

volatile uint32_t rx_id_val;
volatile uint8_t rx_buffer[8];

volatile uint32_t rx_count = 0;

/* ghi lại thứ tự nhận */
volatile uint32_t first_id = 0;
volatile uint32_t second_id = 0;

void delay(volatile uint32_t t)
{
    while (t--)
        __NOP();
}

int main(void)
{
    Can_Init();
    Can_Filter_Config();

    uint8_t msg1[8] = {0x11, 1, 1, 1, 1, 1, 1, 1};
    uint8_t msg2[8] = {0x22, 2, 2, 2, 2, 2, 2, 2};

    /* gửi 2 frame */
    Can_Write(0x300, msg1, 8);

    delay(1000); // có thể bỏ để thử sát nhau hơn

    Can_Write(0x100, msg2, 8);

    while (1)
    {
        if ((CAN1->RF0R & 0x03) != 0)
        {
            Can_Read((uint32_t *)&rx_id_val,
                     (uint8_t *)rx_buffer);

            rx_count++;

            if (rx_count == 1)
            {
                first_id = rx_id_val;
            }

            if (rx_count == 2)
            {
                second_id = rx_id_val;

                while (1)
                    ;
            }
        }
    }
}
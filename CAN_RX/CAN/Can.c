#include "Can.h"

volatile uint32_t id;
volatile uint32_t data;

typedef struct
{
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
} CAN_RxMsg;

volatile CAN_RxMsg rx_msg;
volatile uint8_t rx_flag = 0;

static void Clock_AFIO_GPIOA_Enable(void)
{
    RCC->CR |= (1 << 0);
    RCC->APB2ENR |= (1 << 0);
    RCC->APB2ENR |= (1 << 2);
    RCC->APB1ENR |= (1 << 25);

    GPIOA->CRH &= ~(0xF << 12);
    GPIOA->CRH |= (0x4 << 12);

    GPIOA->CRH &= ~(0xF << 16);
    GPIOA->CRH |= (0xB << 16);
}

void Can_Init(void)
{
    Clock_AFIO_GPIOA_Enable();

    CAN1->MCR &= ~(1 << 1);
    while (CAN1->MSR & (1 << 1))
        ;

    CAN1->MCR |= (1 << 0);
    while (!(CAN1->MSR & (1 << 0)))
        ;

    CAN1->BTR = 0;

    CAN1->BTR |= (4 - 1);
    CAN1->BTR |= (11 << 16); // TS1 = 12 tq
    CAN1->BTR |= (2 << 20);  // TS2 = 3 tq
    CAN1->BTR |= (0 << 24);  // SJW = 1 (mặc định)

    CAN1->MCR &= ~(1 << 0);
    while (CAN1->MSR & (1 << 0))
        ;
}

void Can_Filter_Config(void)
{
    CAN1->FMR |= (1 << 0); // 1. FINIT = 1: Bắt đầu cấu hình bộ lọc

    CAN1->FA1R &= ~(1 << 0); // 2. Tắt Filter 0 để chỉnh sửa

    CAN1->FS1R |= (1 << 0);  // 3. Chọn chế độ 32-bit cho Filter 0
    CAN1->FM1R &= ~(1 << 0); // 4. Chế độ Identifier Mask (Lọc theo mặt nạ)

    CAN1->sFilterRegister[0].FR1 = 0x00000000; // ID mẫu = 0
    CAN1->sFilterRegister[0].FR2 = 0x00000000; // Mask = 0 (Cho phép mọi ID qua cổng)

    CAN1->FFA1R &= ~(1 << 0); // 5. Gán Filter 0 cho FIFO 0

    CAN1->FA1R |= (1 << 0); // 6. Kích hoạt Filter 0
    CAN1->FMR &= ~(1 << 0); // 7. FINIT = 0: Kết thúc cấu hình
}

void Can_Write(uint32_t ID, uint8_t *payload, uint8_t len)
{
    uint8_t mb;

    /* chờ có mailbox trống */
    while ((CAN1->TSR & ((1 << 26) | (1 << 27) | (1 << 28))) == 0)
        ;

    /* chọn mailbox trống */
    if (CAN1->TSR & (1 << 26))
        mb = 0;
    else if (CAN1->TSR & (1 << 27))
        mb = 1;
    else
        mb = 2;

    CAN1->sTxMailBox[mb].TIR = (ID << 21);

    CAN1->sTxMailBox[mb].TDTR = (len & 0x0F);

    CAN1->sTxMailBox[mb].TDLR =
        payload[0] | (payload[1] << 8) | (payload[2] << 16) | (payload[3] << 24);

    CAN1->sTxMailBox[mb].TDHR =
        payload[4] | (payload[5] << 8) | (payload[6] << 16) | (payload[7] << 24);

    /* transmit request */
    CAN1->sTxMailBox[mb].TIR |= 1;
}

void Can_Read(uint32_t *ID, uint8_t *buffer)
{
    // 1. Kiểm tra bit FMP0 (FIFO Message Pending) xem có tin nhắn nào không
    if ((CAN1->RF0R & 0x03) != 0)
    {
        // 2. Lấy ID ra (Dịch ngược lại 21 bit)
        *ID = (CAN1->sFIFOMailBox[0].RIR >> 21) & 0x7FF;

        // 3. Lấy dữ liệu ra
        uint32_t low = CAN1->sFIFOMailBox[0].RDLR;
        uint32_t high = CAN1->sFIFOMailBox[0].RDHR;

        buffer[0] = low & 0xFF;
        buffer[1] = (low >> 8) & 0xFF;
        buffer[2] = (low >> 16) & 0xFF;
        buffer[3] = (low >> 24) & 0xFF;
        buffer[4] = high & 0xFF;
        buffer[5] = (high >> 8) & 0xFF;
        buffer[6] = (high >> 16) & 0xFF;
        buffer[7] = (high >> 24) & 0xFF;

        // 4. QUAN TRỌNG: Giải phóng FIFO (RFOM0 = 1)
        // Nếu thiếu dòng này, bạn chỉ nhận được 3 tin nhắn đầu tiên rồi CAN sẽ "đứng hình"
        CAN1->RF0R |= (1 << 5);
    }
}

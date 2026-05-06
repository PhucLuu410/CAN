#include "Can.h"

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

void Can_Init(uint8_t Mode)
{
    Clock_AFIO_GPIOA_Enable();

    CAN1->MCR &= ~(1 << 1);
    while (CAN1->MSR & (1 << 1))
        ;

    CAN1->MCR |= (1 << 0);
    while (!(CAN1->MSR & (1 << 0)))
        ;
    CAN1->BTR = 0;
    if (Mode == TEST_MODE)
    {
        CAN1->BTR |= (3 << 30);
    }
    else
    {
        CAN1->BTR &= ~(3 << 30);
    }
    CAN1->BTR |= (0 << 24);
    CAN1->BTR |= ((12 - 1) << 16);
    CAN1->BTR |= ((3 - 1) << 20);
    CAN1->BTR |= (1 - 1);
    CAN1->MCR &= ~(1 << 0);
    while (CAN1->MSR & (1 << 0))
        ;
}

void Can_Filter_Config(uint16_t id)
{
    CAN1->FMR |= (1 << 0);
    CAN1->FMR &= ~(1 << 8);
    CAN1->FM1R |= (1 << 0);
    CAN1->FS1R |= (1 << 0);
    CAN1->FA1R |= (1 << 0);

    CAN1->sFilterRegister[0].FR1 = (id << 21);
    CAN1->sFilterRegister[0].FR2 = 0;
    CAN1->FMR &= ~(1 << 0);
}

void Can_Write(Can_TxMessageType *TxMsg)
{
    CAN1->sTxMailBox[0].TIR = 0;
    CAN1->sTxMailBox[0].TIR |= ((TxMsg->id & 0x7FF) << 21);                                                                     // ID
    CAN1->sTxMailBox[0].TIR &= ~(1 << 2);                                                                                       // IDE = 0 (standard)
    CAN1->sTxMailBox[0].TIR &= ~(1 << 1);                                                                                       // RTR = 0 (data frame)                                                                                    // Data Frame
    CAN1->sTxMailBox[0].TDTR = TxMsg->len;                                                                                      // Data Length
    CAN1->sTxMailBox[0].TDLR = (TxMsg->data[0] << 0) | (TxMsg->data[1] << 8) | (TxMsg->data[2] << 16) | (TxMsg->data[3] << 24); // Transmit Request Lower
    CAN1->sTxMailBox[0].TDHR = (TxMsg->data[4] << 0) | (TxMsg->data[5] << 8) | (TxMsg->data[6] << 16) | (TxMsg->data[7] << 24); // Transmit Request Higher
    CAN1->sTxMailBox[0].TIR |= (1 << 0);
    while (!((CAN1->TSR) & (1 << 0)))
        ;
    while (!((CAN1->TSR) & (1 << 1)))
        ;
}

void Can_Read(Can_RxMessageType *RxMsg)
{
    if ((CAN1->RF0R & 0x3) == 0)
        return;
    RxMsg->id = (CAN1->sFIFOMailBox[0].RIR >> 21) & 0x7FF;
    RxMsg->len = (CAN1->sFIFOMailBox[0].RDTR) & 0xF;
    RxMsg->data[0] = CAN1->sFIFOMailBox[0].RDLR & 0xFF;
    RxMsg->data[1] = (CAN1->sFIFOMailBox[0].RDLR >> 8) & 0xFF;
    RxMsg->data[2] = (CAN1->sFIFOMailBox[0].RDLR >> 16) & 0xFF;
    RxMsg->data[3] = (CAN1->sFIFOMailBox[0].RDLR >> 24) & 0xFF;
    RxMsg->data[4] = CAN1->sFIFOMailBox[0].RDHR & 0xFF;
    RxMsg->data[5] = (CAN1->sFIFOMailBox[0].RDHR >> 8) & 0xFF;
    RxMsg->data[6] = (CAN1->sFIFOMailBox[0].RDHR >> 16) & 0xFF;
    RxMsg->data[7] = (CAN1->sFIFOMailBox[0].RDHR >> 24) & 0xFF;
    CAN1->RF0R |= (1 << 5);
}

#include "stm32f103xb.h"
#include "Can.h"
#include <stdint.h>

volatile CAN_RxMsg rx_msg;
volatile uint8_t rx_flag = 0;

/* ---------------- Clock + GPIO ---------------- */
static void Clock_AFIO_GPIOA_Enable(void)
{
    /* HSI ON */
    RCC->CR |= (1 << 0);

    /* AFIO + GPIOA */
    RCC->APB2ENR |= (1 << 0);
    RCC->APB2ENR |= (1 << 2);

    /* CAN Clock */
    RCC->APB1ENR |= (1 << 25);

    /* PA11 = CAN RX input floating */
    GPIOA->CRH &= ~(0xF << 12);
    GPIOA->CRH |= (0x4 << 12);

    /* PA12 = CAN TX alternate push pull */
    GPIOA->CRH &= ~(0xF << 16);
    GPIOA->CRH |= (0xB << 16);
}

/* ---------------- Filter ---------------- */
void Can_Filter_Config(void)
{
    CAN1->FMR |= (1 << 0); // FINIT

    CAN1->FA1R &= ~(1 << 0); // disable filter 0

    CAN1->FS1R |= (1 << 0); // 32-bit scale

    CAN1->FM1R &= ~(1 << 0); // Identifier MASK mode

    CAN1->sFilterRegister[0].FR1 = (0x456 << 21);

    CAN1->sFilterRegister[0].FR2 = (0x7FF << 21);

    CAN1->FFA1R &= ~(1 << 0); // FIFO0

    CAN1->FA1R |= (1 << 0); // enable filter

    CAN1->FMR &= ~(1 << 0); // exit filter init
}

/* ---------------- CAN INIT ---------------- */
void Can_Init(void)
{
    Clock_AFIO_GPIOA_Enable();

    /* Exit sleep */
    CAN1->MCR &= ~(1 << 1);
    while (CAN1->MSR & (1 << 1))
        ;

    /* Init mode */
    CAN1->MCR |= (1 << 0);
    while (!(CAN1->MSR & (1 << 0)))
        ;

    /* Clear BTR first */
    CAN1->BTR = 0;

    /*
      500kbps @ 36MHz
      tq = 4/36M
      1 + 12 + 3 =16 tq
    */

    CAN1->BTR |= (4 - 1);    // BRP
    CAN1->BTR |= (11 << 16); // TS1 =12 tq
    CAN1->BTR |= (2 << 20);  // TS2 =3 tq

    /* LOOPBACK TEST ENABLE
       bỏ dòng này khi test thật với transceiver */
    CAN1->BTR |= (1 << 30);

    /* Normal operation */
    Can_Filter_Config();

    CAN1->MCR &= ~(1 << 0);
    while (CAN1->MSR & (1 << 0))
        ;
}

/* ---------------- Transmit ---------------- */
void Can_Write(uint32_t id, uint8_t *payload, uint8_t len)
{
    while (!(CAN1->TSR & (1 << 26)))

        CAN1->sTxMailBox[0].TIR = (0x300 << 21);
    CAN1->sTxMailBox[0].TDTR = 1;
    CAN1->sTxMailBox[0].TDLR = 0x11;

    CAN1->sTxMailBox[1].TIR = (0x100 << 21);
    CAN1->sTxMailBox[1].TDTR = 1;
    CAN1->sTxMailBox[1].TDLR = 0x22;

    CAN1->sTxMailBox[0].TIR |= 1;
    CAN1->sTxMailBox[1].TIR |= 1;
}

/* ---------------- Receive ---------------- */
uint8_t Can_Read(uint32_t *id, uint8_t *buf)
{
    if ((CAN1->RF0R & 0x03) == 0)
        return 0;

    *id = (CAN1->sFIFOMailBox[0].RIR >> 21);

    uint32_t low = CAN1->sFIFOMailBox[0].RDLR;
    uint32_t high = CAN1->sFIFOMailBox[0].RDHR;

    buf[0] = low & 0xFF;
    buf[1] = (low >> 8) & 0xFF;
    buf[2] = (low >> 16) & 0xFF;
    buf[3] = (low >> 24) & 0xFF;

    buf[4] = high & 0xFF;
    buf[5] = (high >> 8) & 0xFF;
    buf[6] = (high >> 16) & 0xFF;
    buf[7] = (high >> 24) & 0xFF;

    /* Release FIFO */
    CAN1->RF0R |= (1 << 5);

    return 1;
}
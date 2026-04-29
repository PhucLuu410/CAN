#ifndef __CAN_H
#define __CAN_H

#include "stm32f103xb.h"
#include <stdint.h>
#include <stdlib.h>

#define SENSOR_DATA 0
#define CONTROL_DATA 1

#define TEST_MODE 0
#define RELEASE_MODE 1
typedef struct
{
    uint32_t id;
    uint8_t len;
    uint8_t data[8];
} Can_RxMessageType;

typedef struct
{
    uint32_t id;
    uint8_t len;
    uint8_t data[8];
} Can_TxMessageType;

void Can_Init(uint8_t Mode);
void Can_Filter_Config(uint16_t id);
void Can_Write(Can_TxMessageType *TxMsg);
void Can_Read(Can_RxMessageType *RxMsg, uint16_t id);

#endif
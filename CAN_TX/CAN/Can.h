#ifndef CAN_H
#define CAN_H

#include <stdint.h>

typedef struct
{
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];

} CAN_RxMsg;

void Can_Init(void);
void Can_Write(uint32_t id, uint8_t *payload, uint8_t len);
uint8_t Can_Read(uint32_t *id, uint8_t *buf);

#endif
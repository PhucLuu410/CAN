#ifndef __CAN_H
#define __CAN_H

#include "stm32f103xb.h"
#include <stdint.h>
#include <stdlib.h>

void Can_Init(void);
void Can_Filter_Config(void);
void Can_Write(uint32_t ID, uint8_t *payload, uint8_t len);
void Can_Read(uint32_t *ID, uint8_t *buffer);

#endif
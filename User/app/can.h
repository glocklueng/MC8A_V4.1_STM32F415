#ifndef __CAN_H
#define	__CAN_H

#include "stm32f4xx.h"
#include "stm32f4xx_can.h"

static void CAN1_GPIO_Config(void);
static void CAN_NVIC_Config(void);
static void CAN_Mode_Config(void);
static void CAN1_Filter_Config(void);
void CAN_Config(void);

#endif
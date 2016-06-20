#include "motor.h"
#include "WinWatchDog.h"

//���Ź��������� = 4096 * 8 / 42M = 780.2us
//780.2 * (127 - 100) < ˢ��ʱ�� < 780.2 * (127 - 64)
//21ms < ˢ��ʱ�� < 49ms
int WWatchDog_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	unsigned int wwdgcfr = 0;
  //NVIC_PriorityGroupConfig(PriorityGroup); 
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = WWdgPreemptionPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = WWdgSubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	wwdgcfr  = 1 << 9 | 3 << 7 | 100;
	//��WWDGʱ��
	RCC->APB1ENR |= 1 << 11;
	//����WWDG����ֵ,ʱ��Ԥ��Ƶ
	WWDG->CFR = wwdgcfr;
	//ʹ��WWDG,ˢ�¼���ֵ
	WWDG->CR = 0xff;
	//�����־λ
	WWDG->SR = 0;	
	return 0;
}

void WWatch_Refresh(unsigned char val)
{
	WWDG->CR = 0x80 | (val & 0x7f);
}

void WWDG_IRQHandler(void)
{
	MotorPulseDisable();
	MotorEnable(DISABLE);
}

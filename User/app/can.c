#include "can.h" 
#include "Verify.h"
#include "SysTick.h"
#include "motionTypeDef.h"
CanTxMsg TxMsg;	
CanRxMsg RxMsg; 

unsigned int Rev_flag = 0;
unsigned int CardId = 0;
unsigned int AxisId = 0;
unsigned int Recv_Finished = 0;

uint32_t RevRIR;
uint32_t RevRDTR;
uint32_t RevRDLR;
uint32_t RevRDHR;

void GetIds(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	CardId = 0;
	AxisId = 0;
	AxisId += GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) << 3;
	AxisId += GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) << 2;
	AxisId += GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) << 1;
	AxisId += GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);

	CardId += GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6) << 3;
	CardId += GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) << 2;
	CardId += GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8) << 1;
	CardId += GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);

}

static void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1);

	/* Configure CAN RX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Configure CAN TX  pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	  //NVIC_PriorityGroupConfig(PriorityGroup);//VIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	 	/*�ж�����*/
	  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;	   //CAN1 RX0�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CANRxPreemptionPriority;		   //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = CANRxSubPriority;			   //�����ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


static void CAN_Mode_Config(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	 	/************************CANͨ�Ų�������**********************************/
	/*CAN�Ĵ�����ʼ��*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/*CAN��Ԫ��ʼ��*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
	CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  �Զ����߹��� 
	CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //��������ģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		   //BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1=CAN_BS1_7tq;		   //BTR-TS1 ʱ���1 ռ����3��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq;		   //BTR-TS1 ʱ���2 ռ����2��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_Prescaler = 3;		   ////BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤��APB1=42M (42/(1+3+2)/7 = 1Mbps)
	CAN_Init(CAN1, &CAN_InitStructure);
}

static void CAN_Filter_Config(void)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GetIds();
	/*CAN��������ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;										//��������0
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	/* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */

	//CAN_FilterId ������Ҫ��ƥ���ID
	//CAN_FilterMask ��Ҫ��ƥ���λ��һ
	CAN_FilterInitStructure.CAN_FilterIdHigh = ((CardId * 16 + AxisId) << 3);					
	CAN_FilterInitStructure.CAN_FilterIdLow= CAN_ID_EXT | CAN_RTR_DATA; 			
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (0xFF << 3);											
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x7;											
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;											
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
//	CAN_ITConfig(CAN1, CAN_IT_FF0, DISABLE);
//	CAN_ITConfig(CAN1, CAN_IT_FOV0, DISABLE);
}
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();   
}
void CAN1_RX0_IRQHandler(void)
{
	CAN1->IER &= ~CAN_IT_FMP0;
	RevRIR = CAN1->sFIFOMailBox[0].RIR;
	RevRDTR = CAN1->sFIFOMailBox[0].RDTR;
	RevRDLR = CAN1->sFIFOMailBox[0].RDLR;
	RevRDHR = CAN1->sFIFOMailBox[0].RDHR;
//	RxMsg.IDE = (uint8_t)0x04 & CAN1->sFIFOMailBox[0].RIR;
//	RxMsg.ExtId = (uint32_t)0x1FFFFFFF & (CAN1->sFIFOMailBox[0].RIR >> 3);
//	RxMsg.RTR = (uint8_t)0x02 & CAN1->sFIFOMailBox[0].RIR;
//  RxMsg.DLC = (uint8_t)0x0F & CAN1->sFIFOMailBox[0].RDTR;
//  RxMsg.FMI = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDTR >> 8);
//  RxMsg.Data[0] = (uint8_t)0xFF & CAN1->sFIFOMailBox[0].RDLR;
//  RxMsg.Data[1] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDLR >> 8);
//  RxMsg.Data[2] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDLR >> 16);
//  RxMsg.Data[3] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDLR >> 24);
//  RxMsg.Data[4] = (uint8_t)0xFF & CAN1->sFIFOMailBox[0].RDHR;
//  RxMsg.Data[5] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDHR >> 8);
//  RxMsg.Data[6] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDHR >> 16);
//  RxMsg.Data[7] = (uint8_t)0xFF & (CAN1->sFIFOMailBox[0].RDHR >> 24);	
	Rev_flag = 1;
	//CAN1->RF0R |= CAN_RF0R_RFOM0;
}




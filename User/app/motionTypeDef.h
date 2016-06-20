#ifndef MOTIONTYPEDEF_H__
#define MOTIONTYPEDEF_H__

#include <stdint.h>
//#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx.h"
//�����׶ε�ʱ��ٷֱ�
#define AA_PERC							0.2		/*�Ӽ��٣������ٽ׶ε�ʱ��ٷֱ�*/
#define CA_PERC							0.6		/*�ȼ��٣��ȼ��ٽ׶ε�ʱ��ٷֱ�*/
#define RA_PERC							0.2		/*�����٣������ٽ׶ε�ʱ��ٷֱ�*/

//�����׶ε���ɢ������			
#define AA_POINTS						45
#define CA_POINTS						60
#define RA_POINTS						30
#define AR_POINTS						30
#define CR_POINTS						60
#define RR_POINTS						45

#define TOTAL_POINTS					(AA_POINTS + CA_POINTS + RA_POINTS + AR_POINTS + CR_POINTS + RR_POINTS + 1)
#define START_POINTS					(AA_POINTS + CA_POINTS + RA_POINTS)
#define STOP_POINTS						(AR_POINTS + CR_POINTS + RR_POINTS)

#define SPEED_LEVEL						20
#define PSC_CLK							168000000 //����ʱ����14M��ƽ�ʼ���
#define CNTPWM_SIZE_MAX 				1024

#define SLOW_CNT_CLK 			1000000

//Ϊ��ȷ�����������徫�ȣ���ʼ���ٶ������ʱ����һ���Ĺ�ϵ
//�ٶȳ�ʼ��ʱ�ĳ�ʼ�ٶ������ʱ��Ĺ�ϵ
//2 * START_POINTS / acct < init < 3 * START_POINTS / acct ==>
//2 * START_POINTS < init * acct < 3 * START_POINTS ==>
//
#define S_MODEL                         1
//#define T_MODEL                         1
#define CNT_T_MODEL                     1
#define MAX_SPEED                       500000
#define ACC_TIME                        0.1
#define CNT_ACC_TIME                    0.1
#define ACC_SPEED 											8000000

//��ռʱ���ȼ�2λ(0--3)�������ȼ�2λ(0--3)
#define PriorityGroup 									NVIC_PriorityGroup_0

#define WWdgPreemptionPriority 					0
#define WWdgSubPriority 								3

#define Motor1PreemptionPriority 				0
#define Motor1SubPriority 							0

#define Encoder1PreemptionPriority 			0
#define Encoder1SubPriority 						1

#define CANRxPreemptionPriority 				0
#define CANRxSubPriority 								2

/*error*/
typedef enum{
	NORMAL 													= 0,
	PF_SETPU_ERROR 									= 10,
	AXIS_OVERANGE 									= 11,
	MOTOR_IS_DISABLE 								= 12,
	PULS_LESSTHAN_2 								= 13,
	POINTER_2_NULL 									= 14,
	MOTOR_ISRUNNING 								= 15,
	CMD_ERROR 											= 16,
	SPEED_SETUP_ERROR 							= 17,
	MOTION_TABLE_ERROR 							= 18,
	MOTOR_IS_IN_CHANGE_MOVING 			= 19,
	MOTOR_IS_IN_CNT_MOVING 					= 20,
	BEYOND_THE_LIMIT 								= 21,
	LIMIT_SENSOR_INDUCTED 					= 22,
	ENCODER_WAS_DISABLE 						= 23,
	SERVO_ALARM 										= 24
} ErrorCode;			
			
typedef enum{			
	CNT_MOVING 											= 1,
	CNT_PF_MOVING 									= 2,
	PF_MOVING 											= 3,
	CHANGE_MOVING 									= 4,
	STOP_MOVING											= 5,
}MovingStyle;

typedef enum {RUNNING = 1,STOPPED = 0} MtrStatus;
typedef enum {PLUS = 0,REDUCE = 1} MtrDir;
typedef FunctionalState MtrEnable;
typedef enum {OPEN = 0,CLOSE = 1} MtrPulse;
//typedef enum {ON = 0,OFF = 1} SensorSta;
//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
/*pwmĳ��ɢʱ����ڵ�����*/
typedef struct{
	uint16_t Pwm_Cycle;
	uint16_t Pwm_Counter;
} Motion_TableTypeDef;

typedef struct{
	uint32_t IniSp;/*��ʼ�ٶ�*/
	uint32_t StoSp;/*ֹͣ�ٶ�*/
	uint32_t MaxSp;/*����ٶ�*/
	uint32_t pf;
	
	/*���ٶȵķ�Χ�Ƚϴ����ʹ��_iq2�ĸ�ʽ*/
	double AASp;/*�Ӽ��ٶ�*/
	double DASp;/*�����ٶ�*/
	/*����ʱ��ķ�Χ0--1����ʹ��_iq30�ĸ�ʽ*/
	double AAT;
	double CAT;
	double RAT;
	double ART;
	double CRT;
	double RRT;
} Speed_LevelTypeDef;

typedef struct{
	uint32_t DstPulse;						/*��ǰ��Ҫ������pwm������*/
//	uint32_t OutPulse;						/*�Ѿ������pwm������*/
	int32_t DstPos;
	int32_t CurrentPos;					    /*��ǰλ��*/
//	int32_t CurrentEncPos;
	uint32_t MinSteps;						/*���ĳ��S�������˶�����Ҫ������pwm������*/
	uint16_t CurrentIndex;		            /*��ǰִ�е���table����*/
	uint16_t StopIndex;						/*���յ�ָֹͣ��ʱ��ִ�е���table����*/
	uint32_t PwmCntMul;						/*���ٽ׶ε�pwm����*/
	uint16_t PwmCntRem;						/*���ٽ׶ε�pwm����*/
	double MinTimes;						/*���ĳ��S�������˶�����Ҫ������ʱ��*/
	double Time_Cost_Act;					/*���ĳ��S�������˶���ʵ�����ĵ�����ʱ��*/
	Speed_LevelTypeDef *speed;
	Motion_TableTypeDef *table;
	MtrDir Dir;								/*����˶�����*/
	MtrStatus IsRunning;					/*����˶�״̬*/
//	uint16_t IsCntMoving;					/*��ǰ�Ƿ��������б�ʾ*/
//	uint16_t MotorId;						/*�����Id 1...*/
	uint16_t RevStopcmd;                    /*1�����յ�ָֹͣ�0��û�н��յ�ָֹͣ��*/
	uint16_t Enable;						/*���ʹ��*/
	uint16_t pf;							/*�����ǰ�˶��ٶȵȼ� 0...*/
//	uint16_t CntFre;
	uint16_t CntSpeed;
	uint16_t MovingStyle;			/*cntmove,pfmove,stop,changemove*/


	int32_t pLimit;						/*��������λ*/
	int32_t nLimit;						/*��������λ*/	
//	uint32_t ChangeIndex ;
//	uint32_t ChangeFlag ;     /*1.���ٽ׶��յ�changeָ���δ�������ٽ׶�
//															2.���ٽ׶��յ�changeָ��*/
	uint8_t RevChangeRept;
	uint8_t RevChangeCmd;
	uint16_t RevChangeIndex;
	uint32_t RemainPulse;
	
	int32_t TriggerPos;
	MtrDir TriggerDir;
	uint32_t TriggerPeriod;			/*���������ƽ����ʱ�� ms*/
	uint8_t TriggerEnable;
	
	int32_t LightPreStep;			/*��Դ��ǰ��Step ����0��ǰ��С��0�ͺ�*/
	uint32_t LightPeriod;				/*��Դ������ƽ����ʱ�� ms*/
	uint8_t LightEnable;
	
	uint8_t LimitFlag;
	
	uint8_t SearchEnable;
	uint8_t SearchStatus;
	uint8_t EncoderEnable;
	
	int ReversId;
	
//	uint8_t SearchSpeed;
//	MtrDir SearchDir;
	
} Motion_CtrlTypeDef;


#endif





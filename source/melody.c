
/**
  ******************************************************************************
  * @file    melody.c 
  * @author  �̼���
  * @version V1.0
  * @date    19/12/2016
  * @brief   �������� ���õ� ����
  ******************************************************************************
  * @copy
  *
  * ��ư�� �Է� ���� ��(short , midle, long ) �Ǵ� main ������ ��ε�� ��Ÿ���� ���� �Լ���
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 

#include "stm32f10x_conf.h"
#include "MelodyData.h"
#include "melody.h"

/**
 @brief ��ε�(������) ����ϱ� ���� �Լ�
 @param melody ����
 @return void
 @deta
 PARINGMELODY		���� ���� ��ε�
 STARTMELODY		��ư ��� ������ �� ���� ���� ��ε�
 SUCCESSMELODY		��ư ��� ������ �� ���� ��ħ ��ε�
 BUTTONMELODY		��ư ª�� ������ �� ���� ���� ��ε�
 BTCONNECTMELODY	��ư �߰� ���̷� ������ �� �������� ���� ���� ���� ��ε�
 SUCCESSMELODY2	��ư �߰� ���̷� ������ �� �������� ���� ���� ��ħ ��ε�
*/

extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure3; //���� ���� ���� �ϱ� ���� Ÿ�̸� ����ü �ܺ� ����
extern TIM_OCInitTypeDef  TIM_OCInitStructure3; // ���� ���� ���� �ϱ� ���� PWM ����ü �ܺ� ����
bool MelodyOn = false;
int MELODY;
uint32_t MelodyTimer = 0;
uint32_t MelodyPointer = 0;
Melody  m;
uint16_t * MelodyToneArray [15] = {ButtonMelodyTone, SuccessMelodyTone, StartMelodyTone}; //��ε� ���� �ٲٱ� ���� �迭
uint16_t * MelodyTimeArray [15] =  {ButtonMelodyTime, SuccessMelodyTime, StartMelodyTime}; //��ε� ���� �ð��� �ٲٱ� ���� �迭
void MelodyStart(int melody)
{
	MELODY = melody;	//MELODY �� melody.h �� ����ü�� ����� ����ü ����.
	switch(melody)
	{
	
		case BUTTONMELODY:
			m.Length = sizeof(ButtonMelodyTone)/sizeof(uint16_t);
			m.MelodyTime = &ButtonMelodyTime[0];
			m.MelodyTone = &ButtonMelodyTone[0];
			m.Continue = 0;
			break;
		
		case SUCCESSMELODY:
			m.Length = sizeof(SuccessMelodyTone)/sizeof(uint16_t);
			m.MelodyTime = &SuccessMelodyTime[0];
			m.MelodyTone = &SuccessMelodyTone[0];
			m.Continue = 0;
			break;
		
		case STARTMELODY:
			m.Length = sizeof(StartMelodyTone)/sizeof(uint16_t);
			m.MelodyTime = &StartMelodyTime[0];
			m.MelodyTone = &StartMelodyTone[0];
			m.Continue = 0;
			break;
		
		case PARINGMELODY:
			m.Length = sizeof(ParingMelodyTone)/sizeof(uint16_t);
			m.MelodyTime = &ParingMelodyTime[0];
			m.MelodyTone = &ParingMelodyTone[0];
			m.Continue = 0;
			break;
		
		case SUCCESSMELODY2:
			m.Length = sizeof(Success2MelodyTone)/sizeof(uint16_t);
			m.MelodyTime = &Success2MelodyTime[0];
			m.MelodyTone = &Success2MelodyTone[0];
			m.Continue = 0;
			break;
		
		case BTCONNECTMELODY:
			m.Length = sizeof(BTConnectMelodyTone)/sizeof(uint16_t);
			m.MelodyTime = &BTConnectMelodyTime[0];
			m.MelodyTone = &BTConnectMelodyTone[0];
			m.Continue = 0;
			break;
		
	}

	MelodyOn = true ;
	MelodyTimer = 0;
	MelodyPointer = 0;
	TIM_SetCounter(TIM2, 0); //TCNT 0 ���� �ʱ�ȭ
	Buzzer_sound(m.MelodyTone[0]);
	TIM_Cmd(TIM2, ENABLE);
}

void Buzzer_sound(int value) 
{
	uint16_t period;
	period = (1000000/value);
	TIM_TimeBaseStructure3.TIM_Period = period;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure3);
	TIM_OCInitStructure3.TIM_Pulse = period/2;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure3);
}

void MelodyOff(void) //��ε� ���� �Լ�
{
	MELODY = NONEMELODY;
	MelodyOn = false;
	MelodyTimer = 0;
	MelodyPointer = 0;
	TIM_Cmd(TIM2, DISABLE);
	TIM_OCInitStructure3.TIM_Pulse = 0;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure3);

}









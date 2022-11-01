
/**
  ******************************************************************************
  * @file    melody.c 
  * @author  이성일
  * @version V1.0
  * @date    19/12/2016
  * @brief   부저음에 관련된 파일
  ******************************************************************************
  * @copy
  *
  * 버튼이 입력 됐을 때(short , midle, long ) 또는 main 시작을 멜로디로 나타내기 위한 함수들
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 

#include "stm32f10x_conf.h"
#include "MelodyData.h"
#include "melody.h"

/**
 @brief 멜로디를(부저음) 출력하기 위한 함수
 @param melody 종류
 @return void
 @deta
 PARINGMELODY		메인 시작 멜로디
 STARTMELODY		버튼 길게 눌렀을 때 설정 시작 멜로디
 SUCCESSMELODY		버튼 길게 눌렀을 때 설정 마침 멜로디
 BUTTONMELODY		버튼 짧게 눌렀을 때 설정 변경 멜로디
 BTCONNECTMELODY	버튼 중간 길이로 눌렀을 때 조도센서 범위 설정 시작 멜로디
 SUCCESSMELODY2	버튼 중간 길이로 눌렀을 때 조도센서 범위 설정 마침 멜로디
*/

extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure3; //부저 음을 변경 하기 위한 타이머 구조체 외부 참조
extern TIM_OCInitTypeDef  TIM_OCInitStructure3; // 부저 음을 변경 하기 위한 PWM 구조체 외부 참조
bool MelodyOn = false;
int MELODY;
uint32_t MelodyTimer = 0;
uint32_t MelodyPointer = 0;
Melody  m;
uint16_t * MelodyToneArray [15] = {ButtonMelodyTone, SuccessMelodyTone, StartMelodyTone}; //멜로디 톤을 바꾸기 위한 배열
uint16_t * MelodyTimeArray [15] =  {ButtonMelodyTime, SuccessMelodyTime, StartMelodyTime}; //멜로디 톤의 시간을 바꾸기 위한 배열
void MelodyStart(int melody)
{
	MELODY = melody;	//MELODY 는 melody.h 에 구조체로 선언된 구조체 변수.
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
	TIM_SetCounter(TIM2, 0); //TCNT 0 으로 초기화
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

void MelodyOff(void) //멜로디 종료 함수
{
	MELODY = NONEMELODY;
	MelodyOn = false;
	MelodyTimer = 0;
	MelodyPointer = 0;
	TIM_Cmd(TIM2, DISABLE);
	TIM_OCInitStructure3.TIM_Pulse = 0;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure3);

}









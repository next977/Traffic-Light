
/**
  ******************************************************************************
  * @file    delay.c 
  * @author  이성일
  * @version V1.0
  * @date    19/12/2016
  * @brief   딜레이에 관련된 파일
  ******************************************************************************
  * @copy
  *
  * Systic관련 딜레이 와 일반 딜레이 함수
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 


#include "stm32f10x_conf.h"

/**
 @brief 딜레이를 사용 하기 위해 구현한 함수들
 @param delay의 종류
 @return void
 @deta
 delay_s			1s를 사용하기 위해 Systic interrupt를 이용하여 구현한 delay 함수
 delay_ms		1ms를 사용 하기 위해 구현한 delay 함수 
 delay_us		1us를 사용 하기 위해 구현한 delay 함수
 */

void delay_us(uint16_t delay)
{
	while(delay--)
	{
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
	}
}

void delay_ms(uint16_t delay)
{

	while(delay--)
	{
		delay_us(1000);
	}
	

}

void delay_s(uint16_t delay)
{
	delay_stime = delay;
	
	while(delay_stime)
	{
		if((Break ==true)) //Mode 2에서 대기와 발열 하기 위하여 40s, 30s 딜레이 중 버튼을 통해 바로 설정 모드로 들어가기 위한 조건
		{
			break;
		}
	}
	
}



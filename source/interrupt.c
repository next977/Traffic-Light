/**
  ******************************************************************************
  * @file    interrupt.c 
  * @author  이성일
  * @version V1.0
  * @date    19/12/2016
  * @brief   부저,버튼,delay에 관한 interrupt 함수
  ******************************************************************************
  * @copy
  *
  * 부저, 버튼, delay interrupt 함수 구현
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 
#include "stm32f10x_conf.h"
#include <stdio.h>
bool Break = false;
uint8_t z;

/**
 @brief 부저,버튼,delay 인터럽트 함수들
 @param 버튼의 종류
 @return void
 @deta
 TIM2_IRQHandler					Buzzer 주파수 변경 interrupt
 EXTI15_10_IRQHandler				Button 입력 interrupt
 SysTick_Handler					1s Delay 구현하기 위한  interrupt 
 */
 
void TIM2_IRQHandler(void) // 타이머 ms
{

	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		
		if(MelodyOn)
		{
			
			MelodyTimer ++ ;
			if(MelodyTimer == m.MelodyTime[MelodyPointer])
			{
				
				MelodyTimer = 0;
				MelodyPointer ++;
				
				if(MelodyPointer == m.Length)
				{
					if(m.Continue)
					{
						MelodyPointer = 0;
						Buzzer_sound(m.MelodyTone[0]);
						
					}
					else
					{
						MelodyOff();
						
					}
				}
				else
				{
					
					Buzzer_sound(m.MelodyTone[MelodyPointer]);
				}
				
			}
			
			
		}
		else
		{
			
			MelodyOff();
		}
		
		

		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	}

}


void EXTI15_10_IRQHandler(void) //Button interrupt
{
	
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		bAction = true;
		button_time2 = 0;
		if(((FND_On==2))&&(testtest==1)) // Mode 2에서 설정으로 나가기 위한 조건
		{
			Break = true;
		}
		if(longButton == true) // 일반 Mode에서의 Short Button에 대한 조건
		{
			PU = true;
		}
		EXTI_ClearITPendingBit(EXTI_Line13);
		
	}


}

void SysTick_Handler(void) //1s Delay를 구현하기 위한 Systic interrupt
{ 
	
	if(z==10)	//1s Delay 함수를 도는 중 설정 버튼으로 인해 나가기 위한 조건
	{
		z=0;
		delay_stime --;		
	}
	z++;
	delay_time --;
	button_time2 ++;
}


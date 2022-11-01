/**
  ******************************************************************************
  * @file    interrupt.c 
  * @author  �̼���
  * @version V1.0
  * @date    19/12/2016
  * @brief   ����,��ư,delay�� ���� interrupt �Լ�
  ******************************************************************************
  * @copy
  *
  * ����, ��ư, delay interrupt �Լ� ����
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 
#include "stm32f10x_conf.h"
#include <stdio.h>
bool Break = false;
uint8_t z;

/**
 @brief ����,��ư,delay ���ͷ�Ʈ �Լ���
 @param ��ư�� ����
 @return void
 @deta
 TIM2_IRQHandler					Buzzer ���ļ� ���� interrupt
 EXTI15_10_IRQHandler				Button �Է� interrupt
 SysTick_Handler					1s Delay �����ϱ� ����  interrupt 
 */
 
void TIM2_IRQHandler(void) // Ÿ�̸� ms
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
		if(((FND_On==2))&&(testtest==1)) // Mode 2���� �������� ������ ���� ����
		{
			Break = true;
		}
		if(longButton == true) // �Ϲ� Mode������ Short Button�� ���� ����
		{
			PU = true;
		}
		EXTI_ClearITPendingBit(EXTI_Line13);
		
	}


}

void SysTick_Handler(void) //1s Delay�� �����ϱ� ���� Systic interrupt
{ 
	
	if(z==10)	//1s Delay �Լ��� ���� �� ���� ��ư���� ���� ������ ���� ����
	{
		z=0;
		delay_stime --;		
	}
	z++;
	delay_time --;
	button_time2 ++;
}


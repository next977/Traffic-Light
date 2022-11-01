
/**
  ******************************************************************************
  * @file    delay.c 
  * @author  �̼���
  * @version V1.0
  * @date    19/12/2016
  * @brief   �����̿� ���õ� ����
  ******************************************************************************
  * @copy
  *
  * Systic���� ������ �� �Ϲ� ������ �Լ�
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 


#include "stm32f10x_conf.h"

/**
 @brief �����̸� ��� �ϱ� ���� ������ �Լ���
 @param delay�� ����
 @return void
 @deta
 delay_s			1s�� ����ϱ� ���� Systic interrupt�� �̿��Ͽ� ������ delay �Լ�
 delay_ms		1ms�� ��� �ϱ� ���� ������ delay �Լ� 
 delay_us		1us�� ��� �ϱ� ���� ������ delay �Լ�
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
		if((Break ==true)) //Mode 2���� ���� �߿� �ϱ� ���Ͽ� 40s, 30s ������ �� ��ư�� ���� �ٷ� ���� ���� ���� ���� ����
		{
			break;
		}
	}
	
}



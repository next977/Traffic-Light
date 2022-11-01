

/**
  ******************************************************************************
  * @file    button.c 
  * @author  �̼���
  * @version V1.0
  * @date    19/12/2016
  * @brief   ��ư�� ���õ� ����
  ******************************************************************************
  * @copy
  *
  * ��ư�� short , midle, long ������ �Լ���
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 



#include "stm32f10x_conf.h"
#include "button.h"
uint8_t b_key = 0;
bool PU = false;
bool Midle_PU = false;
bool bAction = false;
bool Midle_button = false;
int button;
int Sensor_Mode_select;
bool longButton =false;
long double i = 0;

/**
 @brief ��ư�� ������ �ð��� ���� 3����(short,midle,long) ��ư�� ������ �Լ���
 @param ��ư�� ����
 @return void
 @deta
 button_time2 >1					Short ��ư
 button_time>10					Long ��ư
 */

/**
 @brief ��ư�� ������ �ð��� ���� 3����(short,midle,long) ��ư�� �����ϱ� ���� ������
 @param ��ư�� ���� 
 @return void
 @deta
 button_time2 						Short ��ư ����
 button_time					        Long ��ư ����
 */

void Button_Action(bool * bAction)  // Button_Action �Լ� ���� bAction�� ���� bAction �� ������ �����ϰ�����.
{
	
	
	if(*bAction)
	{
		while(!(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_13))) //Long Button�� �����ϱ� ���Ͽ� ���� �Լ�.
		{
			button_time++; 
			if(button_time>10)break;
			delay_ms(100);
						
		}
		
		if(button_time>10) //Long Button
		{
			printf("long button\r\n");
			*bAction = false;
			button_time =0;
			if(FND_On_Off ==0)
			{
				if(Midle_button != true) 
				{
					DPRINTF("%s\r\n", "Start");
					MelodyStart( STARTMELODY);  //���� �Ҹ� ��� �ð� ����
					delay_ms(50); //���� �Ҹ��� ��Ÿ���� ���� ������
					longButton = true; //Start button�� ������ ���� Short button���� mode�� ���� �Ϸ��� �� ������ ���.
					if(FND_On==6) FND_On=2; // 2�� ��� 40�� �߿� 30�� ��忡�� ��� �������� �ٷ� ���� ���� ���� 2�� ��� ���� ���ڰŸ����� �˷� �ֱ� ���� ����
					if(FND_On==5) FND_On=15;  //5�� �µ� ���� ��� �϶� FND On_Off�� 5������ ���� �Ÿ��� ���� ����
					FND_On_Off=FND_On; //FND_On �� FND_On_Off�� �������ν� ���� ���� � ��� ���� ��� ���� ���ڰŸ����� �˷��ֱ� ���� ��.
					FND_On=0;
					GPIO_ResetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater Off 
				}
			}
			else
			{	
				if(Midle_button != true) 
				{
					DPRINTF("%s\r\n", "Success ");
					MelodyStart( SUCCESSMELODY);  //���� �Ҹ� ��� �ð� ����			
					delay_ms(50); //���� �Ҹ��� ��Ÿ���� ���� ������
					longButton = false; //Success button�� ������ ���Ŀ��� Short button�� ������� �������� �� ������ false�� ����.
					PU =false; // PU ������ false�� �������� ���� Short button�� �ƿ� ����.
 					if(FND_On_Off==15) FND_On_Off=5; //FND_On_Off �� 15�� FND_On_Off 5�� FND_On �� �־� �µ� ���� ���� ���� �ϱ� ���� ����
					FND_On = FND_On_Off;
					FND_On_Off=0;	
					b_key = 0;						
					delay_ms(500);
				}
			}
		}
		
		button_time = 0;
	
		if(!(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_13))) //��ư�� ������ ������ , Short Button�� ���� �ϱ� ���� �Լ�.
		{	
			b_key = 0;
			if(button_time2 >1)
			{
				b_key = 1;  //short button
			}
		}
		
		if((GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_13)))
		{
			
			*bAction = false;
				if((b_key == 1)&&(PU ==true)) //�Ϲ� Mode ������ Short Button ����
				{
					if( button== 0) //Mode 1
					{		
						DPRINTF("%s\r\n", "Button Mode 1");
						FND_On_Off =1;
					}	
					else if( button== 1) //Mode 2
					{
						DPRINTF("%s\r\n", "Button Mode 2");
						FND_On_Off =2;	
					}
			
					else if( button== 2) //Mode 3
					{
						DPRINTF("%s\r\n", "Button Mode 3");
						FND_On_Off =3;
					}
					else if( button== 3) //Mode 4
					{
						DPRINTF("%s\r\n", "Button Mode 4");
						FND_On_Off =4;
					}
					else
					{
						DPRINTF("%s\r\n", "Button Mode 5");
						FND_On_Off =15;
					}
						button ++;
						if(button ==5) 
						{
							button=0;
							MelodyOff();
						}
						MelodyStart(BUTTONMELODY); //���� �Ҹ� ��� �ð��� ����
						delay_ms(50); //���� �Ҹ��� ��Ÿ���� ���� ������
				}
				
		}
	}	
}








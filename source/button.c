

/**
  ******************************************************************************
  * @file    button.c 
  * @author  이성일
  * @version V1.0
  * @date    19/12/2016
  * @brief   버튼에 관련된 파일
  ******************************************************************************
  * @copy
  *
  * 버튼을 short , midle, long 구현한 함수들
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
 @brief 버튼을 누르는 시간에 따라 3가지(short,midle,long) 버튼을 구현한 함수들
 @param 버튼의 종류
 @return void
 @deta
 button_time2 >1					Short 버튼
 button_time>10					Long 버튼
 */

/**
 @brief 버튼을 누르는 시간에 따라 3가지(short,midle,long) 버튼을 구현하기 위한 변수들
 @param 버튼의 종류 
 @return void
 @deta
 button_time2 						Short 버튼 변수
 button_time					        Long 버튼 변수
 */

void Button_Action(bool * bAction)  // Button_Action 함수 안의 bAction은 위에 bAction 의 변수를 참조하고있음.
{
	
	
	if(*bAction)
	{
		while(!(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_13))) //Long Button을 구현하기 위하여 만든 함수.
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
					MelodyStart( STARTMELODY);  //부저 소리 톤과 시간 변경
					delay_ms(50); //부저 소리를 나타내기 위한 딜레이
					longButton = true; //Start button을 누르고 난후 Short button으로 mode를 변경 하려고 이 변수를 사용.
					if(FND_On==6) FND_On=2; // 2번 대기 40초 발열 30초 모드에서 길게 눌렀을때 바로 빠져 나와 지금 2번 모드 임을 깜박거림으로 알려 주기 위한 조건
					if(FND_On==5) FND_On=15;  //5번 온도 조건 모드 일때 FND On_Off를 5번으로 깜빡 거리기 위한 조건
					FND_On_Off=FND_On; //FND_On 을 FND_On_Off로 넣음으로써 지금 현재 어떤 모드 였다 라는 것을 깜박거림으로 알려주기 위한 것.
					FND_On=0;
					GPIO_ResetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater Off 
				}
			}
			else
			{	
				if(Midle_button != true) 
				{
					DPRINTF("%s\r\n", "Success ");
					MelodyStart( SUCCESSMELODY);  //부저 소리 톤과 시간 변경			
					delay_ms(50); //부저 소리를 나타내기 위한 딜레이
					longButton = false; //Success button을 누르고 난후에는 Short button을 사용하지 않으려고 이 변수를 false로 막음.
					PU =false; // PU 변수를 false로 막음으로 인해 Short button을 아에 차단.
 					if(FND_On_Off==15) FND_On_Off=5; //FND_On_Off 가 15면 FND_On_Off 5를 FND_On 에 넣어 온도 조건 모드로 진행 하기 위한 조건
					FND_On = FND_On_Off;
					FND_On_Off=0;	
					b_key = 0;						
					delay_ms(500);
				}
			}
		}
		
		button_time = 0;
	
		if(!(GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_13))) //버튼을 누르고 있을때 , Short Button을 구현 하기 위한 함수.
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
				if((b_key == 1)&&(PU ==true)) //일반 Mode 에서의 Short Button 구현
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
						MelodyStart(BUTTONMELODY); //부저 소리 톤과 시간을 변경
						delay_ms(50); //부저 소리를 나타내기 위한 딜레이
				}
				
		}
	}	
}








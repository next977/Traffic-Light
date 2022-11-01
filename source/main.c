/**
  ******************************************************************************
  * @file    main.c 
  * @author  이성일
  * @version V1.0
  * @date    19/12/2016
  * @brief   메인 함수
  ******************************************************************************
  * @copy
  *
  * 메인 함수 구현
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 
  
#include <stdio.h>
#include <math.h>
#include "stm32f10x_conf.h"
#include "THtable.h" //온도센서 써미스터 저항 테이블
/**
 @brief 버튼에 따른 Mode 변경 및 설정
 @param Mode 종류
 @return void
 @deta
 Mode1					대기 모드 (아무 기능도 하지 않는 상태)
 Mode2					40초 대기, 30초 발열
 Mode3					발열 계속
 Mode4					조도센서 조건 모드
 Mode5					온도센서 조건 모드
 */

bool start = false;
uint32_t button_time=0;
uint32_t button_time2=0;
uint32_t button_time3=0;
uint32_t delay_time=0;
uint32_t delay_stime;
uint32_t Sensor_Mode=0;
uint32_t FND_On_Off=0;
uint32_t FND_On=0;
uint32_t FND_array[]={FND_Pin_A,FND_Pin_B,FND_Pin_C,FND_Pin_D,FND_Pin_E,FND_Pin_F,FND_Pin_G};
int testtest=0;

/*------TH(온도센서 변수)-------*/
uint8_t string[100];
int pointer = 0;
int setstring = 0;
uint16_t a[10]={0};

int main(void)
{
	uint16_t test;
	uint16_t test1;
	uint16_t test2;
	double V_CDS;
	double R_CDS;
	double LUX;
	/*-------온도,조도센서 평균값을 구하기 위한 변수들------*/
	int i;
	/*------------온도,조도센서 값을 담을 공용체와 변수들----------*/
		union data
		{
			uint8_t rdata[4];
			struct d
			{
				int16_t sensorvalue;
				uint16_t batvalue;
			}sd;

		}data;
		
	Setting();  //Sensor init
	/*----------SysTick handler를 구현 하기 위한 SysTick 설정----------*/
	RCC_ClocksTypeDef RCC_Clocks;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	RCC_GetClocksFreq(&RCC_Clocks);
	if (SysTick_Config(RCC_Clocks.SYSCLK_Frequency /10))
	    {
	       		      ///* Capture error */
	       		      while (1);
	    }

	/* Setup SysTick Timer for 100 msec interrupts */
	/*--------프로그램 동작 시퀸스 시작--------*/    
	DPRINTF("%s\r\n", "Start Main"); 
	MelodyStart(PARINGMELODY);  // 부저 소리 톤과 시간을 변경
	delay_ms(50); //부저 소리를 내기 위한 딜레이
	for( i =0 ; i<6 ; i++) //처음 프로그램 동작 시퀸스가 시작 되고 '0' FND로 뿌려주는 for문
	{
		GPIO_ResetBits(GPIOA, FND_array[i]); 
		delay_ms(200);
	}
	GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //모든 FND 초기 꺼진상태
	FND_On =1;	//초기 Mode 1로 진입하여 시작
	button= 1;
	i = 0;
	for(;;)
	{
		Button_Action(&bAction); //bAction은 exturn으로 헤더 파일에 정의가 되어있어서, false로 설정되어있음.
		switch(FND_On_Off) //FND 깜박 거림, 모드 선택 switch문
		{
			case 1 : //'1' FND로 표현
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G);  //모든 FND 초기 꺼진상태
				GPIO_ResetBits(GPIOA, FND_Pin_B|FND_Pin_C); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G);  //모든 FND 초기 꺼진상태
				delay_ms(500);
				break;
		
			case 2 :	//'2' FND로 표현
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //모든 FND 초기 꺼진상태
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_D|FND_Pin_E|FND_Pin_G); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //모든 FND 초기 꺼진상태
				delay_ms(500);
				break;	

			case 3 :	//'3' FND로 표현
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //모든 FND 초기 꺼진상태
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_G); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //모든 FND 초기 꺼진상태
				delay_ms(500);
				break;

			case 4 :	//'4' FND로 표현
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //모든 FND 초기 꺼진상태
				GPIO_ResetBits(GPIOA, FND_Pin_B|FND_Pin_C|FND_Pin_F|FND_Pin_G); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //모든 FND 초기 꺼진상태
				delay_ms(500);
				break;
			
			////////Mode 5 온도 조건 추가////////////
			case 15 : //'5' FND로 표현
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //모든 FND 초기 꺼진상태
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_C|FND_Pin_D|FND_Pin_F|FND_Pin_G); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //모든 FND 초기 꺼진상태
				delay_ms(500);
				break;
			
			default :
				break;
		}
		
		switch(FND_On) //FND 가 계속 켜지게 함, 모드 실행 switch문
		{
			case 1 : //'1' FND로 표현 Mode 1 대기상태
				GPIO_ResetBits(GPIOA, FND_Pin_B|FND_Pin_C);  //FND On
				//delay_ms(500);
				break;
		
			case 2 :	//'2' FND로 표현 Mode 2 대기 40초, 발열 30초
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_D|FND_Pin_E|FND_Pin_G); //FND On
				testtest=1;
				delay_s(41);//대기 40초
				if((Break ==true)) //long button일때 이 조건으로 인해 case문을 나가기 위한 조건
				{	
					FND_On =6;
					testtest=0;
					Break = false;
					break;
				}
				GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater On
				delay_s(31); //발열 30초
				if((Break ==true)) //long button일때 이 조건으로 인해 case문을 나가기 위한 조건
				{	
					FND_On =6;
					Break = false;
					testtest=0;
					break;
				}
				GPIO_ResetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater Off
				break;	
	
			case 3 :	//'3' FND로 표현 Mode 3 발열 계속
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_G); //FND On
				GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater On
				break;
							
					/*Mode 4는 시험시에만 조도센서 구현, 실제 제작 조도, 온도 구현*/		
			case 4 :	//'4' FND로 표현 Mode 4 조도센서 조건 모드 , 조도센서 어두워지면(포스트 잇으로 가리면) 히팅 동작 
				GPIO_ResetBits(GPIOA, FND_Pin_B|FND_Pin_C|FND_Pin_F|FND_Pin_G); //FND On
				test = ADCConvertedValue[0]; //조도센서 ADC값
				//조도 센서 측정
				V_CDS = (double)test * 3.3 / 4096 ;
				R_CDS = (33000/V_CDS) - 10000 ;
				LUX = pow(10, (2-log10((R_CDS/1000)))/0.78); //0.61 - smal 0.638 midum , 0.78 high (조도센서 크기에 따른 값 변경 수치)
				test = (uint16_t)LUX;
				printf("LUX = %d\r\n",test);
				delay_ms(300); // delay를 300ms 이상 주게디면 Sensor Mode 스위치 문 때문에 delay가 더 느려지게 된다. 1s이상 주게되면 2s~3s로 실행됨.
				if(Sensor_Mode==0) Sensor_Mode =5;
						switch(Sensor_Mode)
						{
							case 5 :	 //'A'			
									DPRINTF("%s\r\n", "Sensor_Mode1");
									//1지점 Top 빨간색 스티커
								
									if(!(((1800<test)&&(test<3000))||(test>7000))) //중복 제어
									{
										if(((3000<=test)&&(test<=7000))) //포스트잇 붙혔을때
										{
											if(a[0]==3) //조도센서의 값이 갑자기 튀는 것을 완화해주는 조건 
											{		//예) 신호등이 불이켜져 있고, 포스트 잇으로 가린경우 2800~7000정도의 값이 산출이되는데 간혹 1000 이하로 값이 한번씩 떨어지는 경우가 있다.
												GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater On
										
												a[0]=0;
												a[1]=0;
											}
											a[0]++;
										}
										else //뗏을때
										{
											if(a[1]==3)
											{
												GPIO_ResetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater Off
												a[0]=0;
												a[1]=0;
											}
											a[1]++;;
										}
									}
								
									break;
					
							default :
									break;
						}
				break;
			
			case 5 : // 온도 조건 모드, 온도센서 0도일경우 히팅 동작
					GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_C|FND_Pin_D|FND_Pin_F|FND_Pin_G); 
					test1 = ADCConvertedValue[1]; //온도센서 ADC값
					// 온도 센서 측정
					data.sd.sensorvalue = themotable[test1]; //온도가 10 미만이면 써미스터 저항 테이블을 그대로 사용하여도 실제 온도계와 오차가 거의 없게 나타난다.
					if(data.sd.sensorvalue>10) //온도값이 10이상일때는 써미스터 저항 테이블의 값이 조금더 높게 나타남으로 test+10을 하여 그값으로 환산함으로써 실제 온도계와 오차를 줄이기 위한 조건
					{
						data.sd.sensorvalue = themotable[test1+10];
					}
					printf("TH = %d도\r\n",data.sd.sensorvalue);
					if(!((3000<=test1)&&(test1<=3018))) // 써미스터 저항 테이블에서 0.9~1.1 정도의 값의 구간에서 값들이 왔다 갔다 함에 따라 heater가 켜졌다 꺼졌다를 반복하게 되는데, 이것을 방지하기 위한 조건
					{							// 디지털 온도계와 비교 테스트를 진행 하였을때 0.9~0.7까지의 범위값을 아무것도 안하는 상태로 처리 , (이전에 상태를 그대로 유지하게됨.)
						if(3019<=test1) // 디지털 온도계와 비교 테스트를 진행 하였을때 3019라는 써미스터 저항 테이블의 값은 대략 0.5~0.6 정도의 온도를 나타낸다. 그로므로 0.5~0.6도 이하이면 Heater가 켜지는 조건.
						{
							GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater On
						}
						else //3000은 써미스터 저항 테이블에서 온도 1(1.0)을 나타냄으로 1(1.1)도 정도 되는 순간 바로 Heater가 꺼짐.
						{
							GPIO_ResetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater Off
						}
					
					}
					delay_ms(1000);
				break;
			
			case 6 : 
				 
				break;
			
			default :
				break;
		}
		
		
	}
}

/*------------Printf 함수를 사용하기 위하여 사용하는 함수들-------------*/
//Use Printf
#pragma import(__use_no_semihosting_swi)
 
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
 
int fputc(int ch, FILE *f)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
 
  USART_SendData(USART1, ch);
 
  return(ch);
}
 
int fgetc(FILE *f)
{
    char ch;
 
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
 
    ch = USART_ReceiveData(USART1);
 
  return((int)ch);
}
 
int ferror(FILE *f)
{
  /* Your implementation of ferror */
  return EOF;
}
 
void _ttywrch(int ch)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
 
  USART_SendData(USART1, ch);
}
 
void _sys_exit(int return_code)
{
label:  goto label;  /* endless loop */


}
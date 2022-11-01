/**
  ******************************************************************************
  * @file    main.c 
  * @author  �̼���
  * @version V1.0
  * @date    19/12/2016
  * @brief   ���� �Լ�
  ******************************************************************************
  * @copy
  *
  * ���� �Լ� ����
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 
  
#include <stdio.h>
#include <math.h>
#include "stm32f10x_conf.h"
#include "THtable.h" //�µ����� ��̽��� ���� ���̺�
/**
 @brief ��ư�� ���� Mode ���� �� ����
 @param Mode ����
 @return void
 @deta
 Mode1					��� ��� (�ƹ� ��ɵ� ���� �ʴ� ����)
 Mode2					40�� ���, 30�� �߿�
 Mode3					�߿� ���
 Mode4					�������� ���� ���
 Mode5					�µ����� ���� ���
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

/*------TH(�µ����� ����)-------*/
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
	/*-------�µ�,�������� ��հ��� ���ϱ� ���� ������------*/
	int i;
	/*------------�µ�,�������� ���� ���� ����ü�� ������----------*/
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
	/*----------SysTick handler�� ���� �ϱ� ���� SysTick ����----------*/
	RCC_ClocksTypeDef RCC_Clocks;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	RCC_GetClocksFreq(&RCC_Clocks);
	if (SysTick_Config(RCC_Clocks.SYSCLK_Frequency /10))
	    {
	       		      ///* Capture error */
	       		      while (1);
	    }

	/* Setup SysTick Timer for 100 msec interrupts */
	/*--------���α׷� ���� ������ ����--------*/    
	DPRINTF("%s\r\n", "Start Main"); 
	MelodyStart(PARINGMELODY);  // ���� �Ҹ� ��� �ð��� ����
	delay_ms(50); //���� �Ҹ��� ���� ���� ������
	for( i =0 ; i<6 ; i++) //ó�� ���α׷� ���� �������� ���� �ǰ� '0' FND�� �ѷ��ִ� for��
	{
		GPIO_ResetBits(GPIOA, FND_array[i]); 
		delay_ms(200);
	}
	GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //��� FND �ʱ� ��������
	FND_On =1;	//�ʱ� Mode 1�� �����Ͽ� ����
	button= 1;
	i = 0;
	for(;;)
	{
		Button_Action(&bAction); //bAction�� exturn���� ��� ���Ͽ� ���ǰ� �Ǿ��־, false�� �����Ǿ�����.
		switch(FND_On_Off) //FND ���� �Ÿ�, ��� ���� switch��
		{
			case 1 : //'1' FND�� ǥ��
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G);  //��� FND �ʱ� ��������
				GPIO_ResetBits(GPIOA, FND_Pin_B|FND_Pin_C); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G);  //��� FND �ʱ� ��������
				delay_ms(500);
				break;
		
			case 2 :	//'2' FND�� ǥ��
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //��� FND �ʱ� ��������
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_D|FND_Pin_E|FND_Pin_G); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //��� FND �ʱ� ��������
				delay_ms(500);
				break;	

			case 3 :	//'3' FND�� ǥ��
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //��� FND �ʱ� ��������
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_G); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //��� FND �ʱ� ��������
				delay_ms(500);
				break;

			case 4 :	//'4' FND�� ǥ��
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //��� FND �ʱ� ��������
				GPIO_ResetBits(GPIOA, FND_Pin_B|FND_Pin_C|FND_Pin_F|FND_Pin_G); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //��� FND �ʱ� ��������
				delay_ms(500);
				break;
			
			////////Mode 5 �µ� ���� �߰�////////////
			case 15 : //'5' FND�� ǥ��
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //��� FND �ʱ� ��������
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_C|FND_Pin_D|FND_Pin_F|FND_Pin_G); 
				delay_ms(500);
				GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //��� FND �ʱ� ��������
				delay_ms(500);
				break;
			
			default :
				break;
		}
		
		switch(FND_On) //FND �� ��� ������ ��, ��� ���� switch��
		{
			case 1 : //'1' FND�� ǥ�� Mode 1 ������
				GPIO_ResetBits(GPIOA, FND_Pin_B|FND_Pin_C);  //FND On
				//delay_ms(500);
				break;
		
			case 2 :	//'2' FND�� ǥ�� Mode 2 ��� 40��, �߿� 30��
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_D|FND_Pin_E|FND_Pin_G); //FND On
				testtest=1;
				delay_s(41);//��� 40��
				if((Break ==true)) //long button�϶� �� �������� ���� case���� ������ ���� ����
				{	
					FND_On =6;
					testtest=0;
					Break = false;
					break;
				}
				GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater On
				delay_s(31); //�߿� 30��
				if((Break ==true)) //long button�϶� �� �������� ���� case���� ������ ���� ����
				{	
					FND_On =6;
					Break = false;
					testtest=0;
					break;
				}
				GPIO_ResetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater Off
				break;	
	
			case 3 :	//'3' FND�� ǥ�� Mode 3 �߿� ���
				GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_G); //FND On
				GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater On
				break;
							
					/*Mode 4�� ����ÿ��� �������� ����, ���� ���� ����, �µ� ����*/		
			case 4 :	//'4' FND�� ǥ�� Mode 4 �������� ���� ��� , �������� ��ο�����(����Ʈ ������ ������) ���� ���� 
				GPIO_ResetBits(GPIOA, FND_Pin_B|FND_Pin_C|FND_Pin_F|FND_Pin_G); //FND On
				test = ADCConvertedValue[0]; //�������� ADC��
				//���� ���� ����
				V_CDS = (double)test * 3.3 / 4096 ;
				R_CDS = (33000/V_CDS) - 10000 ;
				LUX = pow(10, (2-log10((R_CDS/1000)))/0.78); //0.61 - smal 0.638 midum , 0.78 high (�������� ũ�⿡ ���� �� ���� ��ġ)
				test = (uint16_t)LUX;
				printf("LUX = %d\r\n",test);
				delay_ms(300); // delay�� 300ms �̻� �ְԵ�� Sensor Mode ����ġ �� ������ delay�� �� �������� �ȴ�. 1s�̻� �ְԵǸ� 2s~3s�� �����.
				if(Sensor_Mode==0) Sensor_Mode =5;
						switch(Sensor_Mode)
						{
							case 5 :	 //'A'			
									DPRINTF("%s\r\n", "Sensor_Mode1");
									//1���� Top ������ ��ƼĿ
								
									if(!(((1800<test)&&(test<3000))||(test>7000))) //�ߺ� ����
									{
										if(((3000<=test)&&(test<=7000))) //����Ʈ�� ��������
										{
											if(a[0]==3) //���������� ���� ���ڱ� Ƣ�� ���� ��ȭ���ִ� ���� 
											{		//��) ��ȣ���� �������� �ְ�, ����Ʈ ������ ������� 2800~7000������ ���� �����̵Ǵµ� ��Ȥ 1000 ���Ϸ� ���� �ѹ��� �������� ��찡 �ִ�.
												GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater On
										
												a[0]=0;
												a[1]=0;
											}
											a[0]++;
										}
										else //������
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
			
			case 5 : // �µ� ���� ���, �µ����� 0���ϰ�� ���� ����
					GPIO_ResetBits(GPIOA, FND_Pin_A|FND_Pin_C|FND_Pin_D|FND_Pin_F|FND_Pin_G); 
					test1 = ADCConvertedValue[1]; //�µ����� ADC��
					// �µ� ���� ����
					data.sd.sensorvalue = themotable[test1]; //�µ��� 10 �̸��̸� ��̽��� ���� ���̺��� �״�� ����Ͽ��� ���� �µ���� ������ ���� ���� ��Ÿ����.
					if(data.sd.sensorvalue>10) //�µ����� 10�̻��϶��� ��̽��� ���� ���̺��� ���� ���ݴ� ���� ��Ÿ������ test+10�� �Ͽ� �װ����� ȯ�������ν� ���� �µ���� ������ ���̱� ���� ����
					{
						data.sd.sensorvalue = themotable[test1+10];
					}
					printf("TH = %d��\r\n",data.sd.sensorvalue);
					if(!((3000<=test1)&&(test1<=3018))) // ��̽��� ���� ���̺��� 0.9~1.1 ������ ���� �������� ������ �Դ� ���� �Կ� ���� heater�� ������ �����ٸ� �ݺ��ϰ� �Ǵµ�, �̰��� �����ϱ� ���� ����
					{							// ������ �µ���� �� �׽�Ʈ�� ���� �Ͽ����� 0.9~0.7������ �������� �ƹ��͵� ���ϴ� ���·� ó�� , (������ ���¸� �״�� �����ϰԵ�.)
						if(3019<=test1) // ������ �µ���� �� �׽�Ʈ�� ���� �Ͽ����� 3019��� ��̽��� ���� ���̺��� ���� �뷫 0.5~0.6 ������ �µ��� ��Ÿ����. �׷ιǷ� 0.5~0.6�� �����̸� Heater�� ������ ����.
						{
							GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater On
						}
						else //3000�� ��̽��� ���� ���̺��� �µ� 1(1.0)�� ��Ÿ������ 1(1.1)�� ���� �Ǵ� ���� �ٷ� Heater�� ����.
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

/*------------Printf �Լ��� ����ϱ� ���Ͽ� ����ϴ� �Լ���-------------*/
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
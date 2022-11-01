/**
  ******************************************************************************
  * @file    setting.c 
  * @author  �̼���
  * @version V1.0
  * @date    19/12/2016
  * @brief   MCU �ϵ���� ����
  ******************************************************************************
  * @copy
  *
  * MCU ��Ʈ ���� �� ADC���� ��
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 


#include "stm32f10x_conf.h"
#include "setting.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
__IO uint16_t ADCConvertedValue[7];

uint16_t PrescalerValue = 0;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; // ����ü ���� ����.
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure3;
TIM_OCInitTypeDef  TIM_OCInitStructure3;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure2;
TIM_OCInitTypeDef  TIM_OCInitStructure2;

void DMA_Configuration(void) // ���� ���ͷ�Ʈ��  ���� ���� ����� ����ϱ� ���� ���. 
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //DMA�� AHB Ŭ���� ��� �ϱ� ������ Enable

	DMA_DeInit(DMA1_Channel1); //DMA1 ä�� 1����
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //ADC1 DR �������� ���� �ּ� �� �Է�
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue; //ADCConvertedvalue �迭�� ���� �ּҸ� ����Ŵ, 32��Ʈ�� ���� ����ȯ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA�� ��ġ�� ���̷�Ʈ�� ���� ���ִ°�, PeripheralSRC ADC�Ҷ�.
	DMA_InitStructure.DMA_BufferSize = 2; // ADCConvertedvalue ��� �迭�� �� 2�� ������. (ADC ä�� 2���� ��� �� ���̱� ������)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // DMA�� ��ġ�ν� ������� ���� ���̴ϱ� Disable
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //DMA�� �޸� �ν� ����Ұ��̴ϱ� enable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ������ũ�⸦ 16��Ʈ�� ����. ADC�� 12��Ʈ �̱⶧��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //peripheraldatasize�� ����
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ADC�� ��ȯ��带 �ؾ� �ϹǷ�.
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //�켱���� ����.
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA�� ADC��� ��ġ�� �޸𸮴� ��ġ�μ� �Ұ��̹Ƿ� �޸� �� �޸� disable
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

}

void ADC1_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_ADCCLKConfig(RCC_PCLK2_Div4);  //ADCŬ�� 72Mhz/4�� ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

	//ADC1->CR2 |= ADC_CR2_TSVREFE; //ADC�� �µ��� ���� ��� ���.


	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //����尡 �ƴϱ⶧���� independent
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ADC������ scan�� continu��带 �� enable
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�ܺ� ���ͷ�Ʈ ��� ����.
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ADC ������ ������ ����
	ADC_InitStructure.ADC_NbrOfChannel = 2; //ä�� �׷� 2�� ���.
	ADC_Init(ADC1, &ADC_InitStructure); //ADC1�� ���

	/* ADC1 regular channel14 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5); //8ä��  CDS , 9ä�� TH
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 6, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 7, ADC_SampleTime_55Cycles5);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	//ADC�� callibration(����)�� �ѹ� ���ְ� ADC�� ���� �ؾ��Ѵ�.
	
	/* Enable ADC1 reset calibration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//���ڵ� ���� ADC ��ŸƮ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;   // Pin_0 - TEMP , Pin_1 - CDS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


}

void UsartInit(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure; //GPIO �� ��� �ϱ� ������ GPIO ����.
    USART_InitTypeDef USART_InitStructure; //USART �� ��� �� ���̱� ������ USART ����.

    /* Enable GPIOA clock                                                   */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //Ŭ�� �־��ִ°�. �翬�� ��.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //Ŭ�� �־��ִ°�. �翬�� ��.
    /* Configure USART1 Rx (PA10) as input floating                         */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10; //RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; //MCU ���忡�� �ޱ⸸ �ϸ� �Ǳ⶧���� ��� ���׵� �Ȱɷ� �־ �Ǳ� ������ �ܼ��� floating ���·� ����.
    														//�����Ͱ� ���ö� 1�̳� 0���� �����ĵ�  1�̳� 0������ pull up �� down �� �����ϱ⿡�� ��� �����Ͱ�
    														//������ �𸣴� floating ���·� ���δ� ��.
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Tx (PA9) as alternate function push-pull            */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; //TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //MCU ���忡�� ���������� push pull �̶�� ����(pull up ������)�� �޾Ƽ� �����͸� �Ѱ��ִ°�.
    												//���� open drain �̿��ٸ� �ܺ����� ����� �޾����� �� ��⿡ pull up , down ������ �ɷ�������� open drain ���
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ; // even ¦�� �ҷ��� wordlength�� 9��Ʈ�� �����ؾߵȴ�.
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //�帧���� ����.
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; //rx,tx Ȱ��ȭ
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

void FND_Configuration(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE);
	
	/* ---------------FND-----------------*/
	GPIO_InitStructure.GPIO_Pin =  FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G; // FND A,B,C,D,E,F,G
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //FND�� Anode Type �̱� ������ FND�� LED���� ��� 1�� ���־� �ʱ⿡ ���� ���·� ��Ÿ���� �Ѵ�
}
void BUZZER_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB |   RCC_APB2Periph_AFIO , ENABLE);
	/* ---------------BUZZER-----------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);   // BUZZER REMAP (PORTB 4)
}

void Heater_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB |   RCC_APB2Periph_AFIO , ENABLE);

	/* ---------------Heater-----------------*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); //Heater off

}

void Button_init(void)
{
	
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure1;
	
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure1);

	GPIOC->ODR &= ~ (1<<13); //14�� ��Ʈ�� 0�� �� �Է�. input ����̱� ������ 0�� ������ ����.
	GPIOC->ODR |= (1<<13); // 14�� ��Ʈ�� 1�� ���� �Է�. �ʱ�ȭ.


	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //Buzzer PB4 �����Ͽ� Buzzer�� ��Ʈ�� ��밡�������.
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
void NVIC_init(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //����ġ ���ͷ�Ʈ ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //Debug ���ͷ�Ʈ Ȯ��.
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable the TIM2 Interrupt */
	 NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIMER_Configuration(void)
{

	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // APB1�� �ִ� 42MHz����, APB2�� �ִ� 84MHz����.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // APB1�� �ִ� 42MHz���������� Ÿ�̸ӵ��� ���ļ��� �ι�� �Ǿ�
													// �ʱⰪ�� 84Mhz�� �Ǿ��ִ�.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1,DISABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE); //�� remap�� ������ ����� ����, GPIO,TIM3�� ��������� TIM3����.

	/* -----------------------------------------------------------------------
	   TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
	   The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
	   clock at 24 MHz the Prescaler is computed as following:
	   - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
	   SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
	   and Connectivity line devices and to 24 MHz for Low-Density Value line and
	   Medium-Density Value line devices

	   The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	   = 24 MHz / 666 = 36 KHz
	   TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	   TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
	   TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
	   TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
	   ----------------------------------------------------------------------- */
////////////�� ������Ʈ������ TIM4 ��� x/////////////////////////////////////
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 23999; //MAX�� ����.
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_TimeBaseStructure3.TIM_Period = 2400;
	TIM_TimeBaseStructure3.TIM_Prescaler = 71;
	TIM_TimeBaseStructure3.TIM_ClockDivision = 0; // 0���� Ŭ���� ���ڴٴ°�.
	TIM_TimeBaseStructure3.TIM_CounterMode = TIM_CounterMode_Up; //ī���� ��� , up, down , PWM �� ����.

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure3);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure3.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure2.TIM_OCMode = TIM_OCMode_Timing;


	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

 
	/* PWM3 Mode configuration: Channel1 */
	TIM_OCInitStructure3.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure3.TIM_Pulse = 0;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure3);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);



	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	/* Timing  Mode2 configuration: Channel */
	TIM_TimeBaseStructure2.TIM_Period = 36000/MELODYSPEED;
	TIM_TimeBaseStructure2.TIM_Prescaler = 5999;
	TIM_TimeBaseStructure2.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure2.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure2);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/*
	   TIM_OCInitStructure1.TIM_OCMode = TIM_OCMode_Timing;
	   TIM_OCInitStructure1.TIM_OutputState=TIM_OutputState_Disable;
	   TIM_OCInitStructure1.TIM_OutputNState=TIM_OutputNState_Disable;

	   TIM_OC1Init(TIM1, &TIM_OCInitStructure1);
	   TIM_OC2Init(TIM1, &TIM_OCInitStructure1);
	   TIM_OC3Init(TIM1, &TIM_OCInitStructure1);
	   TIM_OC4Init(TIM1, &TIM_OCInitStructure1);


	   TIM_TimeBaseStructure1.TIM_Period = 120;
	   TIM_TimeBaseStructure1.TIM_Prescaler = 5999;
	   TIM_TimeBaseStructure1.TIM_ClockDivision = TIM_CKD_DIV1;
	   TIM_TimeBaseStructure1.TIM_CounterMode = TIM_CounterMode_Up;
	   TIM_TimeBaseStructure1.TIM_RepetitionCounter = 0;
	   TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure1);
	   TIM_ARRPreloadConfig(TIM1, ENABLE);
	   TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	   TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	   TIM_Cmd(TIM1, ENABLE);

	 */
}

void Setting(void) //MCU�� ���� �ϴ� �Լ�
{
	UsartInit();
	DMA_Configuration();
	ADC1_Configuration();
	BUZZER_Configuration();
	Heater_Conf();
	TIMER_Configuration();
	Button_init();
	FND_Configuration();
	NVIC_init();
	
}

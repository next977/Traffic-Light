/**
  ******************************************************************************
  * @file    setting.c 
  * @author  이성일
  * @version V1.0
  * @date    19/12/2016
  * @brief   MCU 하드웨어 셋팅
  ******************************************************************************
  * @copy
  *
  * MCU 포트 설정 및 ADC설정 등
  *
  * <h2><center>&copy; COPYRIGHT 2016 AramSolution</center></h2>
  */ 


#include "stm32f10x_conf.h"
#include "setting.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
__IO uint16_t ADCConvertedValue[7];

uint16_t PrescalerValue = 0;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; // 구조체 변수 선언.
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure3;
TIM_OCInitTypeDef  TIM_OCInitStructure3;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure2;
TIM_OCInitTypeDef  TIM_OCInitStructure2;

void DMA_Configuration(void) // 내부 인터럽트나  내부 여러 기능을 사용하기 위해 사용. 
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //DMA는 AHB 클럭을 사용 하기 때문에 Enable

	DMA_DeInit(DMA1_Channel1); //DMA1 채널 1설정
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //ADC1 DR 레지스터 시작 주소 값 입력
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue; //ADCConvertedvalue 배열의 시작 주소를 가리킴, 32비트로 강제 형변환
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA와 장치로 다이렉트로 연결 해주는것, PeripheralSRC ADC할때.
	DMA_InitStructure.DMA_BufferSize = 2; // ADCConvertedvalue 라는 배열이 총 2개 때문에. (ADC 채널 2개만 사용 할 것이기 때문에)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // DMA는 장치로써 사용하지 않을 것이니까 Disable
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //DMA를 메모리 로써 사용할것이니까 enable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 데이터크기를 16비트로 설정. ADC는 12비트 이기때문
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //peripheraldatasize와 동일
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ADC는 순환모드를 해야 하므로.
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //우선순위 높게.
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA는 ADC라는 장치와 메모리대 장치로서 할것이므로 메모리 대 메모리 disable
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

}

void ADC1_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_ADCCLKConfig(RCC_PCLK2_Div4);  //ADC클럭 72Mhz/4로 분주
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

	//ADC1->CR2 |= ADC_CR2_TSVREFE; //ADC의 온도값 측정 기능 사용.


	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //듀얼모드가 아니기때문에 independent
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ADC에서는 scan과 continu모드를 다 enable
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //외부 인터럽트 사용 안함.
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ADC 데이터 오른쪽 정렬
	ADC_InitStructure.ADC_NbrOfChannel = 2; //채널 그룹 2개 사용.
	ADC_Init(ADC1, &ADC_InitStructure); //ADC1번 사용

	/* ADC1 regular channel14 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5); //8채널  CDS , 9채널 TH
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
	//ADC를 callibration(보정)을 한번 해주고 ADC를 시작 해야한다.
	
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
	//위코드 부터 ADC 스타트
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;   // Pin_0 - TEMP , Pin_1 - CDS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


}

void UsartInit(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure; //GPIO 를 사용 하기 때문에 GPIO 설정.
    USART_InitTypeDef USART_InitStructure; //USART 를 사용 할 것이기 때문에 USART 설정.

    /* Enable GPIOA clock                                                   */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //클럭 넣어주는것. 당연한 것.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //클럭 넣어주는것. 당연한 것.
    /* Configure USART1 Rx (PA10) as input floating                         */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10; //RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; //MCU 입장에서 받기만 하면 되기때문에 어떠한 저항도 안걸려 있어도 되기 때문에 단순히 floating 상태로 받음.
    														//데이터가 들어올때 1이나 0으로 들어올탠데  1이나 0에서의 pull up 과 down 을 설정하기에는 어떠한 데이터가
    														//들어올지 모르니 floating 상태로 냅두는 것.
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Tx (PA9) as alternate function push-pull            */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; //TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //MCU 입장에서 내부적으로 push pull 이라는 저항(pull up 같은것)을 달아서 데이터를 넘겨주는것.
    												//만일 open drain 이였다면 외부적인 모듈을 달았을때 그 모듈에 pull up , down 저항이 걸려있을경우 open drain 사용
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ; // even 짝수 할려면 wordlength를 9비트로 설정해야된다.
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //흐름제어 없음.
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; //rx,tx 활성화
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
	
	GPIO_SetBits(GPIOA, FND_Pin_A|FND_Pin_B|FND_Pin_C|FND_Pin_D|FND_Pin_E|FND_Pin_F|FND_Pin_G); //FND가 Anode Type 이기 때문에 FND의 LED들을 모두 1로 해주어 초기에 꺼진 상태로 나타내게 한다
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

	GPIOC->ODR &= ~ (1<<13); //14번 비트만 0의 값 입력. input 모드이기 때문에 0의 값으로 동작.
	GPIOC->ODR |= (1<<13); // 14번 비트만 1의 값을 입력. 초기화.


	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //Buzzer PB4 리맵하여 Buzzer를 포트로 사용가능토록함.
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
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //스위치 인터럽트 설정
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //Debug 인터럽트 확인.
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


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // APB1은 최대 42MHz지원, APB2는 최대 84MHz지원.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // APB1은 최대 42MHz지원하지만 타이머동안 주파수가 두배로 되어
													// 초기값이 84Mhz로 되어있다.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1,DISABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE); //핀 remap은 그핀의 기능을 설정, GPIO,TIM3등 여러기능중 TIM3설정.

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
////////////이 프로젝트에서는 TIM4 사용 x/////////////////////////////////////
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 23999; //MAX값 설정.
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_TimeBaseStructure3.TIM_Period = 2400;
	TIM_TimeBaseStructure3.TIM_Prescaler = 71;
	TIM_TimeBaseStructure3.TIM_ClockDivision = 0; // 0부터 클럭을 세겠다는것.
	TIM_TimeBaseStructure3.TIM_CounterMode = TIM_CounterMode_Up; //카운터 모드 , up, down , PWM 등 선택.

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

void Setting(void) //MCU를 세팅 하는 함수
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

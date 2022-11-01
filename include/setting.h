#ifndef __SETTING_H
#define __SETTING_H
#include "stm32f10x_conf.h"
/*---------FND----------*/


#define FND_Pin_A GPIO_Pin_1
#define FND_Pin_B GPIO_Pin_2
#define FND_Pin_C GPIO_Pin_3
#define FND_Pin_D GPIO_Pin_4
#define FND_Pin_E GPIO_Pin_5
#define FND_Pin_F  GPIO_Pin_6
#define FND_Pin_G GPIO_Pin_7

extern void UsartInit(void);
extern void DMA_Configuration(void);
extern void ADC1_Configuration(void);
extern void BUZZER_Configuration(void);
extern void Heater_Conf(void);
extern void TIMER_Configuration(void);
extern void Button_init(void);
extern void FND_Configuration(void);
extern void NVIC_init(void);
	

extern void Setting(void);

extern uint16_t PrescalerValue ;

extern __IO uint16_t ADCConvertedValue[7];

#endif


	
	
	
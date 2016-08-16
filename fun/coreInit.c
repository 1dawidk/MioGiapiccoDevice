#ifndef COREINIT_C_
#define COREINIT_C_
#include "coreInit.h"

void RCC_config(void)
{
		
		ErrorStatus HSEStartUpStatus;  //zmienna opisujaca rezultat uruchomienia HSE

  RCC_DeInit();	                                         //Reset ustawien RCC
  RCC_HSEConfig(RCC_HSE_ON);                             //Wlaczenie HSE
  HSEStartUpStatus = RCC_WaitForHSEStartUp();            //Odczekaj az HSE bedzie gotowy
  if(HSEStartUpStatus == SUCCESS) {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//
    FLASH_SetLatency(FLASH_Latency_2);                   //ustaw zwloke dla pamieci Flash; zaleznie od taktowania rdzenia
                                                         //0:<24MHz; 1:24~48MHz; 2:>48MHz
    RCC_HCLKConfig(RCC_SYSCLK_Div1);                     //ustaw HCLK=SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                      //ustaw PCLK2=HCLK
    RCC_PCLK1Config(RCC_HCLK_Div2);                      //ustaw PCLK1=HCLK/2
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //ustaw PLLCLK = HSE*9 czyli 8MHz * 9 = 72 MHz
    RCC_PLLCmd(ENABLE);							                     //wlacz PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);  //odczekaj na poprawne uruchomienie PLL
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);           //ustaw PLL jako zrodlo sygnalu zegarowego
    while(RCC_GetSYSCLKSource() != 0x08);                //odczekaj az PLL bedzie sygnalem zegarowym systemu

		//RCC_HSICmd(DISABLE);
		
	  RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	  
    // taktowanie GPIO
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
			
		// taktowanie APB1
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
			
		// taktowanie APB2
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
			
			
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  }
}

void GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	
	//###################################### GPIO E CONFIG
	
		/* System Leds */
			GPIO_InitStruct.GPIO_Pin= SYS_LED_PIN | FUN_LED_PIN | ERROR_LED_PIN | WIFI_LED_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(LED_SEG), &GPIO_InitStruct);	
	
		/* WiFi Module - USART1 TX */
			GPIO_InitStruct.GPIO_Pin= USART1_TX_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(USART1_SEG), &GPIO_InitStruct);
	
		/* WiFi Module - USART1 RX */
			GPIO_InitStruct.GPIO_Pin= USART1_RX_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(USART1_SEG), &GPIO_InitStruct);
			
		/* WiFi Module - Control */
			GPIO_InitStruct.GPIO_Pin= WIFI_RST_PIN | WIFI_CHPD_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(WIFI_PWR_SEG), &GPIO_InitStruct);
			
		/* WiFi Module - Control */
			GPIO_InitStruct.GPIO_Pin= WIFI_GPIO0_PIN | WIFI_GPIO2_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(WIFI_GPIO_SEG), &GPIO_InitStruct);
			
		/* DH11 Module */
			GPIO_InitStruct.GPIO_Pin= DH11_ONOFF_PIN | DH11_DATA0_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(DH11_SEG), &GPIO_InitStruct);
	
		//ADC
			GPIO_InitStruct.GPIO_Pin= ADC_1_PIN | ADC_2_PIN | ADC_3_PIN | ADC_4_PIN | ADC_5_PIN | ADC_6_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AIN;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(ADC_SEG), &GPIO_InitStruct);	
	
		//ONOFF
			GPIO_InitStruct.GPIO_Pin= SOILHYGROMETER_ONOFF_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(SOILHYGROMETER_ONOFF_SEG), &GPIO_InitStruct);

		
	
		//PWM
			GPIO_InitStruct.GPIO_Pin= ENGINE_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(ENGINE_SEG), &GPIO_InitStruct);
			
			//BUTTONS
			GPIO_InitStruct.GPIO_Pin= WIFIMODE_BUTTON_PIN;
			GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;
			GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIO_SEG(BUTTONS_SEG), &GPIO_InitStruct);

//-------- I2C1
	GPIO_InitStruct.GPIO_Pin = (I2C_SDA_PIN | I2C_SCL_PIN);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	
	GPIO_Init(GPIO_SEG(I2C_SEG), &GPIO_InitStruct);
}

void NVIC_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//Wybranie grupy priorytetów
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //grupa prio nr 2 || 4 grupy po 4 podprio

//#################### GRUPA 1 #################################
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	NVIC_Init(&NVIC_InitStructure);
//##############################################################
	
//#################### GRUPA 2 #################################	
//##############################################################
	
//#################### GRUPA 2 #################################
//##############################################################
	
//#################### GRUPA 3 #################################	
//##############################################################
}

#define SYSTICK_CLK_LOAD	9000000

uint32_t SysTick_Init(uint32_t ticks)
{
	uint32_t SysTickSett;
	
	if(ticks > SysTick_LOAD_RELOAD_Msk) return 0;
	SysTick->LOAD= (ticks&SysTick_LOAD_RELOAD_Msk) -1;
	
	NVIC_SetPriority(SysTick_IRQn, 0);
	
	SysTick->VAL= 0;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	
	SysTickSett= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	
	SysTick->CTRL |= SysTickSett;
	
	return 0;
}

void RTC_Init(void)
{
	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_SetPrescaler(38500);
	RTC_WaitForLastTask();

}

extern volatile uint32_t seconds;

void _delay_us(uint32_t us)
{
	int32_t stopVal=(int32_t)SysTick->VAL - us*9;
	uint32_t sec=seconds;
	
	if(stopVal<0)
	{
		sec++;
		stopVal+=SYSTICK_CLK_LOAD;
	}
	
	while(sec>seconds || stopVal<SysTick->VAL);
}

SystemTimestamp getTimeStamp(void)
{
	SystemTimestamp ts;
	
	ts.us= (int32_t)SysTick->VAL/9;
	ts.s= seconds;
	
	return ts;
}

uint32_t getDuration(SystemTimestamp from)
{
	if(from.s==seconds)
	{
		return from.us-((int32_t)SysTick->VAL/9);
	}
	else
	{
		return from.us+(SYSTICK_CLK_LOAD-((int32_t)SysTick->VAL/9));
	}
}

void _delay_ms(uint32_t ms)
{
	_delay_us(ms*1000);
}

void CoreInit(void)
{
	RCC_config();
	RTC_Init();
	SysTick_Init(SYSTICK_CLK_LOAD);
	GPIO_config();
	NVIC_config();
	__enable_irq();
}

#endif

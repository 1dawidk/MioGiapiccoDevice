#ifndef FUN_C_
#define FUN_C_

#include "fun.h"

uint16_t adcData[5];

/**
*	@brief Call to show error
*/
void BlinkError(void)
{
	int i=0;
	
	for(i=0; i<5; i++)
	{
		ERROR_LED_ON;
		_delay_ms(100);
		ERROR_LED_OFF;
		_delay_ms(100);
	}
}

/**
*	@brief Initialize PWM for Engine control
*/
void EngineInit(void)
{
	TimerBaseInit(TIM2, 500, 1440ul);
		TimerPWMxOutInit(TIM2, 4, TIM_OCIdleState_Reset);
	TimerPWMStart(TIM2);
}

/**
*	@brief 			Initialize WiFi module
* @param mode	Module mode - Access Point, Client
*/
void WiFiInit(uint8_t mode)
{
	uint8_t errorCode;
	
	FUN_LED_ON;
	DMAforUART1_RX_config((unsigned long int)&(_UART1_RxLine[0]), UART1_RX_LINE_SIZE);
  UART1_init(115200);
	FUN_LED_OFF;
	
	WIFI_RST_H;
	WIFI_POWER_ON;
	WIFI_GPIO0_H;
	WIFI_GPIO2_H;
	
	_delay_ms(1000);
	
	
	WIFI_LED_ON;
	FUN_LED_ON;
		
	if(mode==WIFI_MODE_AP)
	{
		errorCode=WiFi_InitApMode("PlantCloudSystem","mynewplanttest",ESP8266_ENMODE_WAP_WAP2_PSK);
	}
	else
	{
		errorCode=WiFi_InitClientMode();
		//errorCode=WiFi_StartWiFiConnecting("FunBox-83CB","domek123");
	}
	
	if(errorCode==ESP8266_STATE_INIT_ERROR){
		BlinkError();
		WIFI_LED_OFF;
	}
	
	FUN_LED_OFF;
}


/**
*	@brief Initialize Soil Hygrometer (ADC and DMA)
*/
void SoilHygrometerInsolationInit(void)
{
	DMAforADC_config((unsigned long int)(&adcData[0]), 5);
	ADC_config();
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	SOILMOISTURE_DEVICE_OFF;
}

void DHT11Init(void)
{
	uint8_t tmp1, tmp2;
	
	DHT11_DATA0_OUT;
	DHT11_DATA0_L;
	
	DHT11_OFF;
	_delay_ms(900);
	DHT11_ON;
	
	getDHTData(&tmp1, &tmp2);
}

void MemoryInit(SystemConfig_dt *sysCfg)
{
	I2C1_init();
	_delay_ms(20);
	
	if(EERead_Byte(EE_ADR_FIRSTRUN))
	{
		
	}
	else
	{
		
	}
}


/**
*	@brief Used to start Plant System
*	@return Initialization result. OK or ERROR
*/
uint8_t StartSys(SystemConfig_dt *sysCfg)
{
	CoreInit();
	SYS_LED_OFF;
	FUN_LED_OFF;
	ERROR_LED_OFF;
	WIFI_LED_OFF;
	
	EngineInit();
	SoilHygrometerInsolationInit();
	DHT11Init();
	MemoryInit(sysCfg);
	
	
	if(WIFIMODE_BUTTON_STATE)
	{
		WiFiInit(WIFI_MODE_AP);
		Server_StartRxListener("<h1>Hi, my name is Plant!</h1><p>First set up my configuration</p>");
		sysCfg->mode=SYS_MODE_CONFIG;
	}
	else
	{
		WiFiInit(WIFI_MODE_CLIENT);
		sysCfg->mode=SYS_MODE_NORMAL;
	}
	
	return 0;
}

uint8_t getSoilMoisture(void)
{
	uint8_t soilMoistureData;
	
	SOILMOISTURE_DEVICE_ON;
	_delay_ms(100);
	soilMoistureData=(adcData[0]*100)/MAX_SOILMOISTURE;
	_delay_ms(20);
	SOILMOISTURE_DEVICE_OFF;
	
	
	return soilMoistureData;
}

uint8_t getInsolation(void)
{
	uint8_t insolation;
	
	SOILMOISTURE_DEVICE_ON;
	_delay_ms(100);
	insolation= (adcData[1]*100)/MAX_INSOLATION;
	_delay_ms(20);
	SOILMOISTURE_DEVICE_OFF;
	
	return insolation;
}

#define STATE_HIGH	1
#define STATE_LOW		0

uint8_t DHT11_waitFor(uint8_t state)
{
	uint32_t ackCnt=100000;
	
	if(state==STATE_HIGH)
	{
		while(!DHT11_DATA0_READ)
			if(ackCnt-- <= 0)
				return 1; //Return Error
	}
	else
	{
		while(DHT11_DATA0_READ)
			if(ackCnt-- <= 0)
				return 1; //Return Error
	}
	
	return DHT11_DATA0_READ;
}

uint8_t getDHTData(uint8_t *temp, uint8_t *RH)
{
	uint8_t i;
	uint8_t buffer[5];
	
	SystemTimestamp ts;
	
	for(i=0; i<5; i++)
		buffer[i]=0;
	
	/* Send start signal */
	DHT11_DATA0_OUT;
	DHT11_DATA0_L;
	_delay_ms(30);
	DHT11_DATA0_H;
	_delay_us(20);
	
	/* Receive start signal */
	DHT11_DATA0_IN;

	if(DHT11_waitFor(STATE_HIGH)) return 1; //Return Error
	
	if(DHT11_waitFor(STATE_LOW)) return 2; //Return Error
	
	/* Receive 40-bit data */
	for(i=0; i<40; i++)
	{
		/* Wait for high state */
		if(DHT11_waitFor(STATE_HIGH)) return 3; //Return Error
		
		/* Check high state length */
			ts= getTimeStamp();
		
		if(DHT11_waitFor(STATE_LOW)) return 4; //Return Error
		
		if(getDuration(ts)>40)
				buffer[i/8]|=(1<<(7-(i%8)));
	}
	
	if(buffer[0]+buffer[2]!=buffer[4]) return 5;
	
	*temp= buffer[2];
	*RH= buffer[0];
	
	return 0;
}

void setEngineRPM(uint16_t rpm)
{
	TIM2->CCR4= rpm;
}

void NormalModeLoopStep(void)
{

}

#endif

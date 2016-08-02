#ifndef FUN_C_
#define FUN_C_

#include "fun.h"

uint16_t adcData[6];

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
	uint32_t startClk;
	
	FUN_LED_ON;
	DMAforUART1_RX_config((unsigned long int)&(_UART1_RxLine[0]), UART1_RX_LINE_SIZE);
  UART1_init(115200);
	FUN_LED_OFF;
	
	WIFI_RST_H;
	WIFI_POWER_ON;
	WIFI_GPIO0_H;
	WIFI_GPIO2_H;
	
	startClk=CLOCK;
	while(!strstr((const char*)_UART1_RxLine, "ready") && CLOCK-startClk<10);
	
	WIFI_LED_ON;
	FUN_LED_ON;
		
	if(mode==WIFI_MODE_AP)
	{
		errorCode=WiFi_InitApMode("PlantCloudSystem","mynewplantpass",ESP8266_ENMODE_WAP_WAP2_PSK);
	}
	else
	{
		errorCode=WiFi_InitClientMode();
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
	DMAforADC_config((unsigned long int)(&adcData[0]), 6);
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
	_delay_ms(100);
	DHT11_ON;
	
	getDHTData(&tmp1, &tmp2);
}

void MemoryInit(SystemConfig_dt *sysCfg, Plant_dt *plants)
{
	uint8_t i;
	uint8_t readResult;
	uint32_t j;
	
	EEInit(0xa0);
	
	DMAforUART1_RX_config((unsigned long int)&(_UART1_RxLine[0]), UART1_RX_LINE_SIZE);
  UART1_init(115200);
	
	if(FACTORY_BOOT)
	{
		EEClear();
	}
	
	
	readResult= EERead_Byte(EE_ADR_FIRSTRUN);
	if(readResult)
	{		
		sysCfg->mode= SYS_MODE_FIRSTRUN;
		
		sysCfg->WiFi_ssid="";
		sysCfg->WiFi_pass="";
		sysCfg->user_login="";
		sysCfg->user_pass="";
		
		sysCfg->plantsCnt=0;
	}
	else
	{
		sysCfg->WiFi_ssid= EERead_String(EE_ADR_SSID);
		sysCfg->WiFi_pass= EERead_String(EE_ADR_PASS);
		
		sysCfg->user_login= EERead_String(EE_ADR_USER_LOGIN);
		sysCfg->user_pass= EERead_String(EE_ADR_USER_PASS);
		sysCfg->plantsCnt= EERead_Byte(EE_ADR_PLANTSCNT);
		
		for(i=0; i<(sysCfg->plantsCnt); i++)
		{
			(&(plants[i]))->name= EERead_String(EE_ADR_PLANTNAME_BASE+i);
		}
		
		sysCfg->WiFi_ssid="dlink";
		sysCfg->WiFi_pass="sikakama1";
		sysCfg->user_login="1dawidk";
		sysCfg->user_pass="sikakama1";
		
		sysCfg->plantsCnt=2;
		(plants)->name="Poziomka";
		(plants+1)->name="Lawenda";
	}
}

#define REGISTER_STATE_FINISH				0
#define REGISTER_STATE_TALKING			1
#define REGISTER_STATE_START				2

uint8_t RegisterToServer(HttpHeaderParam *httpReqParams, SystemConfig_dt *sysCfg, Plant_dt *plants)
{
	uint32_t startClk=CLOCK;
	uint8_t server_responseCode;
	uint8_t state=REGISTER_STATE_START;
	uint8_t register_errorCode=0;
	uint8_t i;
	char deviceIdStr[5];
	char plantsCntStr[2];
	char plantNameStr[8];
	char *jValue;
	char *server_responseStr;
	HttpVar_dt httpVars[8];
	
	httpVars[0].name="Login";
	httpVars[0].value=	sysCfg->user_login;
	
	httpVars[1].name="Pass";
	httpVars[1].value=sysCfg->user_pass;
	
	httpVars[2].name="DeviceId";
	sprintf(deviceIdStr, "%d", DEVICE_ID);
	httpVars[2].value=deviceIdStr;
	
	httpVars[3].name="PlantsCnt";
	sprintf(plantsCntStr, "%d", sysCfg->plantsCnt);
	httpVars[3].value=plantsCntStr;
	
	httpVars[4].name="Plant1";
	httpVars[5].name="Plant2";
	httpVars[6].name="Plant3";
	httpVars[7].name="Plant4";
	
	for(i=0; i<sysCfg->plantsCnt; i++)
	{
		httpVars[4+i].value=plants[i].name;
	}
	
	while((CLOCK-startClk<10) && state!=REGISTER_STATE_FINISH)
	{
		switch(state)
		{
			case REGISTER_STATE_START:
				Server_ConnectTo("www.dawidkulpa.pl");
				UART1_flushRx();
				Server_Request(POST, SERVER_CFG_PATH, httpVars, 4+sysCfg->plantsCnt, httpReqParams, 4);
				state=REGISTER_STATE_TALKING;
				break;
			case REGISTER_STATE_TALKING:
				server_responseCode= Server_checkForResponse((char *)_UART1_RxLine);
				if(server_responseCode!=0)
				{
					server_responseStr=Server_GetBody(_UART1_RxLine);
					
					for(i=0; i<sysCfg->plantsCnt; i++)
					{
						sprintf(plantNameStr, "Plant%d", i+1);
						jValue= JSON_getJValue(plantNameStr, server_responseStr);
						sscanf(jValue, "%d", &(plants[i].id));
						free(jValue);
					}
					
					state= REGISTER_STATE_FINISH;
					
					free(server_responseStr);
				}
				break;
		}
	}
	
	if(CLOCK-startClk>10)
		register_errorCode=1;
	
	return register_errorCode;
}

char* BuildAPResponse(SystemConfig_dt *sysCfg, Plant_dt *plants)
{
	char *response;
	char *plantNameTag;
	uint16_t responseLen=0;
	uint8_t i;
	
	responseLen+= strlen("{\"Answ\":\"Hello!\""); //Dodanie dlugosci wymaganego poczatku
	
	responseLen+= strlen(",\"");									//WiFi SSID
	responseLen+= strlen(SERVER_VAR_WIFI_SSID);
	responseLen+=	strlen("\":\"");
	responseLen+= strlen(sysCfg->WiFi_ssid);
	responseLen+= strlen("\"");
	
	responseLen+= strlen(",\"");									//WiFi Pass
	responseLen+= strlen(SERVER_VAR_WIFI_PASS);
	responseLen+=	strlen("\":\"");
	responseLen+= strlen(sysCfg->WiFi_pass);
	responseLen+= strlen("\"");
	
	responseLen+= strlen(",\"");									//Plants Cnt
	responseLen+= strlen(SERVER_VAR_PLANTS_CNT);
	responseLen+=	strlen("\":");
	responseLen+= 1;
	
	for(i=0; i<sysCfg->plantsCnt; i++)
	{
		responseLen+= strlen(",\"");									//Plant Name
		responseLen+= strlen(SERVER_VAR_PLANT_NAME_BASE)+1;
		responseLen+=	strlen("\":\"");
		responseLen+= strlen(plants[i].name);
		responseLen+= strlen("\"");
	}
	
	responseLen+= strlen("}")+1;									//Zakonczenie i NULL
	
	response= malloc(sizeof(char)*responseLen);
	
	//Utworzenie odpowiedzi AP
	sprintf(response, "{\"Answ\":\"Hello!\",\"wifiSSID\":\"%s\",\"wifiPass\":\"%s\",\"PlantsCnt\":%d", sysCfg->WiFi_ssid, sysCfg->WiFi_pass, sysCfg->plantsCnt);
	
	for(i=0; i<sysCfg->plantsCnt; i++)
	{
		sprintf(plantNameTag,"Plant%d", i);
		strcat(response, ",\"");
		strcat(response, plantNameTag);
		strcat(response, "\":\"");
		strcat(response, plants[i].name);
		strcat(response, "\"");
	}
	
	strcat(response, "}");
	
	return response;
}

/**
*	@brief Used to start Plant System
*	@return Initialization result. OK or ERROR
*/
uint8_t StartSys(SystemConfig_dt *sysCfg, Plant_dt *plants)
{
	SYS_LED_OFF;
	FUN_LED_OFF;
	ERROR_LED_OFF;
	WIFI_LED_OFF;
	
	
	FUN_LED_ON;
	EngineInit();
	SoilHygrometerInsolationInit();
	DHT11Init();
	MemoryInit(sysCfg, plants);
	FUN_LED_OFF;
	
	if(WIFIMODE_BUTTON_STATE || sysCfg->mode==SYS_MODE_FIRSTRUN)
	{		
		WiFiInit(WIFI_MODE_AP);
		Server_StartRxListener(BuildAPResponse(sysCfg, plants));
		sysCfg->mode=SYS_MODE_CONFIG;
	}
	else 
	{
		WiFiInit(WIFI_MODE_CLIENT);
		sysCfg->mode=SYS_MODE_NORMAL;
		WiFi_StartWiFiConnecting(sysCfg->WiFi_ssid, sysCfg->WiFi_pass);
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
	insolation= (adcData[5]*100)/MAX_INSOLATION;
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

void MainLoop_NormalStep(SystemConfig_dt *sysCfg, Plant_dt *plants){
}

#endif

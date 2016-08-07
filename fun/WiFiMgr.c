#ifndef _WIFI_MGR_C
#define _WIFI_MGR_C

#include "WiFiMgr.h"

uint8_t sleep;

uint8_t WiFi_InitApMode(char *SSID, char *pass, uint8_t enMode)
{
	char apData[40];
	uint16_t readStart;
	
	readStart= UART1_RxCurrent;
	esp8266_sendData(ESP8266_CMD_MODE, ESP8266_CWMODE_AP);
	if(esp8266_waitForResp(3, readStart)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
	
	
	readStart= UART1_RxCurrent;
	sprintf(apData, "\"%s\",\"%s\",3,%d", SSID, pass, enMode);
	esp8266_sendData(ESP8266_CMD_SAP, apData);
	if(esp8266_waitForResp(3, readStart)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
	
	
	readStart= UART1_RxCurrent;
	esp8266_sendData(ESP8266_CMD_MUX, ESP8266_MUXMODE_MULTI);
	if(esp8266_waitForResp(3, readStart)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;

	
	readStart= UART1_RxCurrent;
	esp8266_sendData(ESP8266_CMD_SERVER, "1,80");
	if(esp8266_waitForResp(3, readStart)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
	
	
	readStart= UART1_RxCurrent;
	esp8266_sendData(ESP8266_CMD_STO, "3");
	if(esp8266_waitForResp(3, readStart)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;

	
	return ESP8266_STATE_OK;
}

uint8_t WiFi_InitClientMode(void)
{
	uint16_t readStart=UART1_RxCurrent;
	
	esp8266_sendData(ESP8266_CMD_MODE, ESP8266_CWMODE_STA);
	if(esp8266_waitForResp(3, readStart)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
	
	return ESP8266_STATE_OK;
}

void WiFi_Sleep(void)
{
	WIFI_POWER_OFF;
	sleep=TRUE;
}

uint8_t WiFi_IsAsleep(void)
{
	return sleep;
}

uint8_t WiFi_Wakeup(void)
{
	uint16_t readStart=UART1_RxCurrent;
	
	WIFI_POWER_ON;
	if(esp8266_waitForSpecResp("ready", 3, readStart)!=ESP8266_RESPONSE_OK)
	{
		sleep=FALSE;
		return ESP8266_STATE_INIT_ERROR;
	}
	
	sleep=FALSE;
	return ESP8266_STATE_OK;
}

uint8_t WiFi_StartWiFiConnecting(char *SSID, char *pass)
{
	char apData[60];
	uint16_t readStart=UART1_RxCurrent;
	
	sprintf(apData, "\"%s\",\"%s\"", SSID, pass);
	esp8266_sendData(ESP8266_CMD_JAP, apData);
	if(esp8266_waitForResp(20, readStart)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
	
	return ESP8266_STATE_OK;
}

uint8_t WiFi_IsWiFiConnected(void)
{
	uint8_t esp8266_response;
	uint16_t readStart=UART1_RxCurrent;
	
	esp8266_sendAsk(ESP8266_CMD_JAP);
	esp8266_response= esp8266_waitForSpecResp("No AP", 4, readStart);
	
	if(esp8266_waitForResp(5, readStart)==ESP8266_RESPONSE_OK)
	{
		if(esp8266_response==ESP8266_RESPONSE_OK)
		{
			return WIFI_DISCONNECTED;
		}
		return WIFI_CONNECTED;
	}
	else
	{
		return WIFI_DISCONNECTED;
	}
	
}

#endif

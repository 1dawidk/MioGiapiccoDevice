#ifndef _WIFI_MGR_C
#define _WIFI_MGR_C

#include "WiFiMgr.h"

uint8_t WiFi_InitApMode(char *SSID, char *pass, uint8_t enMode)
{
	char apData[40];
	
	uint32_t startTime=CLOCK;
	esp8266_sendData(ESP8266_CMD_MODE, ESP8266_CWMODE_AP);
	if(esp8266_waitForResp(20)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
	
	
	sprintf(apData, "\"%s\",\"%s\",3,%d", SSID, pass, enMode);
	esp8266_sendData(ESP8266_CMD_SAP, apData);
	if(esp8266_waitForResp(20)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
	
	
	esp8266_sendData(ESP8266_CMD_MUX, ESP8266_MUXMODE_MULTI);
	if(esp8266_waitForResp(20)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;

	
	esp8266_sendData(ESP8266_CMD_SERVER, "1,80");
	if(esp8266_waitForResp(20)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
	
	
	esp8266_sendData(ESP8266_CMD_STO, "3");
	if(esp8266_waitForResp(20)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;

	
	return ESP8266_STATE_OK;
}

uint8_t WiFi_InitClientMode(void)
{
	char apData[60];
	
	uint32_t startTime=CLOCK;
	esp8266_sendData(ESP8266_CMD_MODE, ESP8266_CWMODE_STA);
	if(esp8266_waitForResp(3)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
}

uint8_t WiFi_StartWiFiConnecting(char *SSID, char *pass)
{
	char apData[60];
	
	sprintf(apData, "\"%s\",\"%s\"", SSID, pass);
	esp8266_sendData(ESP8266_CMD_JAP, apData);
	if(esp8266_waitForResp(20)!=ESP8266_RESPONSE_OK)
		return ESP8266_STATE_INIT_ERROR;
	
}

uint8_t WiFi_IsWiFiConnected(void)
{
	uint8_t esp8266_response;
	
	esp8266_sendAsk(ESP8266_CMD_JAP);
	esp8266_response= esp8266_waitForSpecResp("No AP", 2);
	
	if(esp8266_response==ESP8266_RESPONSE_OK || esp8266_response==ESP8266_RESPONSE_BUSY)
	{
		return WIFI_DISCONNECTED;
	}
	
	
	return WIFI_CONNECTED;
}

#endif

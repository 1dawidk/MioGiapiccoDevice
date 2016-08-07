#ifndef _ESP8266_DRV_C
#define _ESP8266_DRV_C

#include "esp8266Drv.h"

void esp8266_sendCmd(char *cmd)
{
	UART1_putStr("AT+");
	UART1_putStr(cmd);
	UART1_putStr("\r\n");
}

void esp8266_sendData(char *cmd, char *data)
{
	UART1_putStr("AT+");
	UART1_putStr(cmd);
	UART1_putStr("=");
	UART1_putStr(data);
	UART1_putStr("\r\n");
}

void esp8266_sendAsk(char *cmd)
{
	UART1_putStr("AT+");
	UART1_putStr(cmd);
	UART1_putStr("?");
	UART1_putStr("\r\n");
}

uint8_t esp8266_checkResponse(uint16_t strStart)
{
	if(strstr((const char*)(UART1_getRxData(strStart)), "OK"))
		return ESP8266_RESPONSE_OK;
	
	if(strstr((const char*)(UART1_getRxData(strStart)), "ERROR"))
		return ESP8266_RESPONSE_ERROR;
	
	return ESP8266_RESPONSE_UNKNOWN;
}

uint8_t esp8266_checkResponseAfter(uint16_t strStart, char *after)
{
	char *afterPos= strstr((const char*)(UART1_getRxData(strStart)), after);
	
	if(after)
	{
		if(strstr((const char*)(UART1_getRxData(strStart)), "OK"))
			return ESP8266_RESPONSE_OK;
	
		if(strstr((const char*)(UART1_getRxData(strStart)), "ERROR"))
			return ESP8266_RESPONSE_ERROR;
	}
	
	return ESP8266_RESPONSE_UNKNOWN;
}

uint8_t esp8266_waitForPrompt(uint8_t timeout, uint16_t strStart)
{
	int endClk=CLOCK+timeout+1;
	
	while(!strstr((const char*)UART1_getRxData(strStart), ">") && endClk>CLOCK);
	
	if(endClk>CLOCK)
		return ESP8266_RESPONSE_OK;
	else
		return ESP8266_RESPONSE_ERROR;
}

uint8_t esp8266_waitForResp(uint8_t timeout, uint16_t strStart)
{
	int endClk=CLOCK+timeout+1;
	
	while((esp8266_checkResponse(strStart)==ESP8266_RESPONSE_UNKNOWN) && endClk>CLOCK);
	
	return esp8266_checkResponse(strStart);
}

uint8_t esp8266_waitForRespAfter(uint8_t timeout, uint16_t strStart, char *after)
{
	int endClk= CLOCK+timeout+1;
	
	while((esp8266_checkResponseAfter(strStart, after)==ESP8266_RESPONSE_UNKNOWN) && endClk>CLOCK);

	return esp8266_checkResponseAfter(strStart, after);
}

uint8_t esp8266_waitForSpecResp(char *resp, uint8_t timeout, uint16_t strStart)
{
	int endClk=CLOCK+timeout+1;
	
	while(!strstr((const char*)UART1_getRxData(strStart), resp) && endClk>CLOCK);

	if(endClk>CLOCK)
		return ESP8266_RESPONSE_OK;
	else
		return ESP8266_RESPONSE_ERROR;
}

#endif

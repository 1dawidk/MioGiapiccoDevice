#ifndef _ESP8266_DRV_C
#define _ESP8266_DRV_C

#include "esp8266Drv.h"

void esp8266_sendCmd(char *cmd)
{
	UART1_flushRx();
	UART1_putStr("AT+");
	UART1_putStr(cmd);
	UART1_putStr("\r\n");
}

void esp8266_sendData(char *cmd, char *data)
{
	UART1_flushRx();
	UART1_putStr("AT+");
	UART1_putStr(cmd);
	UART1_putStr("=");
	UART1_putStr(data);
	UART1_putStr("\r\n");
}

void esp8266_sendDataNoClear(char *cmd, char *data)
{
	UART1_putStr("AT+");
	UART1_putStr(cmd);
	UART1_putStr("=");
	UART1_putStr(data);
	UART1_putStr("\r\n");
}

void esp8266_sendAsk(char *cmd)
{
	UART1_flushRx();
	UART1_putStr("AT+");
	UART1_putStr(cmd);
	UART1_putStr("?");
	UART1_putStr("\r\n");
}

uint8_t esp8266_checkResponse(void)
{
	if(strstr((const char*)_UART1_RxLine, "OK"))
		return ESP8266_RESPONSE_OK;
	
	if(strstr((const char*)_UART1_RxLine, "ERROR"))
		return ESP8266_RESPONSE_ERROR;
	
	return ESP8266_RESPONSE_UNKNOWN;
}

uint8_t esp8266_waitForPrompt(uint8_t timeout)
{
	int endClk=CLOCK+timeout+1;
	
	while(!strstr((const char*)_UART1_RxLine, ">") && endClk>CLOCK);
	
	if(endClk>CLOCK)
		return ESP8266_RESPONSE_OK;
	else
		return ESP8266_RESPONSE_ERROR;
}

uint8_t esp8266_waitForResp(uint8_t timeout)
{
	int endClk=CLOCK+timeout+1;
	
	while((esp8266_checkResponse()==ESP8266_RESPONSE_UNKNOWN) && endClk>CLOCK);
	
	return esp8266_checkResponse();
}

uint8_t esp8266_waitForSpecResp(char *resp, uint8_t timeout)
{
	int endClk=CLOCK+timeout+1;
	
	while(!strstr((const char*)_UART1_RxLine, resp) && endClk>CLOCK);
	
	if(endClk>CLOCK)
		return ESP8266_RESPONSE_OK;
	else
		return ESP8266_RESPONSE_ERROR;
}

#endif
#ifndef _SERVER_DRIVER_C
#define _SERVER_DRIVER_C

#include "ServerDriver.h"

volatile char *serverResp;
Client_dt client;

#define SERVER_HEADER_OK 				"HTTP/1.1 200 OK\r\nConnection:close\r\n\r\n"
#define SERVER_HEADER_NOTFOUND 	"HTTP/1.1 404 Not Found\r\nConnection:close\r\n\r\n"
#define SERVER_REQ_HEAD 				" HTTP/1.1"

extern volatile uint32_t seconds;
static uint16_t rxListener_readStart=0;

uint8_t Server_SendData(uint8_t connId, char *data)
{
	char dataPreStr[10];
	char timeStr[10];
	uint32_t readStart;
	
	sprintf(timeStr, "%d", seconds);
	sprintf(dataPreStr, "%d,%d", connId, strlen(data)+strlen(SERVER_HEADER_OK));
	
	readStart=UART1_RxCurrent;
	esp8266_sendData(ESP8266_CMD_SEND, dataPreStr);
	if(esp8266_waitForPrompt(1, readStart)!=ESP8266_RESPONSE_OK)
		return SERVER_RESPONSE_ERROR;
	UART1_putStr(SERVER_HEADER_OK);
	UART1_putStr(data);
	
	esp8266_waitForSpecResp("SEND OK", 4, readStart);
	
	return SERVER_RESPONSE_OK;
}

char* Server_GetPOSTVars(const char *httpHeader)
{
	char *start;
	int content_length=0;
	char *result;
	
	start= strstr(httpHeader, "Content-Length: ");
	sscanf(start, "Content-Length: %d", &content_length);
	start= strstr(httpHeader, "\r\n\r\nMethod");
	
	if(content_length && start)
	{
		start+=4;
		result= malloc(sizeof(char)*content_length+1);
		strncpy(result, start, content_length);
		result[content_length]='\0';
		
		return result;
	}
	
	return 0;
}

char* Server_getPOSTVar(char *varName, char* varsString)
{
	char *start= strstr(varsString, varName);
	char *stop= strstr(start, "&");
	char *value;
	uint8_t len;
	
	if(start)
	{
		if(stop)
		{
			len= stop-start-1-strlen(varName);
		}
		else
		{
			len= strlen(varsString)-(start-varsString)-1-strlen(varName);
		}
		
		value= malloc(sizeof(char)*len+1);
		strncpy(value, start+strlen(varName)+1, len);
		value[len]='\0';
		
		return value;
	}
	
	return 0;
}

void Server_StartRxListener(char *newServerResp)
{
	serverResp= newServerResp;
	
	client.open=FALSE;
	client.handled=FALSE;
	
	 rxListener_readStart=UART1_RxCurrent;
}

char* Server_RxListen(void)
{
	char idStr[3];
	uint8_t res;
	char *method;
	
	client.configString=0;
	
	if(strstr((const char*)UART1_getRxData(rxListener_readStart), "+IPD"))
	{
		sscanf((const char*)UART1_getRxData(rxListener_readStart), "+IPD,%hhu,", &client.id);
		client.handled=FALSE;
		client.open=TRUE;	
	}
	
	if(client.open)
	{
		if(strstr((const char*)UART1_getRxData(rxListener_readStart), "\r\n\r\nMethod"))
		{
			_delay_ms(20);
			client.postVars=Server_GetPOSTVars((const char*)UART1_getRxData(rxListener_readStart));
			client.open=FALSE;
			rxListener_readStart=UART1_RxCurrent;
		}
	}
	else if(!client.handled)
	{
		method= Server_getPOSTVar("Method", client.postVars);
		res=strcmp(method, "Talk");
			if(!res)
			{
				Server_SendData(client.id, (char *)serverResp);
			}
			else
			{
				res=strcmp(method, "Config");
				if(!res)
				{
					Server_SendData(client.id, "{\"Answ\": \"OK!\"}");
					client.configString=1;
				}
			}
			
			sprintf(idStr, "%d", client.id);
			esp8266_sendData(ESP8266_CMD_CLOSE, idStr);
			client.handled=TRUE;
			
			if(client.configString)
			{
				return client.postVars;
			}
			
			free(client.postVars);
	}
		
		return 0;
}

uint8_t Server_ConnectTo(char *URL)
{
	char cipStartData[30];
	uint16_t readStart;
	
	sprintf(cipStartData,"\"TCP\",\"%s\",80", URL);
	readStart= UART1_RxCurrent;
	esp8266_sendData(ESP8266_CMD_CIPSTART, cipStartData);
	esp8266_waitForSpecResp("CONNECT", 4, readStart);
	esp8266_waitForResp(4, readStart);
	
	return 0;
}

void Server_putHttpRequestParams(HttpHeaderParam *httpReqParams, uint8_t httpParamsLen)
{
	uint8_t i;

	for(i=0; i<httpParamsLen; i++)
	{
		switch(httpReqParams[i].name)
		{
			case Host:
				UART1_putStr("Host: ");
				break;
			case Accept:
				UART1_putStr("Accept: ");
				break;
			case ContentLength:
				UART1_putStr("Content-Length: ");
				break;
			case ContentType:
				UART1_putStr("Content-Type: ");
				break;
		}
		
		UART1_putStr(httpReqParams[i].value);
		UART1_putStr("\r\n");
	}
	
	UART1_putStr("\r\n");
}

uint16_t Server_getRequestLength(HttpHeaderParam *httpReqParams, uint8_t paramsLen)
{
	uint16_t len=0;
	uint8_t cnt;
	
	for(cnt=0; cnt<paramsLen; cnt++)
	{
		switch(httpReqParams[cnt].name)
		{
			case Host:
				len+= strlen("Host: ");
				break;
			case Accept:
				len+= strlen("Accept: ");
				break;
			case ContentLength:
				len+= strlen("Content-Length: ");
				break;
			case ContentType:
				len+= strlen("Content-Type: ");
				break;
		}
		
		len+= strlen(httpReqParams[cnt].value);
		len+= strlen("\r\n");
	}
	
	len+= strlen("\r\n");
	
	
	return len;
}

void Server_Request(HttpMethod httpMethod, char *path, HttpVar_dt *httpVars, uint8_t varLen, HttpHeaderParam *httpReqParams, uint8_t paramsLen)
{
	uint16_t requestLen=0;
	uint16_t dataLen=0;
	char sendLenText[5];
	char dataLenText[5];
	uint8_t cnt;
	uint16_t readStart;
	
	
	/*Zliczenie dlugosci danych*/
	for(cnt=0; cnt<varLen; cnt++)
	{
		dataLen+=strlen(httpVars[cnt].name);
		dataLen+=strlen(httpVars[cnt].value);
		dataLen+=2;
	}
	dataLen--;
	
	sprintf(dataLenText,"%d",dataLen);
	
	/*UzupeBnienie pola dBugosci http request*/
	for(cnt=0; cnt<paramsLen; cnt++)
	{
		if(httpReqParams[cnt].name==ContentLength)
		{
			httpReqParams[cnt].value=dataLenText;			
			break;
		}
	}
	
	
	/*Zliczanie dBugo[ci naglowka wiadomosci*/
	if(httpMethod==POST)
		requestLen=strlen("POST ");
	else
		requestLen=strlen("GET ");

	requestLen+=strlen(path);
	requestLen+=strlen(SERVER_REQ_HEAD);
	requestLen+=strlen("\r\n");
	requestLen+=Server_getRequestLength(httpReqParams, paramsLen);	
	
	
	sprintf(sendLenText, "%d", requestLen+dataLen);
	readStart= UART1_RxCurrent;
	esp8266_sendData(ESP8266_CMD_SEND, sendLenText);
	esp8266_waitForPrompt(2, readStart);
	
	/*Wyslanie http request*/
	if(httpMethod==POST)
		UART1_putStr("POST ");
	else
		UART1_putStr("GET ");
	
	UART1_putStr(path);
	UART1_putStr(SERVER_REQ_HEAD);
	UART1_putStr("\r\n");
	
	Server_putHttpRequestParams(httpReqParams, paramsLen);
	
	for(cnt=0; cnt<varLen; cnt++)
	{
		UART1_putStr(httpVars[cnt].name);
		UART1_putStr("=");
		UART1_putStr(httpVars[cnt].value);
		if(cnt<varLen-1)
			UART1_putStr("&");
	}
}

uint8_t Server_checkForResponse(char *responseBuff)
{
	char* httpStart=strstr(responseBuff, "200 OK");
	char* httpEnd=strstr(responseBuff, "CLOSED");
	
	if(httpStart && httpEnd && httpEnd>httpStart)
	{
		return 1;
	}
	
	return 0;
}

char* Server_GetBody(char *httpHeader)
{
	char *start;
	int content_length=0;
	char *result;
	
	start= strstr(httpHeader, "Content-Type: text/html\r\n\r\n");
	sscanf(start, "Content-Type: text/html\r\n\r\n%x", &content_length);
	
	if(content_length && start)
	{
		start+=strlen("Content-Type: text/html\r\n\r\n")+4;
		result= malloc(sizeof(char)*content_length+4);
		strncpy(result, start, content_length);
		result[content_length]='\0';
		
		return result;
	}
	
	return 0;
}

#endif

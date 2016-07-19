#ifndef _SERVER_DRIVER_C
#define _SERVER_DRIVER_C

#include "ServerDriver.h"

volatile char *serverResp;
Client_dt client;

#define SERVER_HEADER_OK "HTTP/1.1 200 OK\r\nConnection:close\r\n\r\n"
#define SERVER_HEADER_NOTFOUND "HTTP/1.1 404 Not Found\r\nConnection:close\r\n\r\n"
#define SERVER_REQ_HEAD 			" HTTP/1.1"

extern volatile uint32_t seconds;

uint8_t Server_SendData(uint8_t connId, char *data)
{
	char dataPreStr[10];
	char timeStr[10];
	
	sprintf(timeStr, "%d", seconds);
	sprintf(dataPreStr, "%d,%d", connId, strlen(data)+strlen(SERVER_HEADER_OK));
	
	esp8266_sendData(ESP8266_CMD_SEND, dataPreStr);
	if(esp8266_waitForPrompt(1)!=ESP8266_RESPONSE_OK)
		return SERVER_RESPONSE_ERROR;
	UART1_putStr(SERVER_HEADER_OK);
	UART1_putStr(data);
	
	return SERVER_RESPONSE_OK;
}

char* String_get(char *from, char *to, char *of)
{
	char *start= strstr(of, from);
	char *stop= strstr(of, to);
	uint8_t len;
	
	if(start && stop && (start<stop))
	{
		len
	}
}

char* Server_GetPOSTVar(char *name)
{
	char *inStrPos= strstr((const char*)_UART1_RxLine, name);
	uint8_t len= strlen(name);
	uint8_t i;
	
	char *result;
	
	if(inStrPos)
	{
		inStrPos+=len+1;
		i=0;
		
		while(inStrPos[i]!='&' && inStrPos[i]!='\r')
			i++;
		
		i++;
		
		result= (char *)malloc(sizeof(char)*i);
		
		strncpy(result, inStrPos, i-1);
		result[i-1]=0;
	}
}

char *Server_GetPOSTString(void)
{
	
}

void Server_StartRxListener(char *newServerResp)
{
	UART1_flushRx();
	serverResp= newServerResp;
}

uint8_t Server_RxListen(void)
{
	char idStr[3];
	char *postValue;
	char *pos;
	
	if(strstr((const char*)_UART1_RxLine, "+IPD"))
	{
		sscanf((const char*)_UART1_RxLine, "+IPD,%d,", &client.id);
		client.handled=FALSE;
		client.open=TRUE;
	}
	
	if(client.open)
	{
		pos= strstr((const char*)_UART1_RxLine, "CLOSE");
		if(pos)
		{
			UART1_lock();
			client.postVars= (char *)malloc(sizeof(char)*100);
			strncpy(client.postVars, 
			client.open=FALSE;
		}
	}
	else if(!client.handled)
	{
		if(!strcmp(postValue, "Talk"))
		{
			Server_SendData(client.id, (char *)serverResp);
			sprintf(idStr, "%d", client.id);
			esp8266_sendData(ESP8266_CMD_CLOSE, idStr);
			client.handled=TRUE;
			UART1_unlock();
		}
		else
		{
			client.handled=TRUE;
		}
		
		free(client.postVars);
	}
		
		return 0;
}

uint8_t Server_ConnectTo(char *URL)
{
	char cipStartData[30];
	
	sprintf(cipStartData,"\"TCP\",\"%s\",80", URL);
	
	esp8266_sendData(ESP8266_CMD_CIPSTART, cipStartData);
	esp8266_waitForResp(2);
	
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
	
	/*Zliczenie dlugosci danych*/
	for(cnt=0; cnt<varLen; cnt++)
	{
		dataLen+=strlen(httpVars[cnt].name);
		dataLen+=strlen(httpVars[cnt].value);
		dataLen+=2;
	}
	dataLen++;
	
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
	esp8266_sendData(ESP8266_CMD_SEND, sendLenText);
	esp8266_waitForPrompt(2);
	
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
		else
			UART1_putStr("\r\n");
	}
}

uint8_t Server_checkForResponse(char *responseBuff)
{
	char *dataP;
	
	if(strstr((const char*)_UART1_RxLine, "200 OK"))
	{
		dataP=strstr((const char*)_UART1_RxLine, "\r\n\r\n");
		if(dataP)
		{
			strcpy(responseBuff, dataP+4);
			return 1;
		}
	}
	
	return 0;
}

#endif

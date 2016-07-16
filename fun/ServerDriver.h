#ifndef _SERVER_DRIVER_H
#define _SERVER_DRIVER_H

#define SERVER_RESPONSE_OK			0
#define SERVER_RESPONSE_ERROR		1

#include "stm32f10x.h"
#include "esp8266Drv.h"

typedef struct{
	char *name;
	char *value;
} HttpVar_dt;

typedef struct{
	uint8_t id;
	uint8_t handled;
} Client_dt;

typedef enum {Host, Accept, ContentLength, ContentType } HttpHeaderPamars;

typedef struct{
	HttpHeaderPamars name;
	char *value;
} HttpHeaderParam;

typedef enum {POST, GET} HttpMethod;

uint8_t Server_SendData(uint8_t connId, char *data);
void Server_StartRxListener(char *newServerResp);
uint8_t Server_RxListen(void);
uint8_t Server_ConnectTo(char *URL);
void Server_Request(HttpMethod httpMethod, char *path, HttpVar_dt *httpVars, uint8_t varLen, HttpHeaderParam *httpReqParams, uint8_t paramsLen);
uint8_t Server_checkForResponse(char *responseBuff);


#endif

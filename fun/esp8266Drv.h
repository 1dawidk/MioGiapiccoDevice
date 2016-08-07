#ifndef _ESP8266_DRV_H
#define _ESP8266_DRV_H

#include "stm32f10x.h"
#include "../def/pin_descr.h"
#include "UART/UartDrv.h"
#include "string.h"
#include "def/def.h"
#include "coreinit.h"

#define ESP8266_TURN_ON
#define ESP8266_TURN_OFF

#define ESP8266_STATE_OK						0
#define ESP8266_STATE_INIT_ERROR		1
#define ESP8266_STATE_ERROR					2

#define ESP8266_ENMODE_NONE					0
#define ESP8266_ENMODE_WEP					1
#define ESP8266_ENMODE_WAP_PSK			2
#define ESP8266_ENMODE_WAP2_PSK			3
#define ESP8266_ENMODE_WAP_WAP2_PSK	4

#define ESP8266_CWMODE_STA					"1"
#define ESP8266_CWMODE_AP						"2"
#define ESP8266_CWMODE_BB						"3"

#define ESP8266_MUXMODE_SINGLE			"0"
#define ESP8266_MUXMODE_MULTI				"1"

#define ESP8266_SERVER_START				"1"
#define ESP8266_SERVER_STOP					"0"

#define ESP8266_CMD_MODE						"CWMODE"
#define ESP8266_CMD_RST							"RST"
#define ESP8266_CMD_JAP							"CWJAP"
#define ESP8266_CMD_LAP							"CWLAP"
#define ESP8266_CMD_SAP							"CWSAP"
#define ESP8266_CMD_MUX							"CIPMUX"
#define ESP8266_CMD_UPDATE					"CIUPDATE"
#define ESP8266_CMD_CIPSTART				"CIPSTART"
#define ESP8266_CMD_SERVER					"CIPSERVER"
#define ESP8266_CMD_STO							"CIPSTO"
#define ESP8266_CMD_SEND						"CIPSEND"
#define ESP8266_CMD_CLOSE						"CIPCLOSE"

#define ESP8266_RESPONSE_OK					0
#define ESP8266_RESPONSE_ERROR			1
#define ESP8266_RESPONSE_UNKNOWN		2
#define ESP8266_RESPONSE_BUSY				3

void esp8266_sendCmd(char *cmd);
void esp8266_sendData(char *cmd, char *data);
void esp8266_sendAsk(char *cmd);

uint8_t esp8266_checkResponse(uint16_t strStart);

uint8_t esp8266_waitForPrompt(uint8_t timeout, uint16_t strStart);
uint8_t esp8266_waitForResp(uint8_t timeout, uint16_t strStart);
uint8_t esp8266_waitForRespAfter(uint8_t timeout, uint16_t strStart, char *after);
uint8_t esp8266_waitForSpecResp(char *resp, uint8_t timeout, uint16_t strStart);

#endif

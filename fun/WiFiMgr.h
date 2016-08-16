#ifndef _WIFI_MGR_H
#define _WIFI_MGR_H

#include "stm32f10x.h"
#include "esp8266Drv.h"

#define WIFI_CONNECTED				1
#define WIFI_DISCONNECTED			0

uint8_t WiFi_InitApMode(char *SSID, char *pass, uint8_t enMode);
uint8_t WiFi_InitClientMode(void);

void WiFi_Sleep(void);
uint8_t WiFi_Wakeup(void);
uint8_t WiFi_IsAsleep(void);

uint8_t WiFi_StartWiFiConnecting(char *SSID, char *pass);
uint8_t WiFi_IsWiFiConnected(void);

#endif

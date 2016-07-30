
#ifndef TYPESDEF_H
#define TYPESDEF_H	

#include "stdint.h"

typedef struct{
	char *name;
	uint32_t id;
	
	uint8_t soil_moisture;
	uint8_t air_humidity;
	uint8_t temperature;
	uint8_t insolation;
}	Plant_dt;

#define SYS_MODE_NORMAL			0
#define SYS_MODE_CONFIG			1
#define SYS_MODE_FIRSTRUN		2

typedef struct{
	uint8_t mode;
	uint8_t plantsCnt;
	char *user_login;
	char *user_pass;
	char *WiFi_ssid;
	char *WiFi_pass;
} SystemConfig_dt;

typedef struct{
	uint32_t lastTime;
	uint32_t talkStart;
	uint8_t talking;
	uint8_t dataPrepared;
	uint8_t broken;
} Measure;

typedef struct{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	
	uint8_t day;
	uint8_t month;
	uint16_t year;
	
	uint32_t systemUs;
	uint32_t sustemS;
} Timestamp;

typedef enum{
	OK,
	NoWiFi,
	SensorError
} SystemState;

#endif


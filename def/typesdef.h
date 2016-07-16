
#ifndef TYPESDEF_H
#define TYPESDEF_H	

#include "stdint.h"

typedef struct{
	char name[20];
	uint32_t id;
	
	uint8_t soil_moisture;
	uint8_t air_humidity;
	uint8_t temperature;
	uint8_t insolation;
	uint8_t wattering;
}	Plant_dt;

typedef struct{
	uint8_t mode;
	uint8_t plantsCnt;
	char *userLogin;
	char *userPass;
	char *WiFi_ssid;
	char *WiFi_pass;
} SystemConfig_dt;

typedef struct{
	uint32_t lastTime;
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


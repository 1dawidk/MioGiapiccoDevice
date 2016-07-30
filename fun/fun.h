#ifndef FUN_H_
#define FUN_H_

#include "stm32f10x.h"
#include "ADCHighDrv.h"
#include "def/def.h"
#include "def/pin_descr.h"
#include "coreInit.h"
#include "DMADrv.h"
#include "TimerHighDrv.h"
#include "def/typesdef.h"
#include "UART/UartDrv.h"
#include "esp8266Drv.h"
#include "ServerDriver.h"
#include "WiFiMgr.h"
#include "I2C/I2cDrv.h"
#include "fun/eeprom.h"
#include "fun/JSON.h"

uint8_t RegisterToServer(HttpHeaderParam *httpReqParams, SystemConfig_dt *sysCfg, Plant_dt *plants);
uint8_t StartSys(SystemConfig_dt *sysCfg, Plant_dt *plants);
void DHT11Init(void);

uint8_t getSoilMoisture(void);
uint8_t getDHTData(uint8_t *temp, uint8_t *RH);
uint8_t getInsolation(void);

void setEngineRPM(uint16_t rpm);

uint8_t receivePlantData(Plant_dt *plant, char *userLogin, char *userPass);
uint8_t sendPlantData(Plant_dt *plant, char *userLogin, char *userPass);

#endif

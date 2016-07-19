#ifndef _DEF_H
#define _DEF_H

#include "high_GPIODrv.h"

//################################### CPU_SET
#define STM32F103RB
#define F_CPU											72000000UL
#define __NAME__            			"stm32f103Cb"

#define NULL 											(uint8_t *)0
//*******************************************

//############################## PLANT SYSTEM
#define WIFI_MODE_AP							0
#define WIFI_MODE_CLIENT					1

#define SYS_MODE_NORNMAL					0
#define SYS_MODE_CONFIG						1

#define FALSE											0
#define TRUE											1

#define SERVER_SEND_PATH 					"/setPlantDataScript.php"
#define SERVER_READ_PATH 					"/getPlantDataScript.php"
#define SERVER_CFG_PATH						"/plantsConfigScript.php"
//*******************************************

//##################################### LEDS
#define FUN_LED_ON 								SET_SEG(LED_SEG) &= ~FUN_LED_PIN
#define FUN_LED_OFF 							SET_SEG(LED_SEG) |= FUN_LED_PIN
#define FUN_LED_TOG 							SET_SEG(LED_SEG) ^= FUN_LED_PIN

#define SYS_LED_ON 								SET_SEG(LED_SEG) &= ~SYS_LED_PIN
#define SYS_LED_OFF 							SET_SEG(LED_SEG) |= SYS_LED_PIN
#define SYS_LED_TOG 							SET_SEG(LED_SEG) ^= SYS_LED_PIN

#define ERROR_LED_ON 							SET_SEG(LED_SEG) &= ~ERROR_LED_PIN
#define ERROR_LED_OFF 						SET_SEG(LED_SEG) |= ERROR_LED_PIN
#define ERROR_LED_TOG 						SET_SEG(LED_SEG) ^= ERROR_LED_PIN

#define WIFI_LED_ON 							SET_SEG(LED_SEG) &= ~WIFI_LED_PIN
#define WIFI_LED_OFF 							SET_SEG(LED_SEG) |= WIFI_LED_PIN
#define WIFI_LED_TOG 							SET_SEG(LED_SEG) ^= WIFI_LED_PIN
//*******************************************

//############################## WIFI MODULE 
#define WIFI_RST_H								SET_SEG(WIFI_PWR_SEG) |= WIFI_RST_PIN
#define WIFI_RST_L								SET_SEG(WIFI_PWR_SEG) &= ~WIFI_RST_PIN
#define WIFI_POWER_ON							SET_SEG(WIFI_PWR_SEG) |= WIFI_CHPD_PIN
#define WIFI_POWER_OFF						SET_SEG(WIFI_PWR_SEG) &= ~WIFI_CHPD_PIN

#define WIFI_GPIO0_H							SET_SEG(WIFI_GPIO_SEG) |= WIFI_GPIO0_PIN
#define WIFI_GPIO0_L							SET_SEG(WIFI_GPIO_SEG) &= ~WIFI_GPIO0_PIN
#define WIFI_GPIO2_H							SET_SEG(WIFI_GPIO_SEG) |= WIFI_GPIO2_PIN
#define WIFI_GPIO2_L							SET_SEG(WIFI_GPIO_SEG) &= ~WIFI_GPIO2_PIN
//*******************************************

//############################## DH11
#define DHT11_ON									SET_SEG(DH11_SEG) &= ~DH11_ONOFF_PIN
#define DHT11_OFF									SET_SEG(DH11_SEG) |= DH11_ONOFF_PIN

#define DHT11_DATA0_IN						GPIOA->CRH= (GPIOA->CRH&(~(0xF<<P11s*4))) | (8<<P11s*4)
#define DHT11_DATA0_OUT						GPIOA->CRH=(GPIOA->CRH&(~(0xF<<P11s*4))) | (3<<P11s*4)
#define DHT11_DATA0_H							SET_SEG(DH11_SEG) |= DH11_DATA0_PIN
#define DHT11_DATA0_L							SET_SEG(DH11_SEG) &= ~DH11_DATA0_PIN
#define DHT11_DATA0_READ					(READ_SEG(DH11_SEG)&DH11_DATA0_PIN)
//*******************************************

//################################### BUTTONS
#define WIFIMODE_BUTTON_STATE			!(READ_SEG(BUTTONS_SEG)&WIFIMODE_BUTTON_PIN)
#define FUNCTION_BUTTON_STATE			!(READ_SEG(BUTTONS_SEG)&FUNCTION_BUTTON_PIN)
//*******************************************

//##################################### POMIARY
#define MAX_SOILMOISTURE					2750
#define MAX_AIRHUMIDITY						0x0850
#define MAX_INSOLATION						225;

#define SOILMOISTURE_DEVICE_ON		SET_SEG(SOILHYGROMETER_ONOFF_SEG)&=~SOILHYGROMETER_ONOFF_PIN
#define SOILMOISTURE_DEVICE_OFF		SET_SEG(SOILHYGROMETER_ONOFF_SEG)|=SOILHYGROMETER_ONOFF_PIN
//*******************************************

//##################################### CZASY
#define CONNECTION_SEP						300
#define MEASUREMENT_SEP						30
//*******************************************

//##################################### USART
#define UART1_USE									1
#define UART1_USE_DMA							1
#define USE_DMA_USART1						1
#define UART1_RX_LINE_SIZE				1024
#define UART1_TX_LINE_SIZE				32
//*******************************************

//############## DEFINICJE ZEGARA SYSTEMOWEGO
#define CLOCK 										(((uint32_t)RTC->CNTH<<16) | RTC->CNTL) 
#define TIMEfrom(x)								(CLOCK-x)
//*******************************************

//##################### ADRESY PAMIECI EEPROM
#define EE_ADR_FIRSTRUN						0
#define EE_ADR_PLANTSCNT					1

#define EE_ADR_SSID								0
#define EE_ADR_PASS								1
#define EE_ADR_USER_LOGIN					2
#define EE_ADR_USER_PASS					3
#define EE_ADR_PLANTBASE_NAME			4
//*******************************************

#endif

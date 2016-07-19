#ifndef _PIN_DESCR_H
#define _PIN_DESCR_H

/*####################################### LED */
#define LED_SEG          					B

#define WIFI_LED_PIN							P12
#define SYS_LED_PIN								P14
#define FUN_LED_PIN								P13
#define ERROR_LED_PIN							P15
/*********************************************/

#define BUTTONS_SEG								A
#define WIFIMODE_BUTTON_PIN				P8

#define ADC_SEG										A
#define ADC_1_PIN									P1
#define ADC_2_PIN									P2
#define ADC_3_PIN									P4
#define ADC_4_PIN									P3
#define ADC_5_PIN									P5
#define ADC_6_PIN									P6

#define SOILHYGROMETER_ONOFF_SEG	C
#define SOILHYGROMETER_ONOFF_PIN	P3

#define ENGINE_SEG          			A
#define ENGINE_PIN         				P3 

/*####################################### WIFI MODULE */
#define USART1_SEG								B
#define USART1_TX_PIN							P6
#define USART1_RX_PIN							P7


#define WIFI_PWR_SEG							B
#define WIFI_RST_PIN							P1
#define WIFI_CHPD_PIN							P0

#define WIFI_GPIO_SEG							C
#define WIFI_GPIO0_PIN						P5
#define WIFI_GPIO2_PIN						P4
/*********************************************/

/*################################# EEPROM MODULE */
#define I2C_SEG										B					


#define I2C_SDA_PIN								P11
#define I2C_SCL_PIN								P10
/*********************************************/

/*####################################### DH11 */
#define DH11_SEG									A

#define DH11_ONOFF_PIN						P12

#define DH11_DATA0_PIN						P11
/*********************************************/

#endif

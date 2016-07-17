#ifndef _PIN_DESCR_H
#define _PIN_DESCR_H

/*####################################### LED */
#define LED_SEG          					B

#define WIFI_LED_PIN							P12
#define SYS_LED_PIN								P13
#define FUN_LED_PIN								P14
#define ERROR_LED_PIN							P15
/*********************************************/

#define BUTTONS_SEG								C
#define WIFIMODE_BUTTON_PIN				P1
#define	SECOND_BUTTON_PIN					P0

#define ADC_SEG										A
#define ADC_1_PIN									P1
#define ADC_2_PIN									P2
#define ADC_3_PIN									P4
#define ADC_4_PIN									P3

#define SOILHYGROMETER_ONOFF_SEG	C
#define SOILHYGROMETER_ONOFF_PIN	P3

#define ENGINE_SEG          			A
#define ENGINE_PIN         				P3 

/*####################################### WIFI MODULE */
#define USART1_SEG								B
#define USART1_TX_PIN							P6
#define USART1_RX_PIN							P7


#define WIFI_CTRL_SEG							B
#define WIFI_RST_PIN							P11
#define WIFI_CHPD_PIN							P10
#define WIFI_GPIO0_PIN						P2
#define WIFI_GPIO2_PIN						P0
/*********************************************/

/*################################# EEPROM MODULE */
#define I2C_SEG										


#define I2C_SDA_PIN
#define I2C_SCL_PIN
/*********************************************/

/*####################################### DH11 */
#define DH11_SEG									A

#define DH11_ONOFF_PIN						P12

#define DH11_DATA0_PIN						P11
#define DH11_DATA1_PIN						P10
#define DH11_DATA2_PIN						P9
#define DH11_DATA3_PIN						P8
/*********************************************/

#endif

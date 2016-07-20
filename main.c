#include "main.h"

int main(void)
{
	//System variables
	SystemConfig_dt sysCfg;
	uint8_t dhtStateCode;
	Plant_dt plant[4];
	
	//Int to text buff
	char soilMoistureText[5]="";
	char airHumidityText[5]="";
	char temperatureText[5]="";
	char insolationText[5]="";
	
	//Http connection variablels
	HttpHeaderParam httpReqParams[4];
	HttpVar_dt httpVars[8];
	
	//Time variables
	Measure measure;
	
	char *configStr;
	char *tmpStr;
	JSON serverJSONResp;
	
	//System init	
	StartSys(&sysCfg);
	
	measure.talking=0;
	measure.dataPrepared=0;
	measure.broken=0;
	
	FUN_LED_OFF;
	SYS_LED_ON;
	_delay_ms(100);	
	FUN_LED_OFF;
	SYS_LED_OFF;
	
	/*HTTP VARIABLES*/
	/*Login*/
	httpVars[0].name="Login";
	httpVars[0].value=sysCfg.user_login;
	
	httpVars[1].name="Pass";
	httpVars[1].value=sysCfg.user_pass;
	
	httpVars[2].name="PlantId";
	httpVars[2].value="5";
	
	httpVars[3].name="Watering";
	httpVars[3].value="0";
	httpVars[4].name="SoilMoisture";
	httpVars[4].value=soilMoistureText;
	httpVars[5].name="AirHumidity";
	httpVars[5].value=airHumidityText;
	httpVars[6].name="Temperature";
	httpVars[6].value=temperatureText;
	httpVars[7].name="Insolation";
	httpVars[7].value=insolationText;
	
	/*HTTP REQUEST PARAMS*/
	httpReqParams[0].name=Host;
	httpReqParams[0].value="dawidkulpa.pl";
	
	httpReqParams[1].name=Accept;
	httpReqParams[1].value="*/*";
	
	httpReqParams[2].name=ContentLength;
	httpReqParams[2].value="";
	
	httpReqParams[3].name=ContentType;
	httpReqParams[3].value="application/x-www-form-urlencoded";	

	//Main inf loop
	while(1)
	{		
		/* NORMAL MODE LOOP */
		if(sysCfg.mode==SYS_MODE_NORMAL)
		{
			if(measure.broken)
			{
				ERROR_LED_ON;
				FUN_LED_ON;
				DHT11Init();
				measure.broken=0;
				FUN_LED_OFF;
				ERROR_LED_OFF;
			}
			if((CLOCK-measure.lastTime)>MEASUREMENT_SEP)
			{
				if(WiFi_IsWiFiConnected()==WIFI_DISCONNECTED)
				{
					WIFI_LED_OFF;
					WiFi_StartWiFiConnecting(sysCfg.WiFi_ssid, sysCfg.WiFi_pass);
				}
				else
				{
					WIFI_LED_ON;
					
					if(measure.dataPrepared)
					{
						if(measure.talking==1)
						{
							strcpy((char *)rxLocker, (char *)_UART1_RxLine);
							//If server resp clear data prepared and talking
							if(Server_checkForResponse((char *)rxLocker))
							{
								measure.talking=0;
								measure.dataPrepared=0;
								//Set update clock
								measure.lastTime=CLOCK;
								//serverJSONResp= JSON_FromString((char *)rxLocker);
							}
						}
						else
						{
							Server_ConnectTo("www.dawidkulpa.pl");
							measure.talking=1;
							Server_Request(POST, SERVER_SEND_PATH, httpVars, 8, httpReqParams, 4);
						}
					}
					else
					{
						FUN_LED_ON;
		
						//Get and set soil moisture data
						plant[0].soil_moisture= getSoilMoisture();
						sprintf(soilMoistureText, "%d", plant[0].soil_moisture);
						
						//Get and set air humidity and temperature data
						dhtStateCode=getDHTData(&(plant[0].temperature), &(plant[0].air_humidity));
						
					
						if(dhtStateCode)
						{
							//Set -1 for invalid data
							measure.broken=1;
							sprintf(airHumidityText, "-1");
							sprintf(temperatureText, "-1");
						}
						else
						{
							//Set valid data
							sprintf(airHumidityText, "%d", plant[0].air_humidity);
							sprintf(temperatureText, "%d", plant[0].temperature);
						}
						
						//Get insolation
							sprintf(insolationText, "%d", getInsolation());
						
						//Check if watering
						if(plant[0].soil_moisture<40)
							httpVars[3].value="1";
						else
							httpVars[3].value="0";
						
						//Set data prepared
						if(!measure.broken)
						{
							measure.dataPrepared=1;
						}
						
						FUN_LED_OFF;
					}
				}
			}
		}
		
		/* CONFIG MODE LOOP */
		else if(sysCfg.mode==SYS_MODE_CONFIG)
		{
			if(configStr=Server_RxListen())
			{
				tmpStr= Server_getPOSTVar("wifiSSID", configStr);
				EEWrite_String(EE_ADR_SSID, tmpStr);
				_delay_ms(20);
				
				free(tmpStr);
				tmpStr= Server_getPOSTVar("wifiPass", configStr);
				EEWrite_String(EE_ADR_PASS, tmpStr);
				_delay_ms(20);
				
				free(tmpStr);
				tmpStr= Server_getPOSTVar("userLogin", configStr);
				EEWrite_String(EE_ADR_USER_LOGIN, tmpStr);
				_delay_ms(20);
				
				free(tmpStr);
				tmpStr= Server_getPOSTVar("userPass", configStr);
				EEWrite_String(EE_ADR_USER_PASS, tmpStr);
				_delay_ms(20);
				
				EEWrite_Byte(EE_ADR_FIRSTRUN, FALSE);
			}
			
			if((CLOCK-measure.lastTime)>0)
			{
				WIFI_LED_TOG;
				FUN_LED_TOG;
				measure.lastTime=CLOCK;
			}
		}	
	}
	
	return 0;
}

#include "main.h"

int main(void)
{
	//System variables
	SystemConfig_dt sysCfg;
	uint8_t dhtStateCode;
	uint8_t configRegister=0;
	uint8_t registerCnt=0;
	uint8_t server_responseCode;
	Plant_dt plant[4];
	
	uint32_t talkStartClk;
	uint16_t readStart;
	
	//Int to text buff
	char soilMoistureText[5]="";
	char airHumidityText[5]="";
	char temperatureText[5]="";
	char insolationText[5]="";
	char idText[5]="";
	
	//Http connection variablels
	HttpHeaderParam httpReqParams[4];
	HttpVar_dt httpVars[8];
	
	//Time variables
	Measure measure;
	
	char *configStr;
	char *tmpStr;
	char *postPlantName;
	uint32_t tmpInt;
	uint8_t i;
	
	//System init	
	StartSys(&sysCfg, plant);
	
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
	httpVars[2].value=idText;
	
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
	
	measure.lastTime=CLOCK-MEASUREMENT_SEP+30;
	
	//Main inf loop
	while(1)
	{		
		/* NORMAL MODE LOOP */
		if(sysCfg.mode==SYS_MODE_NORMAL && configRegister)
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
				if(WiFi_IsAsleep()==TRUE)
				{
					WiFi_Wakeup();
				}
				
				if(!measure.talking && WiFi_IsWiFiConnected()!=WIFI_CONNECTED)
				{
					WIFI_LED_OFF;
					WiFi_StartWiFiConnecting(sysCfg.WiFi_ssid, sysCfg.WiFi_pass);
					_delay_ms(500);
				}
				else
				{
					WIFI_LED_ON;
					
					if(measure.dataPrepared)
					{
						if(measure.talking==1)
						{
							_delay_ms(10);
							server_responseCode= Server_checkForResponse(UART1_getRxData(readStart));
							//If server resp clear data prepared and talking
							if(server_responseCode==1 || CLOCK-talkStartClk>30)
							{
								if(measure.sendCnt<sysCfg.plantsCnt-1)
								{
									measure.sendCnt++;
								}
								else
								{
									measure.dataPrepared=0;
									WiFi_Sleep();
									WIFI_LED_OFF;
									measure.lastTime=CLOCK;
									//serverJSONResp= JSON_FromString((char *)rxLocker);
								}
								
								measure.talking=0;
							}
						}
						else
						{
							sprintf(idText, "%d", plant[measure.sendCnt].id);
							sprintf(soilMoistureText, "%d", plant[measure.sendCnt].soil_moisture);
					
							Server_ConnectTo("www.dawidkulpa.pl");
							measure.talking=1;
							readStart=UART1_RxCurrent;
							Server_Request(POST, SERVER_SEND_PATH, httpVars, 8, httpReqParams, 4);
							talkStartClk=CLOCK;
						}
					}
					else
					{
						FUN_LED_ON;
		
						//Get and set soil moisture data
						for(i=0; i<sysCfg.plantsCnt; i++)
						{
							plant[i].soil_moisture= getSoilMoisture(i);
						}
						
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
						
						//Set data prepared
						if(!measure.broken)
						{
							measure.dataPrepared=1;
							measure.sendCnt=0;
						}
						
						FUN_LED_OFF;
					}
				}
			}
		}
		
		/* REGISTER CONFIG LOOP */
		else if(sysCfg.mode==SYS_MODE_NORMAL && !configRegister)
		{
			registerCnt++;
			_delay_ms(1000);
			FUN_LED_ON;
			if(!RegisterToServer(httpReqParams, &sysCfg, plant))
			{
				configRegister=1;
				WiFi_Sleep();
				WIFI_LED_OFF;
			}
			FUN_LED_OFF;
		}
		
		/* CONFIG MODE LOOP */
		else if(sysCfg.mode==SYS_MODE_CONFIG)
		{
			configStr=Server_RxListen();
			
			if(configStr)
			{
				tmpStr= Server_getPOSTVar("wifiSSID", configStr);
				EEWrite_String(EE_ADR_SSID, tmpStr);
				free(tmpStr);	
				_delay_ms(200);
							
				
				tmpStr= Server_getPOSTVar("wifiPass", configStr);
				EEWrite_String(EE_ADR_PASS, tmpStr);
				free(tmpStr);	
				_delay_ms(200);
				
				
				tmpStr= Server_getPOSTVar("userLogin", configStr);
				EEWrite_String(EE_ADR_USER_LOGIN, tmpStr);
				free(tmpStr);	
				_delay_ms(200);
				
				
				tmpStr= Server_getPOSTVar("userPass", configStr);
				EEWrite_String(EE_ADR_USER_PASS, tmpStr);
				free(tmpStr);	
				_delay_ms(200);
				
				
				tmpStr= Server_getPOSTVar("PlantsCnt", configStr);
				sscanf(tmpStr, "%d", &tmpInt);
				EEWrite_Byte(EE_ADR_PLANTSCNT, tmpInt);
				free(tmpStr);	
				_delay_ms(200);
				
				postPlantName= malloc(sizeof(char)*8);
				
				for(i=0; i<tmpInt; i++)
				{
					sprintf(postPlantName, "Plant%d", i);
					tmpStr= Server_getPOSTVar(postPlantName, configStr);
					EEWrite_String(EE_ADR_PLANTNAME_BASE+i, tmpStr);
					_delay_ms(200);
					free(tmpStr);
				}
				
				free(postPlantName);
				free(configStr);
				
				EEWrite_Byte(EE_ADR_FIRSTRUN, FALSE);
				
				Server_StartRxListener(BuildAPResponse(&sysCfg, plant));
			}
			
			if((CLOCK-measure.lastTime)>0)
			{
				WIFI_LED_TOG;
				FUN_LED_TOG;
				measure.lastTime=CLOCK;
			}
		}	
	}
}

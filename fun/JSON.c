#ifndef _JSON_C
#define _JSON_C

#include "JSON.h"

JSONObj JSON_getFirstJObj(char *str)
{
	char *ps, *pm, *pe;
	uint16_t nameLen, valueLen;
	JSONObj jObj;
	
	ps= strstr(str, "\"");
	pm= strstr(ps+1, "\":\"");
	pe= strstr(pm+3, "\"");
	
	nameLen=pm-ps-1;
	valueLen=pe-pm-3;
	
	jObj.name= (char *)malloc(nameLen*sizeof(char));
	jObj.value= (char *)malloc(valueLen*sizeof(char));
	
	strncpy(jObj.name, ps+1, nameLen);
	strncpy(jObj.value, pm+3, valueLen);
	
	return jObj;
}

/* Konwersja JSON <-> String */
JSON JSON_FromString(char *stringFormat)
{
	JSON json;
	
	uint16_t cnt;
	uint16_t i;
	
	char tmpName[20], tmpValue[20];
	
	i=0;
	cnt++;
	while(stringFormat[i])
	{
		if(stringFormat[i]==':')
			cnt++;
		
		i++;
	}
	
	json.list= (JSONObj *)malloc(cnt*sizeof(JSONObj));
	
	for(i=0; i<cnt-1; i++)
	{
		json.list[i]=JSON_getFirstJObj(stringFormat);
		stringFormat= strstr(stringFormat, ",")+1;
	}
}


char* JSON_ToString(JSON *json)
{
	
}

/* Czytanie list JSON */
char* JSON_getValue(JSON *json, char *name)
{
	uint16_t i;
	
	for(i=0; i<json->len; i++)
	{
		if(json->list[i].name==name)
		{
			return json->list[i].value;
		}
	}
	
	return JSON_GET_ERROR;
}

#endif

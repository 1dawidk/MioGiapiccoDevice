#ifndef _JSON_H
#define _JSON_H

#include "stm32f10x.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define JSON_GET_ERROR "[JSON get error]"

/**
* @brief	Single field in JSON
*/
typedef struct {
	char *name;
	char *value;
} JSONObj;

/**
* @brief	List of JSON fields
*/
typedef struct {
	uint16_t len;
	JSONObj *list;
} JSON;




/* Konwersja JSON <-> String */
JSON JSON_FromString(char *stringFormat);
char* JSON_ToString(JSON *json);

/* Czytanie list JSON */
char* JSON_getValue(JSON *json, char *name);

#endif

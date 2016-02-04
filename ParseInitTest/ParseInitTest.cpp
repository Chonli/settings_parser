// ParseInitTest.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <Windows.h>

#define MAX_LENGTH 50

typedef struct{
	char name[MAX_LENGTH+1];
	char value[MAX_LENGTH+1];
}SettingItem;

typedef struct{
	int nbItem;
	int maxNbItem;
	SettingItem* items;
}SettingData;

SettingData*  initSettingData(int maxData)
{
	SettingData *setD = (SettingData*)malloc(sizeof(SettingData));
	if(setD != NULL){
		setD->nbItem = 0;
		setD->maxNbItem = maxData;
		setD->items = (SettingItem*)malloc((maxData - 1) * sizeof(SettingItem));
		memset(setD->items, 0, (maxData-1) * sizeof(SettingItem));		
	}
	return(setD);
}

int ParseFile(char* path,SettingData* retData)
{
	int dataRead = 0, i =0, j=0;
	FILE* fileH = NULL;
	int maxSize = sizeof(SettingItem)*(retData->maxNbItem-1);
	char* readBuffer = (char*)malloc(maxSize);
	char* ptrReadBuffer;
	char* ptrTmp;
	memset(readBuffer,0,maxSize);


	fileH = fopen(path,"r");
	if(fileH != NULL){
		dataRead = fread(readBuffer,sizeof(char),maxSize,fileH);
		fclose(fileH);
		ptrReadBuffer = readBuffer;	
		ptrTmp = ptrReadBuffer;
		while((i < dataRead) && (retData->nbItem < retData->maxNbItem)){
			if(*ptrReadBuffer != '#' || *ptrReadBuffer != '\0'){
				//read name
				while(*ptrReadBuffer != '=') {++i; ++ptrReadBuffer;}
				*ptrReadBuffer = '\0';
				strcpy(retData->items[retData->nbItem].name,ptrTmp);
				ptrTmp = ++ptrReadBuffer;++i;
				
				//read value
				while(*ptrReadBuffer != '\n') {++i; ++ptrReadBuffer;}	
				*ptrReadBuffer = '\0';
				strcpy(retData->items[retData->nbItem].value, ptrTmp);
				ptrTmp = ++ptrReadBuffer;++i;

				retData->nbItem += 1;
			}else
			{
				while(*ptrReadBuffer != '\n') {++i; ++ptrReadBuffer;}
			}
		}
	}else{
		printf("open file error");
	}
	return 0;
}

char* GetValue(SettingData* data, char* name)
{
	char* value = NULL;

	for(int i=0; i< data->nbItem; ++i)
	{
		if(strcmp(data->items[i].name, name) == 0)
		{
			value = data->items[i].value;
			break;
		}
	}
	return value;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int NBParam = 0;
	SettingData *data = initSettingData(10);

	ParseFile("../toto.txt", data);

	//print read data
	printf("---- data read ----\n");
	for(int i=0; i< data->nbItem; ++i)
	{
		printf("%s=%s\n", data->items[i].name, data->items[i].value);
	}
	printf("--------------------\n");
	printf("toto value = %s",GetValue(data, "toto"));

	return 0;
}


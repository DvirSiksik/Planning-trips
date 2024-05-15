#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <crtdbg.h>
#include "General.h"



char* getStrExactName(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	printf("%s\t", msg);
	myGets(temp, MAX_STR_LEN, stdin);

	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;

	strcpy(theStr, str);

	return theStr;
	
}

char* myGets(char* buffer, int size, FILE* source)
{
	char* ok;
	if (buffer != NULL && size > 0)
	{
		do { //skip only '\n' strings
			ok = fgets(buffer, size, source);
		} while (ok && ((strlen(buffer) <= 1) || (isspace(buffer[0]))));
	
		if (ok)
		{
			char* back = buffer + strlen(buffer);
			//trim end spaces
			while ((buffer < back) && (isspace(*--back)));
			*(back + 1) = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}



void generalArrayFunction(void* arr[], int numOfElements, int sizeOfElement, void (*f)(void*))
{
	for (int i = 0; i < numOfElements; i++)
	{
		f((char*)(arr)+i*sizeOfElement);
	}
}

void generalVariableArrayFunction(void* arr, int numOfElements, int sizeOfElement, void (*f)(void*))
{
	for (int i = 0; i < numOfElements; i++)
	{
		f((char*)(arr)+i * sizeOfElement);
	}
}


int getString(char* buffer, int len)
{
	if (scanf("%s", buffer) > len)
		return 0;
	return 1;
}


void printGeneralEnum(char* msg, const char** eNumArr, int len)
{
	printf(msg);
	printf("\n");
	for (int i = 0; i < len; i++)
		printf("%d.	%s\n", i+1, eNumArr[i]);
}

void printString(const void* vC)
{
	char* c = (char*)vC;
	printf("%s\n", c);
}

void freeString(char* c)
{
	free(c);
}
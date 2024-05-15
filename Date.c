#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "General.h"
#include "Date.h"
#include "fileHelper.h"


const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV '-'

void getCorrectDate(Date* pDate)
{
	char date[MAX_STR_LEN];
	int ok = 1;

	do {
		printf("Enter trip  Date yyyy%cmm%cdd \t",
			SPECIAL_TAV, SPECIAL_TAV);
		myGets(date, MAX_STR_LEN, stdin);
		ok = checkDate(date, pDate);
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}


int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 10)
		return 0;
	if ((date[4] != SPECIAL_TAV) || (date[7] != SPECIAL_TAV))
		return 0;
	sscanf(date, "%d%*c%d%*c%d", &year, &month, &day);
	if (day < 1 || month < 1 || month > 12)
		return 0;

	if (day > DAY_MONTHS[month - 1])
		return 0;

	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}



void printDate(const Date* pDate)
{
	printf("Date: %d-%d-%d\n", pDate->day, pDate->month, pDate->year);
}


int writeDateToBinaryFile(const Date* date, FILE* fp)
{
	if (!writeIntToFile(date->day, fp, "ERROR writing day"))
		return 0;
	if (!writeIntToFile(date->month, fp, "ERROR writing month"))
		return 0;
	if (!writeIntToFile(date->year, fp, "ERROR writing year"))
		return 0;
	return 1;
}

int writeDatesArrToBinaryFile(const Date* date, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!writeDateToBinaryFile(&date[i], fp))
			return 0;
	}
	return 1;
}

int loadDateFromBinaryFile(Date* date, FILE* fp)
{
	if (!readIntFromFile(&date->day, fp, "ERROR reading day"))
		return 0;
	if (!readIntFromFile(&date->month, fp, "ERROR reading month"))
		return 0;
	if (!readIntFromFile(&date->year, fp, "ERROR reading year"))
		return 0;
	return 1;
}

int loadDateArrToBinaryFile(Date* date, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!loadDateFromBinaryFile(&date[i], fp))
			return 0;
	}
	return 1;
}

void writeDateToFile(const Date * date, FILE * fp)
{
	if (!date)
		return;
	fprintf(fp, "%d %d %d\n", date->day, date->month, date->year);
	
}


int readDateFromFile(Date* date,FILE* fp)
{	
	if(fscanf(fp, "%d %d %d", &date->day, &date->month, &date->year)!=3)
		return 0;
	fgetc(fp);
	return 1;
}

int loadDateArrToTxtFile(Date* date, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!readDateFromFile(&date[i], fp))
			return 0;
	}
	return 1;
}

void writeDateArrToTxtFile(const Date* date, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)

		writeDateToFile(&date[i], fp);
}

void freeDatePtr(void* v)
{
	Date* d = (Date*)v;
	free(d);
}
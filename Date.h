#pragma once

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);

int		checkDate(char* date, Date* pDate);

void	printDate(const Date* pDate);

int writeDateToBinaryFile(const Date* date, FILE* fp);

int writeDatesArrToBinaryFile(const Date* date, int count, FILE* fp);

int loadDateFromBinaryFile(Date* date, FILE* fp);

int loadDateArrToBinaryFile(Date* date, int count, FILE* fp);

void writeDateToFile(const Date* date, FILE* fp);

int readDateFromFile(Date* date, FILE* fp);

int loadDateArrToTxtFile(Date* date, int count, FILE* fp);

void writeDateArrToTxtFile(const Date* date, int count, FILE* fp);

void freeDatePtr(void* v);
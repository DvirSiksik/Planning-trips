#pragma once

#define MAX_STR_LEN 255


char* getStrExactName(const char* msg);

char* myGets(char* buffer, int size, FILE* fp);

char* getDynStr(char* str);
void generalArrayFunction(void* arr[], int numOfElements, int sizeOfElement, void (*f)(void*));

void generalVariableArrayFunction(void* arr, int numOfElements, int sizeOfElement, void (*f)(void*));

int getString(char* buffer, int len);

void printGeneralEnum(char* msg, const char** eNumArr, int len);

void printString(const void* vC);

void freeString(char* c);



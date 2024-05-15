#pragma once

#define LICENSE_NUM 5
typedef unsigned char BYTE;

typedef struct {
	char* company;
	char license_num [LICENSE_NUM+1];
	int numOfSeats;

}Car;

int initCar(Car* c);

void getLicenseNum(Car* c);

int getNumOfSeats(Car* c);

void printCar(const Car* c);

int	saveCarToBinaryFileCompressed(const Car* pC, FILE* fp);

int readCarFromBinaryFileCompressed(Car* pC, FILE* fp);

int	saveCarToTxtFile(const Car* pC, FILE* fp);

int	readCarToTxtFile(Car* pC, FILE* fp);

void freeCar(Car* c);

#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <crtdbg.h>
#include "Car.h"
#include "General.h"
#include "Macros.h"


int initCar(Car* c)
{
	c->company = getStrExactName("Enter car company name:\n");
	getLicenseNum(c);
		
	if(!getNumOfSeats(c))
		PRINT_MSG_RETURN_ZERO("ERROR");

	
	return 1;
}

void getLicenseNum(Car* c)
{
	char temp[MAX_STR_LEN];
	int ok = 1;
	do {
		ok = 1;
		printf("Enter license number.Total length %d.\n ---Include: upper case letters and numbers---\n\t", LICENSE_NUM);
		myGets(temp, MAX_STR_LEN, stdin);

		if (strlen(temp) != LICENSE_NUM)
		{
			printf("license number should be total %d letters and numbers\n", LICENSE_NUM);
			ok = 0;
		}
		else {
			for (int i = 0; i < LICENSE_NUM; i++)
			{
				if (isupper(temp[i]) == 0 && (temp[i] < '0' || temp[i] > '9'))
				{
					printf("Need to be upper case letter or number\n");
					ok = 0;
					break;
				}
			}
		}
	} while (!ok);

	strcpy(c->license_num, temp);

}

int getNumOfSeats(Car* c)
{
	printf("Enter number of seats in the car:\n");
	do {
		printf("Car has 2-7 seats.\n\t");
		scanf("%d", &c->numOfSeats);
	} while(c->numOfSeats < 2 || c->numOfSeats > 7);
	getchar();
	return 1;
}

void printCar(const Car* c)
{
	printf("Company: %s\n", c->company);
	printf("Lisence number: %s\n", c->license_num);
	printf("Number of seats: %d\n", c->numOfSeats);
}


int	saveCarToBinaryFileCompressed(const Car* pC, FILE* fp)
{
	BYTE data[2] = { 0 };

	int len = (int)strlen(pC->company);
	data[0] = len;
	data[1] = ((pC->numOfSeats)) << 5;

	if (fwrite(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	if (fwrite(pC->company, sizeof(char), len, fp) != len)
		return 0;

	if (fwrite(pC->license_num, sizeof(char), LICENSE_NUM+1, fp) != LICENSE_NUM+1)
		return 0;

	return 1;

}


int readCarFromBinaryFileCompressed(Car* pC, FILE* fp)
{
	BYTE data[2];
	if (fread(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	int len = data[0];
	pC->numOfSeats = (data[1] >>5) &0x7 ;

	pC->company = (char*)calloc(len + 1, sizeof(char));
	if (!pC->company)
		return 0;

	if (fread(pC->company, sizeof(char), len, fp) != len)
	{
		free(pC->company);
		return 0;
	}

	if (fread(pC->license_num, sizeof(char), LICENSE_NUM+1, fp) != LICENSE_NUM+1)
	{
		free(pC->license_num);
		return 0;
	}

	return 1;
}
int	saveCarToTxtFile(const Car* pC, FILE* fp)
{
	fprintf(fp, "%s\n", pC->company);
	fprintf(fp, "%s\n", pC->license_num);
	fprintf(fp, "%d\n", pC->numOfSeats);

	return 1;

}
int	readCarToTxtFile(Car* pC, FILE* fp)
{
	char temp[MAX_STR_LEN];

	myGets(temp, MAX_STR_LEN, fp);
	if (!temp)
		return 0;
	pC->company = getDynStr(temp);

	myGets(pC->license_num, LICENSE_NUM + 1, fp);

	if (fscanf(fp, "%d", &pC->numOfSeats) != 1)
		return 0;
	fgetc(fp);
	return 1;


}


void freeCar(Car* c)
{
	free(c->company);
}




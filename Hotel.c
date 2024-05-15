#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <string.h>
#include "Hotel.h"
#include "General.h"
#include "Macros.h"
#include "fileHelper.h"

int initHotel(Hotel* theHotel)
{
	theHotel->hotelName = getStrExactName("Insert hotel name:\n");
	printf("Insert the price for 1 night:\n");
	scanf("%d", &theHotel->price);

	int option = 0;
	do {
		printf("Is breakfast include in this price?\n1=YES, 0=NO\n");
		scanf("%d", &option);
	} while (option != 0 && option != 1);

	theHotel->includeBreakfast = option;

	L_init(&theHotel->facilitiesList);

	theHotel->hotelArea = getArea("Choose area of the hotel:");

	theHotel->countFacilities = 0;
	chooseFacility(theHotel);

	return 1;
}

void chooseFacility(Hotel* theHotel)
{
	int add = 1;
	int option;
	char* temp;
	int len = 0;
	while (add)
	{
		do {
			printf("Do you want to add facilities of the hotel?\n 1=YES, 0=NO\n");
			scanf("%d", &add);
		} while (add != 1 && add != 0);
	
		if (add)
		{
			printGeneralEnum("Choose one of the following facilities:\n", HotelFacilities, eNofFacilities);
			do {
				printf("Enter a number 1-%d\n", eNofFacilities);
				scanf("%d", &option);
			} while (option < 1 || option > eNofFacilities);
			len = (int)strlen(HotelFacilities[option - 1]);
			temp = (char*)calloc(len + 1, sizeof(char));
			strcpy(temp, HotelFacilities[option - 1]);
			addFacility(&theHotel->facilitiesList, temp);
			theHotel->countFacilities++;
		}
	}
	return;
}

int addFacility(LIST* theList, char* facility)
{
	NODE* tmp = &theList->head;
	if (!L_find(tmp, facility))
		L_insert(tmp, facility);
	else
		printf("There is already this facility in the hotel.\n");
	return 1;
}

void printHotel(const void* v)
{
	const Hotel* theHotel = (Hotel*)v;
	SET_PRINT_TO_DIFFERENT_COLOR("\033[1;36m");
	SET_PRINT_UNDERLINE_TEXT();
	printf("Hotel name: %s\n", theHotel->hotelName);
	SET_PRINT_TO_DEFAULT();
	printf("Area: %s\n", TripArea[theHotel->hotelArea]);
	printf("price: %d\n", theHotel->price);
	
	printf("The hotel price includes breakfast: ");
	PRINT_BOOLEAN(theHotel->includeBreakfast);
	if (theHotel->countFacilities != 0)
	{
		SET_PRINT_UNDERLINE_TEXT();
		printf("The facilities in the hotel are:");
		SET_PRINT_TO_DEFAULT();
		L_print(&theHotel->facilitiesList, printString);
	}

}


void freeHotel(Hotel* theHotel)
{
	L_free(&theHotel->facilitiesList, freeString);
	free(theHotel->hotelName);
}

void freeHotelPtr(void* v)
{
	Hotel* thehotel = (Hotel*)v;
	freeHotel(thehotel);
	free(thehotel);
}

int writeHotelToBinaryFile(const Hotel* hotel, FILE* fp)
{
	writeStringToFile(hotel->hotelName, fp, "ERROR hotel name");

	writeIntToFile(hotel->hotelArea, fp, "ERROR area enum");

	writeIntToFile(hotel->countFacilities, fp, "ERROR");

	L_WriteBFile(&hotel->facilitiesList, fp);

	writeIntToFile(hotel->price, fp, "ERROR");

	writeIntToFile(hotel->includeBreakfast, fp, "ERROR BOOL");

	return 1;
}

int loadHotelFromBinaryFile(Hotel* hotel, FILE* fp)
{
	int tmp = 0;

	hotel->hotelName= readStringFromFile(fp, "ERROR");
	if (!hotel->hotelName)
		return 0;

	if (!readIntFromFile(&tmp, fp, "ERROR"))
		return 0;
	hotel->hotelArea = (eArea)tmp;

	if (!readIntFromFile(&hotel->countFacilities, fp, "ERROR"))
		return 0;

	if (!L_readBFile(&hotel->facilitiesList, hotel->countFacilities, fp))
		return 0;

	if (!readIntFromFile(&hotel->price, fp, "ERROR"))
		return 0;

	if (!readIntFromFile(&tmp, fp, "ERROR"))
		return 0;
	hotel->includeBreakfast = (BOOL)tmp;

	return 1;


}


int writeHotelToTxtFile(const Hotel* hotel, FILE* fp)
{
	fprintf(fp, "%s\n", hotel->hotelName);
	fprintf(fp, "%d\n", (int)hotel->hotelArea);

	fprintf(fp, "%d\n", hotel->countFacilities);
	if (!L_WriteTFile(&hotel->facilitiesList, fp))
		return 0;

	fprintf(fp, "%d\n", hotel->price);

	fprintf(fp, "%d\n", (int)hotel->includeBreakfast);

	return 1;
}

int loadHotelFromTxtFile(Hotel* hotel, FILE* fp)
{
	char temp[MAX_STR_LEN];

	myGets(temp, MAX_STR_LEN, fp);
	hotel->hotelName = getDynStr(temp);

	if (fscanf(fp, "%d", &hotel->hotelArea) != 1)
		return 0;

	if (fscanf(fp, "%d", &hotel->countFacilities) != 1)
		return 0;

	L_readTFile(&hotel->facilitiesList,hotel->countFacilities,fp);

	if (fscanf(fp, "%d", &hotel->price) != 1)
		return 0;

	if (fscanf(fp, "%d", &hotel->includeBreakfast) != 1)
		return 0;

	return 1;
}

int L_find_Hotel(const NODE* pNode, int Value)
{
	int count = 0;
	const NODE* tmp = pNode->next;
	Hotel* tempHotel;
	while (tmp != NULL)
	{
		 tempHotel = (Hotel*)tmp->key;
		 int area = (int)tempHotel->hotelArea;
		 if (area == Value)
		 {
			 printHotel(tempHotel);
			 printf("\n");
			 count++;
		 }
		tmp = tmp->next;
	}

	return count;
}


int L_WriteHotelBFile(const LIST* pList, FILE* f)
{
	NODE* tmp;
	tmp = pList->head.next;
	while (tmp != NULL)
	{
		if(!writeHotelToBinaryFile((Hotel*)tmp->key, f))
	//	if (!writeH(tmp->key, f, "ERROR LIST"))
			return 0;
		tmp = tmp->next;
	}
	return 1;
}

int L_WriteHotelTFile(const LIST* pList, FILE* f)
{
	NODE* tmp;
	tmp = pList->head.next;
	while (tmp != NULL)
	{
		writeHotelToTxtFile((Hotel*)tmp->key, f);
		//fprintf(f, "%s\n", (char*)tmp->key);
		tmp = tmp->next;
	}
	return 1;
}
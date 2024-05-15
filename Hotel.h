#pragma once


#include "list.h"
#include "Trip.h"

typedef enum {
	ePool, eGym, eTennisCourt, eRestaurant, eNofFacilities
}eHotelFacilities;

static const char* HotelFacilities[eNofFacilities] =
{ "Swimming pool", "Gym", "Tennis cort", "Restaurant" };


typedef struct {
	char* hotelName;
	eArea hotelArea;
	LIST facilitiesList;
	int countFacilities;
	int price;
	BOOL includeBreakfast;

}Hotel;

int initHotel(Hotel* theHotel);

void chooseFacility(Hotel* theHotel);

int addFacility(LIST* theList, char* facility);

void printHotel(const void* v);

void freeHotel(Hotel* theHotel);

void freeHotelPtr(void* v);

int writeHotelToBinaryFile(const Hotel* hotel, FILE* fp);

int loadHotelFromBinaryFile(Hotel* hotel, FILE* fp);

int writeHotelToTxtFile(const Hotel* hotel, FILE* fp);

int loadHotelFromTxtFile(Hotel* hotel, FILE* fp);

int L_find_Hotel(const NODE* pNode, int Value);

int L_WriteHotelBFile(const LIST* pList, FILE* f);

int L_WriteHotelTFile(const LIST* pList, FILE* f);

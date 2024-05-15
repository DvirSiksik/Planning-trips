#pragma once


#include "Weather.h"
#include "def.h"
#include "list.h"
#include "General.h"

#define LEN_OF_TRIP_DESC 250

typedef enum {
	eGalil, eGolan, eJerusalem, eCenter, eNegev, eEilat, eNofAreas
}eArea;

static const char* TripArea[eNofAreas]=
 { "Galil", "Golan heights", "Jerusalem", "Center", "Negev", "Eilat" };

typedef enum {
	eEasy, eMedium, eHigh, eNofDifficultyLevel
} eDifficultyLevel;

static const char* TripDifficulty[eNofDifficultyLevel]=
 { "Easy", "Meadium", "High" };

typedef enum {
	eSwimSuite, eTowel, eOpenShoes, eCloseShoes, eNofEquipment
} eEquipment;

static const char* TripEquipment[eNofEquipment] =
{ "Swim suite", "Towel", "OpenShoes", "CloseShoes"};



typedef struct {
	char title[MAX_STR_LEN];
	char description[LEN_OF_TRIP_DESC+1];
	eArea theArea;
	Weather recomendW;
	eDifficultyLevel difLevel;
	int duration;
	int countEquip;
	LIST equipmentsList;

}Trip;

int initTrip(Trip* t);

int addEquipment(LIST* equipmentsList,  char* theEqipment);

eEquipment getEnumEquipment();

eArea getArea(char* msg);

eDifficultyLevel setLevel();

void getDuration(Trip* t);

void printTrip(const void* v);

int write_trip_to_b_file(const Trip* t, FILE* fp);

int write_tripsArr_to_b_file(const Trip* t, int count, FILE* fp);

int loadTripFromBFile(Trip* t, FILE* fp);

int loadTripsArrFromBFile(Trip* t, int count, FILE* fp);

int write_trip_to_txt_file(const Trip* t, FILE* fp);

int loadTripFromTxtFile(Trip* t, FILE* fp);

int write_tripsArr_to_Txt_file(const Trip* t, int count, FILE* fp);

int loadTripsArrFromTxtFile(Trip* t, int count, FILE* fp);

void freeTrip(Trip* t);

void freeTripPtr(void* vT);


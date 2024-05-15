#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <string.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <ctype.h>
#include "trip.h"
#include "Macros.h"
#include "fileHelper.h"


int initTrip(Trip* t)
{
	
	char tmp[MAX_STR_LEN];
	L_init(&t->equipmentsList);
	
	printf("Enter trip title:\n\t");
	myGets(t->title, MAX_STR_LEN, stdin);

	t->theArea = getArea("Choose area of the trip:");

	t->difLevel = setLevel();

	initWeatherForTrip(&t->recomendW);
	
	getDuration(t);
	
	printf("Enter general desription for this trip\n");
	do {
		printf("--Up to %d characters--\n", LEN_OF_TRIP_DESC);
		myGets(tmp, MAX_STR_LEN, stdin);
	} while (strlen(tmp) > LEN_OF_TRIP_DESC);
	strcpy(t->description, tmp);

	t->countEquip = 0;

	printGeneralEnum("Choose equipment neccessery for the trip:", TripEquipment, eNofEquipment);
	int opt = 1;
	char* temp;
	int len = 0;

	do {
		eEquipment theEqipment = getEnumEquipment();
		len = (int)strlen(TripEquipment[theEqipment]);
		temp = (char*)calloc(len + 1, sizeof(char));
		strcpy(temp, TripEquipment[theEqipment]);
		if (addEquipment(&t->equipmentsList, temp))
			t->countEquip++;
		printf("Do you want to insert more equipments from the list?\n1=YES   0=NO\n\t");
		scanf("%d", &opt);
	} while (opt);
	
	return 1;
}

int addEquipment(LIST* equipmentsList,  char* theEqipment)
{
	NODE* tmp = &equipmentsList->head;
	if (!L_find(tmp, theEqipment))
	{
		L_insert(tmp, theEqipment);
		return 1;
	}
		
	else
	{
		printf("--This equipment is already in the list--\n");
		return 0;
	}
		
}

eEquipment getEnumEquipment()
{
	int option = 0;
	do {
		printf("The num has to be 1-%d: \n", eNofEquipment);
		scanf("%d", &option);
	} while (option <1 || option>eNofEquipment);
	getchar();
	return (eEquipment)option - 1;
}

eArea getArea(char* msg)
{
	int option = 0;

	printGeneralEnum(msg, TripArea, eNofAreas);

	do {
		printf("The num has to be 1-%d: \n", eNofAreas);
		scanf("%d", &option);
	} while (option <1 || option>eNofAreas );
	getchar();
	return (eArea)option-1;
}

eDifficultyLevel setLevel()
{
	int option = 0;

	printGeneralEnum("Choose difficulty of the trip:", TripDifficulty, eNofDifficultyLevel);
	
	do {
		printf("The num has to be 1-%d: \n", eNofDifficultyLevel);
		scanf("%d", &option);
	} while (option <1 || option>eNofDifficultyLevel);
	getchar();
	return (eDifficultyLevel)option-1;
}

void getDuration(Trip* t)
{
	printf("How much time does the trip take in average? \n");
	do {
		printf("--The time is full hour--\n--Has to be  between 0 hours to 10 hours--\n\t");
		scanf("%d", &t->duration);
	} while (t->duration < 0 || t->duration > 10);
}

void printTrip(const void* v)
{
	const Trip* t = (Trip*)v;
	SET_PRINT_TO_DIFFERENT_COLOR("\033[1;92m");
	SET_PRINT_UNDERLINE_TEXT();
	printf("Trip name: %s\n", t->title);
	SET_PRINT_TO_DEFAULT();

	printf("Area: %s\n", TripArea[t->theArea]);
	SET_PRINT_UNDERLINE_TEXT();
	printf("\nWeather: \n");
	SET_PRINT_TO_DEFAULT();
	printWeather(&t->recomendW);
	printf("\nDifficulty: %s\n", TripDifficulty[t->difLevel]);
	printf("Average duration: %d\n", t->duration);
	printf("Trip description: %s\n", t->description);
	SET_PRINT_UNDERLINE_TEXT();
	printf("\nEquipment list:");
	SET_PRINT_TO_DEFAULT();
	L_print(&t->equipmentsList, printString);
	printf("\n");
}

int write_trip_to_b_file(const Trip* t, FILE* fp)
{
	int tmp = 0;

	writeStringToFile(t->title, fp, "ERROR write title");

	writeStringToFile(t->description, fp, "ERROR write descriptiom");

	writeIntToFile(t->theArea, fp, "ERROR write area enum");

	writeWeatherToBFile(&t->recomendW, fp);

	writeIntToFile(t->difLevel, fp, "ERROR write difLevel enum");

	writeIntToFile(t->duration, fp, "ERROR write duration ");

	writeIntToFile(t->countEquip, fp, "ERROR write count equip");

	L_WriteBFile(&t->equipmentsList, fp);

	return 1;

}


int loadTripFromBFile(Trip* t, FILE* fp)
{
	int tmp = 0;

	if (!readIntFromFile(&tmp, fp, "ERROR"))
		return 0;


	fread(t->title, sizeof(char), tmp, fp);

	if (!readIntFromFile(&tmp, fp, "ERROR"))
		return 0;


	fread(t->description, sizeof(char), tmp, fp);

	if (!readIntFromFile(&tmp, fp, "ERROR"))
		return 0;
	t->theArea = tmp;

	if (!loadWeatherToBFile(&t->recomendW, fp))
		return 0;
	
	if (!readIntFromFile(&tmp, fp, "ERROR"))
		return 0;
	t->difLevel = tmp;

	if (!readIntFromFile(&t->duration, fp, "ERROR"))
		return 0;

	if (!readIntFromFile(&t->countEquip, fp, "ERROR"))
		return 0;

	if (!L_readBFile(&t->equipmentsList,t->countEquip, fp))
		return 0;
	return 1;
}

int write_tripsArr_to_b_file(const Trip* t,int count, FILE* fp)
{

	for (int i = 0; i < count; i++)
	{
		if (!write_trip_to_b_file(&t[i], fp))
			return 0;
	}
	return 1;
}

int loadTripsArrFromBFile(Trip* t,int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!loadTripFromBFile(&t[i], fp))
			return 0;
	}
	return 1;
}
int write_trip_to_txt_file(const Trip* t, FILE* fp)
{
	fprintf(fp, "%s\n", t->title);
	fprintf(fp, "%s\n", t->description);
	fprintf(fp, "%d\n", (int)t->theArea);
	writeWeatherToTxtFile(&t->recomendW, fp);
	fprintf(fp, "%d\n", (int)t->difLevel);
	fprintf(fp, "%d\n", t->duration);
	fprintf(fp, "%d\n", t->countEquip);
	L_WriteTFile(&t->equipmentsList, fp);
	return 1;

}
int loadTripFromTxtFile(Trip* t, FILE* fp)
{
	int tmp = 0;

	myGets(t->title, MAX_STR_LEN, fp);

	myGets(t->description, LEN_OF_TRIP_DESC, fp);

	if (fscanf(fp, "%d", &tmp) != 1)
		return 0;
	fgetc(fp);
	t->theArea = tmp;

	if (!loadWeatherToTxtFile(&t->recomendW, fp))
		return 0;
	if (fscanf(fp, "%d", &tmp) != 1)
		return 0;
	fgetc(fp);
	t->difLevel = tmp;

	if (fscanf(fp, "%d", &tmp) != 1)
		return 0;
	fgetc(fp);
	t->duration = tmp;

	if (fscanf(fp, "%d", &t->countEquip) != 1)
		return 0;
	fgetc(fp);

	if (!L_readTFile(&t->equipmentsList, t->countEquip, fp))
		return 0;

	return 1;
}

int write_tripsArr_to_Txt_file(const Trip* t, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!write_trip_to_txt_file(&t[i], fp))
			return 0;
	}
	return 1;
}

int loadTripsArrFromTxtFile(Trip* t, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!loadTripFromTxtFile(&t[i], fp))
			return 0;
	}
	return 1;
}

void freeTrip(Trip* t)
{
	L_free(&t->equipmentsList, freeString);
}

void freeTripPtr( void* vT)
{
	 Trip* pT = (Trip*)vT;
	freeTrip(pT);
}


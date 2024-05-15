#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <crtdbg.h>
#include "Weather.h"
#include "General.h"
#include "fileHelper.h"

int initWeather(Weather* w)
{
	w->temperature = 0.0;
	w->theType = 0;
	return 1;
}

int initWeatherForTrip(Weather* w)
{
	w->theType = getWeatherType();
	printf("Enter recomended temperature for trip: \n");
	do {
		printf("Temperature has to be 0-50 degrees: \n\t");
		scanf("%f", &w->temperature);
	} while (w->temperature < MIN_TEMP || w->temperature > MAX_TEMP);

	return 1;
}

eWeatherType getWeatherType()
{
	int option = 0;

	printGeneralEnum("Choose recomended weather for the trip:", weatherType, eNofWeather);

	do {
		printf("The num has to be 1-%d: \n", eNofWeather);
		scanf("%d", &option);
	} while (option <1 || option>eNofWeather);
	getchar();
	return (eWeatherType)option-1;
}


void printWeather(const Weather* w)
{
	printf("Temperature: %.2f \n", w->temperature);
	printf("Weather description: %s\n", weatherType[w->theType]);
}

int writeWeatherToBFile(const Weather* w, FILE* fp)
{
	writeFloatToFile(w->temperature, fp, "ERROR");
	writeIntToFile(w->theType, fp, "ERROR");
	return 0;
}

int loadWeatherToBFile(Weather* w, FILE* fp)
{
	int tmp=0;
	if (!readFloatFromFile(&w->temperature, fp, "ERROR"))
		return 0;

	if (!readIntFromFile(&tmp, fp, "ERROR"))
		return 0;
	w->theType = (eWeatherType)tmp;

	return 1;
}
void writeWeatherToTxtFile(const Weather* w, FILE* fp)
{
	fprintf(fp, "%.2f\n", w->temperature);
	fprintf(fp, "%d\n", (int)w->theType);
}

int loadWeatherToTxtFile(Weather* w, FILE* fp)
{
	if (fscanf(fp, "%f", &w->temperature) != 1)
		return 0;
	fgetc(fp);
	if (fscanf(fp, "%d", &w->theType) != 1)
		return 0;
	fgetc(fp);
	return 1;
}


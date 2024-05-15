#pragma once
#include "Manager.h"
#include "Forcast.h"


int	initManagerFromFile(Manager* m, const char* fileName);

int	saveManagerToBFile(const Manager* pM, const char* fileName);

int	initManagerFromTxtFile(Manager* m, const char* fileName);

int	saveManagerToTxtFile(const Manager* pM, const char* fileName);

int loadForecastArrFromTFile(Manager* m, FILE* fp);

int loadForecastArrFromBFile(Manager* m, FILE* fp);

int createCitiesForecastsArr(Manager* m);

int createUSersArr(Manager* m);

int	createTripsArr(Manager* m);

int loadHotelsListFromBinaryFile(Manager* m, int count, FILE* fp);

int loadHotelsListFromTxtFile(Manager* m, int count, FILE* fp);



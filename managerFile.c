#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "General.h"
#include "fileHelper.h"
#include "managerFile.h"
#include "Manager.h"
#include "Macros.h"


int	initManagerFromFile(Manager* m, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open manager file\n");
		return 0;
	}

	m->allForecast = NULL;
	int tmp = 0;
	if (!readIntFromFile(&m->countForecasts, fp, "Error reading cities forecasts count\n"))
	{
		fclose(fp);
		return 0;
	}

	if (!createCitiesForecastsArr(m))
	{
		printf("Alocation error\n");
		fclose(fp);
		return 0;
	}

	if (!loadForecastArrFromBFile(m, fp))
	{
		printf("Alocation error\n");
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		fclose(fp);
		return 0;
	}

	if (!readIntFromFile(&m->countTrips, fp, "Error reading trips count\n"))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		fclose(fp);
		return 0;
	}

	if (!createTripsArr(m))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		fclose(fp);
		return 0;
	}

	if (!loadTripsArrFromBFile(m->tripsArr,m->countTrips, fp))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		fclose(fp);
		return 0;
	}

	if (!readIntFromFile(&m->countUsers, fp, "Error reading users count\n"))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		fclose(fp);
		return 0;
	}

	if (!createUSersArr(m))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		fclose(fp);
		return 0;
	}

	if (!loadUsersArrFromBinaryFile(m->allUsers, m->countUsers, fp))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		fclose(fp);
		return 0;
	}

	if (!readIntFromFile(&m->countHotels, fp, "Error reading hotels count\n"))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		fclose(fp);
		return 0;
	}

	if (!L_init(&m->hotelList))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		fclose(fp);
		return 0;
	}

	if (!loadHotelsListFromBinaryFile(m, m->countHotels,fp))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		L_free(&m->hotelList, freeHotelPtr);
		fclose(fp);
		return 0;
	}
	
	if (!readIntFromFile(&tmp, fp, "ERROR"))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		L_free(&m->hotelList, freeHotelPtr);
		fclose(fp);
		return 0;
	}
	m->type = (eSortType)tmp;
	fclose(fp);
	return 1;
}


int	saveManagerToBFile(const Manager* pM, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp) 
		PRINT_MSG_RETURN_ZERO("Error open manager file to write\n");
	

	if (!writeIntToFile(pM->countForecasts, fp, "Error write cities forecasts count\n"))
		CLOSE_FILE_RETURN_ZERO(fp);

	for (int i = 0; i < pM->countForecasts; i++)
	{
		if (!write_forecast_to_b_file(pM->allForecast[i], fp))
			CLOSE_FILE_RETURN_ZERO(fp);
	}

	if (!writeIntToFile(pM->countTrips, fp, "Error write trips count\n"))
		CLOSE_FILE_RETURN_ZERO(fp);

	if (!write_tripsArr_to_b_file(pM->tripsArr,pM->countTrips,fp))
		PRINT_MSG_CLOSE_FILE_RETURN_ZERO("Error write trips array\n", fp);
	
	if (!writeIntToFile(pM->countUsers, fp, "Error write users count\n"))
		CLOSE_FILE_RETURN_ZERO(fp);

	if(!writeUsersArrToBinaryFile(pM->allUsers,pM->countUsers,fp))
		PRINT_MSG_CLOSE_FILE_RETURN_ZERO("Error write users array\n", fp);

	if (!writeIntToFile(pM->countHotels, fp, "Error write hotels count\n"))
		CLOSE_FILE_RETURN_ZERO(fp);

	if(!L_WriteHotelBFile(&pM->hotelList,fp))
		PRINT_MSG_CLOSE_FILE_RETURN_ZERO("Error write hotels array\n", fp);

	if(!writeIntToFile(pM->type,fp,"ERROR"))
		CLOSE_FILE_RETURN_ZERO(fp);


	fclose(fp);
	return 1;
}




int	initManagerFromTxtFile(Manager* m, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "r");
	if (!fp)
	{
		printf("Error open manager file\n");
		return 0;
	}

	m->allForecast = NULL;
	int tmp = 0;

	if (fscanf(fp, "%d", &m->countForecasts) != 1)
	{
		fclose(fp);
		return 0;
	}
	fgetc(fp);
	if (!createCitiesForecastsArr(m))
	{
		printf("Alocation error\n");
		fclose(fp);
		return 0;
	}

	if (!loadForecastArrFromTFile(m, fp))
	{
		printf("Alocation error\n");
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		fclose(fp);
		return 0;
	}

	if(fscanf(fp, "%d", &m->countTrips)!=1)
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		fclose(fp);
		return 0;
	}
	fgetc(fp);
	if (!createTripsArr(m))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		fclose(fp);
		return 0;
	}

	if (!loadTripsArrFromTxtFile(m->tripsArr, m->countTrips, fp))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		fclose(fp);
		return 0;
	}

	if (fscanf(fp, "%d", &m->countUsers) != 1)
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		fclose(fp);
		return 0;
	}
	fgetc(fp);
	if (!createUSersArr(m))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		fclose(fp);
		return 0;
	}

	if (!loadUsersArrFromTxtFile(m->allUsers, m->countUsers, fp))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		fclose(fp);
		return 0;
	}

	if (fscanf(fp, "%d", &m->countHotels) != 1)
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		fclose(fp);
		return 0;
	}
	fgetc(fp);
	if (!L_init(&m->hotelList))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		fclose(fp);
		return 0;
	}

	if(!loadHotelsListFromTxtFile(m,m->countHotels,fp))
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		L_free(&m->hotelList, freeHotelPtr);
		fclose(fp);
		return 0;
	}

	if (fscanf(fp, "%d", &tmp) != 1)
	{
		generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast*), freeForecastPtr);
		free(m->allForecast);
		generalArrayFunction(&m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
		free(m->tripsArr);
		generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
		free(m->allUsers);
		L_free(&m->hotelList, freeHotelPtr);
		fclose(fp);
		return 0;
	}
	fgetc(fp);
	m->type = (eSortType)tmp;
	fclose(fp);
	return 1;
	
}


int	saveManagerToTxtFile(const Manager* pM, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "w");
	if (!fp)
		PRINT_MSG_RETURN_ZERO("Error open manager file to write\n");

	fprintf(fp, "%d\n", pM->countForecasts);	

	for (int i = 0; i < pM->countForecasts; i++)
	{
		if (!write_forecast_to_file(pM->allForecast[i],fp))
			CLOSE_FILE_RETURN_ZERO(fp);
	}

	fprintf(fp, "%d\n", pM->countTrips);


	if (!write_tripsArr_to_Txt_file(pM->tripsArr, pM->countTrips, fp))
		PRINT_MSG_CLOSE_FILE_RETURN_ZERO("Error write trips array\n", fp);

	
	fprintf(fp, "%d\n", pM->countUsers);

	if (!writeUsersArrToTxtFile(pM->allUsers, pM->countUsers, fp))
		PRINT_MSG_CLOSE_FILE_RETURN_ZERO("Error write users array\n", fp);


	fprintf(fp, "%d\n", pM->countHotels);

	if(!L_WriteHotelTFile(&pM->hotelList, fp))
		PRINT_MSG_CLOSE_FILE_RETURN_ZERO("Error write hotels array\n", fp);


	fprintf(fp, "%d\n", pM->type);


	fclose(fp);
	return 1;
}

int loadForecastArrFromBFile(Manager* m, FILE* fp)
{

	for (int i = 0; i < m->countForecasts; i++)
	{
		if (!loadForecastFromBFile(m->allForecast[i], fp))
		{
			return 0;
		}
	}
	return 1;
}

int loadForecastArrFromTFile(Manager* m, FILE* fp)
{

	for (int i = 0; i < m->countForecasts; i++)
	{
		if (!loadForecastFromFile(m->allForecast[i], fp))
		{
			return 0;
		}
	}
	return 1;
}


int createCitiesForecastsArr(Manager* m)
{
	if (m->countForecasts > 0)
	{
		m->allForecast = (Forecast**)malloc(m->countForecasts * sizeof(Forecast*));
		if (!m->allForecast)
		{
			printf("Alocation error\n");
			return 0;
		}
	}
	else
		m->allForecast = NULL;

	for (int i = 0; i < m->countForecasts; i++)
	{
		m->allForecast[i] = (Forecast*)calloc(NUM_OF_FORECASTS, sizeof(Forecast));
		if (!m->allForecast[i])
		{
			printf("Alocation error\n");
			return 0;
		}
	}
	return 1;
}

int createUSersArr(Manager* m)
{
	if (m->countUsers > 0)
	{
		m->allUsers = (User**)malloc(m->countUsers * sizeof(User*));
		if (!m->allUsers)
		{
			printf("Alocation error\n");
			return 0;
		}
	}
	else
		m->allUsers = NULL;

	for (int i = 0; i < m->countUsers; i++)
	{
		m->allUsers[i] = (User*)calloc(1, sizeof(User));
		if (!m->allUsers[i])
		{
			printf("Alocation error\n");
			return 0;
		}
	}
	return 1;
}


int	createTripsArr(Manager* m)
{
	m->tripsArr = (Trip*)malloc(m->countTrips * sizeof(Trip));
	if (!m->tripsArr)
	{
		printf("Alocation error\n");
		return 0;
	}
	return 1;
}



int loadHotelsListFromBinaryFile(Manager* m, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		Hotel* hotelTmp=calloc(1,sizeof(Hotel));
		
		loadHotelFromBinaryFile(hotelTmp, fp);
		if (!hotelTmp)
			return 0;
		L_insert(&m->hotelList.head, hotelTmp);
		
	}
	return 1;
}

int loadHotelsListFromTxtFile(Manager* m, int count, FILE* fp)
{
	Hotel* hotelTmp;
	for (int i = 0; i < count; i++)
	{

		 hotelTmp=calloc(1,sizeof(Hotel));
		loadHotelFromTxtFile(hotelTmp, fp);
		if (!hotelTmp)
			return 0;
		L_insert(&m->hotelList.head, hotelTmp);
	
		
	}

	return 1;
}
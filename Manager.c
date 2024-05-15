#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include "Manager.h"
#include <crtdbg.h>
#include "General.h"
#include "Macros.h"

int initManager(Manager* m)
{
	m->allForecast = (Forecast**)calloc(eNofAreas, sizeof(Forecast*));
	if (!m->allForecast)
		FREE_RETURN_ZERO(m->allForecast);
	
	m->countTrips = 0;

	m->countForecasts = 0;

	m->allUsers = (User**)calloc(1, sizeof(User*));
	if (!m->allUsers)
		FREE_RETURN_ZERO(m->allUsers);
	
	m->countUsers = 0;

	L_init(&m->hotelList);
	m->countHotels = 0;

	m->type = 0;
	m->tripsArr = NULL;
	
	getForcastAllCities(m);
	return 1;
}

int updateForecastAllCites(Manager* m)
{
	generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast**), freeForecastPtr);
	free(m->allForecast);
	m->allForecast = (Forecast**)calloc(eNofAreas, sizeof(Forecast*));
	if (!m->allForecast)
		FREE_RETURN_ZERO(m->allForecast);
	m->countForecasts = 0;
	if (!getForcastAllCities(m))
		return 0;
	return 1;
}

int getForcastAllCities(Manager* m)
{
	
	for (int i = 0; i < eNofAreas; i++)
	{
		m->allForecast[i] = (Forecast*)calloc(NUM_OF_FORECASTS, sizeof(Forecast));
		if (!m->allForecast[i])
			free(m->allForecast[i]);

		getForcast(TripAreaCities[i], m->allForecast[i]);
		m->countForecasts++;
	}
	
	return 1;
}

void printForecastForCity(Forecast** allForecasts)
{
	for (int i = 0; i < eNofAreas; i++)
	{
		SET_PRINT_TO_DIFFERENT_COLOR("\033[1;92m");
		SET_PRINT_UNDERLINE_TEXT();
		printf("Forecast for: %s\n", TripAreaCities[i]);
		SET_PRINT_TO_DEFAULT();

		printSelectedForecasts(allForecasts[i]);
		printf("\n");
	}
}

int addNewUser(Manager* m)
{
	User* theUser =(User*)calloc(1, sizeof(User));
	if (!initUser(theUser))
		FREE_RETURN_MINUS_ONE(theUser);

	if (bsearch(&theUser, m->allUsers, m->countUsers, sizeof(User**), compareUserByID)!=NULL)
	{
		printf("There is already user with this ID\n");
		FREE_RETURN_MINUS_ONE(theUser);
	}

	m->allUsers = (User**)realloc(m->allUsers, (m->countUsers + 1) * sizeof(User*));
	if (!m->allUsers)
		FREE_RETURN_MINUS_ONE(m->allUsers);

	m->allUsers[m->countUsers] = theUser;
	m->countUsers++;

	sortUsersArr(m);
	
	return theUser->id;
}

int initTripForUser(Trip* UTrip, Trip* theTrip)
{
	strcpy(UTrip->title, theTrip->title);
	UTrip->theArea = theTrip->theArea;
	UTrip->recomendW = theTrip->recomendW;
	strcpy(UTrip->description, theTrip->description);
	UTrip->difLevel = theTrip->difLevel;
	UTrip->duration = theTrip->duration;
	UTrip->countEquip = theTrip->countEquip;
	if (!L_init(&UTrip->equipmentsList))
		return 0;
	NODE* tmp = theTrip->equipmentsList.head.next;
	NODE* headList = &UTrip->equipmentsList.head;
	int len;
	for (int i = 0; i < UTrip->countEquip; i++)
	{
		len =(int) strlen((char*)tmp->key);
		char* str = (char*)calloc(len+1, sizeof(char));
		strcpy(str,(char*)tmp->key);
		if (!L_insert(headList, str))
			return 0;
		tmp = tmp->next;
	}
	return 1;
}

int addTripForUser(User* u, Trip* t, Date* d)
{
	u->tripsArr = (Trip*)realloc(u->tripsArr, (u->countTrips + 1) * sizeof(Trip));
	if (!u->tripsArr)
		CHECK_NULL_FREE_RETURN_ZERO(u->tripsArr);


	if (&(u->tripsArr[u->countTrips]) == NULL)
		PRINT_MSG_RETURN_ZERO("__ERROR ALLOCATING SPACE__\n");

	initTripForUser(&u->tripsArr[u->countTrips],t);

	u->tripsDates = (Date*)realloc(u->tripsDates, (u->countTrips + 1) * sizeof(Date));
	if (!u->tripsDates)
		FREE_RETURN_ZERO(u->tripsDates);


	if (&(u->tripsDates[u->countTrips]) == NULL)
		PRINT_MSG_RETURN_ZERO("__ERROR ALLOCATING SPACE__\n");

	u->tripsDates[u->countTrips] = *d;

	u->countTrips++;

	
	return 1;
}


void addNewTrip(Manager* m)
{
	Trip theTrip;
	if (!initTrip(&theTrip))
		return;

	m->tripsArr = (Trip*)realloc(m->tripsArr, (m->countTrips + 1) * sizeof(Trip));
	if (!m->tripsArr)
		FREE_RETURN(m->tripsArr);
	
	m->tripsArr[m->countTrips] = theTrip;

	m->countTrips++;
}


eSortType getSortType()
{
	int option;
	printf("\n\n");
	
	printGeneralEnum("Base on what field do you want to sort ?", SortTypeStr, eNofSortTypes);
	do {
		printf("Enter a number 1-%d\n", eNofSortTypes);
		scanf("%d", &option);
	} while (option < 1 || option > eNofSortTypes);
	getchar();
	return (eSortType)(option-1);
}

void  sortTrip(Manager* m)
{
	if (m->countTrips == 0)
		PRINT_MSG_RETURN("///		There are no trips to be sorted		///");
	
	eSortType option = getSortType();

	switch (option)
	{
	case 1:
		qsort(m->tripsArr, m->countTrips, sizeof(Trip), compareTripByArea);
		break;
	case 2:
		qsort(m->tripsArr, m->countTrips, sizeof(Trip), compareTripByDuration);
		break;
	case 3:
		qsort(m->tripsArr, m->countTrips, sizeof(Trip), compareTripByDifficulty);
		break;
	}
	m->type = option;
}

void findTrip(Manager* m)
{
	Trip* pTrip = (Trip*)calloc(1, sizeof(Trip));
	if (pTrip)
	{
		if (m->type != 0)
		{
			switch (m->type)
			{
			case 1:
				printf("Search by location:	\n");
				pTrip->theArea = getArea("Choose area of the trip:");
				pTrip = (bsearch(pTrip, m->tripsArr, m->countTrips, sizeof(Trip), compareTripByArea));
				break;
			case 2:
				printf("Search by duration:	\n");
				getDuration(pTrip);
				pTrip = (bsearch(pTrip, m->tripsArr, m->countTrips, sizeof(Trip), compareTripByDuration));
				break;
			case 3:
				printf("Search by difficulty:	\n");
				pTrip->difLevel = setLevel();
				pTrip = (bsearch(pTrip, m->tripsArr, m->countTrips, sizeof(Trip), compareTripByDifficulty));
				break;
			}

			if (pTrip)
			{
				SET_PRINT_TO_DIFFERENT_COLOR("\033[1;32m");
				SET_PRINT_UNDERLINE_TEXT();
				printf("Example for Trip found:\n");
				SET_PRINT_TO_DEFAULT();
				printTrip(pTrip);
			
			}
			else
				printf("Trip was not found :( \n");
				
		}
		else
		{
			printf("The search cannot be performed, array not sorted\n");
			free(pTrip);
		}
			
	
	}
	else
		free(pTrip);
}

void sortUsersArr(Manager* m)
{
	if (m->countUsers == 0)
	{
		PRINT_MSG_RETURN("///		There are no users to be sorted		///");
	}

	qsort(m->allUsers, m->countUsers, sizeof(User**), compareUserByID);
}

int findUserByID(Manager* m)
{
	int id = 0;
	printf("Enter user ID: \n\t");
	scanf("%d", &id);

	for (int i = 0; i < m->countUsers; i++)
	{
		if (id == m->allUsers[i]->id)
			return i;
	}

	return -1;
}

void addNewHotel(Manager* m)
{
	Hotel* theHotel;
	theHotel = calloc(1, sizeof(Hotel));
	if (!initHotel(theHotel))
		return;
	NODE* pNode = &m->hotelList.head;
	
	L_insert(pNode, theHotel);
	m->countHotels++;
} 


int compareUserByID(const void* v1, const void* v2)
{
	const User* pU1 = *(const void**)v1;
	const User* pU2 = *(const void**)v2;
	return pU1->id - pU2->id;
}

int	compareTripByArea(const void* v1, const void* v2)
{
	const Trip* pT1 = (const Trip*)v1;
	const Trip* pT2 = (const Trip*)v2;
	return pT1->theArea - pT2->theArea;
}

int	compareTripByDuration(const void* v1, const void* v2)
{
	const Trip* pT1 = (const Trip*)v1;
	const Trip* pT2 = (const Trip*)v2;
	return pT1->duration - pT2->duration;
}

int	compareTripByDifficulty(const void* v1, const void* v2)
{
	const Trip* pT1 = (const Trip*)v1;
	const Trip* pT2 = (const Trip*)v2;
	return pT1->difLevel - pT2->difLevel;
}

int	compareTripByTitle(const void* v1, const void* v2)
{
	const Trip* pT1 = (const Trip*)v1;
	const Trip* pT2 = (const Trip*)v2;
	return strcmp(pT1->title,pT2->title);
}


int planTripForUser(Manager* m, User* pUser)
{
	if (m->countTrips == 0)
		PRINT_MSG_RETURN_ZERO("///		There are no trips in our stock yet		/// \n");

	if (pUser->countTrips > MAX_NUM_TRIPS)
		PRINT_MSG_RETURN_ZERO("You arrived to maximum trips for your user\n");

	Date pDate;

	do {
		getCorrectDate(&pDate);
	} while (checkRangeDate(m->allForecast[0], &pDate)==-1);

	int optionArea=getArea("Choose area for your trip:\n");

	int temperature;
	char* wind;
	char* weatherDesc;
	temperature = getMaxTemperatureForDay(m->allForecast[optionArea], &pDate);
	wind = getWindForDay(m->allForecast[optionArea], &pDate);
	weatherDesc = getWeatherDescForDay(m->allForecast[optionArea], &pDate);

	int countTripsToPresent = 0;
	for (int i = 0; i < m->countTrips; i++)
	{
		if (!strcmp(TripArea[optionArea] , TripArea[m->tripsArr[i].theArea]))
		{
			int minTemperature = (int)(m->tripsArr[i].recomendW.temperature - RANGE_OF_TEMPERATURE);
			int maxTemperature = (int)(m->tripsArr[i].recomendW.temperature + RANGE_OF_TEMPERATURE);
			if (temperature > minTemperature && temperature < maxTemperature)
			{
				printTrip(&m->tripsArr[i]);
				countTripsToPresent++;
				printf("\n\n");
			}	
		}
	}

	if (countTripsToPresent == 0)
	{
		printf("There are no compatible trips according to your request.\n");
		return 1;
	}
		
	Trip pT;

	int opt = 0;
	do {
		printf("Insert title of the trip you want:\n--Please pay attention to UPPER/LOWER case--\n");
		myGets(pT.title, MAX_STR_LEN, stdin);
		for (int i = 0; i < m->countTrips; i++)
		{
			if (compareTripByTitle(&pT, &m->tripsArr[i])==0)
			{	
				addTripForUser(pUser, &m->tripsArr[i], &pDate);
				
				SET_PRINT_TO_DIFFERENT_COLOR("\033[1;33m");
				printf("\nThe expected weather for the trip you choose to travel at ");
				printDate(&pDate);
				printf("\nThe expected temperature is: %d°C \n", temperature);
				printf("The wind speed will be: %s m/s\n", wind);
				printf("And the weather will be: %s\n\n", weatherDesc);
				SET_PRINT_TO_DEFAULT();

				int countHotelsToPresent = 0;
				SET_PRINT_TO_DIFFERENT_COLOR("\033[1;35m");
				SET_PRINT_UNDERLINE_TEXT();
				printf("We have some suggestions for hotels in this area: \n");
				SET_PRINT_TO_DEFAULT();
				
				NODE* tmpList = &m->hotelList.head;
				countHotelsToPresent =L_find_Hotel(tmpList, optionArea);

				if (countHotelsToPresent == 0)
				{
					SET_PRINT_TO_DIFFERENT_COLOR("\033[1;31m");
					printf("--SORRY-- \n currently we dont have hotels suggestions for this area.\n");
					SET_PRINT_TO_DEFAULT();
				}
				return 1;
			}
		}
		printf("Do you want to try again? \n1=YES 0=NO\n");
		scanf("%d", &opt);
	} while (opt);
	
	return 0;
}


int checkRangeDate(Forecast* f, Date* d)
{
	if (d->day < f[0].date.day || d->day  > f[NUM_OF_FORECASTS - 1].date.day)
		PRINT_MSG_RETURN_MINUS_ONE("\t\t__SORRY__ \nWe cant plan trip for this date. Please try again.\n");
	if (d->month < f[0].date.month || d->month  > f[NUM_OF_FORECASTS - 1].date.month)
		PRINT_MSG_RETURN_MINUS_ONE("\t\t__SORRY__ \nWe cant plan trip for this date. Please try again.\n");
	if (d->year < f[0].date.year || d->year  > f[NUM_OF_FORECASTS - 1].date.year)
		PRINT_MSG_RETURN_MINUS_ONE("\t\t__SORRY__ \nWe cant plan trip for this date. Please try again.\n");
	return 1;
}

void freeManager(Manager* m)
{
	generalArrayFunction(m->allForecast, m->countForecasts, sizeof(Forecast**), freeForecastPtr);
	free(m->allForecast);

	generalArrayFunction(m->allUsers, m->countUsers, sizeof(User*), freeUserPtr);
	free(m->allUsers);
	generalVariableArrayFunction(m->tripsArr, m->countTrips, sizeof(Trip), freeTripPtr);
	free(m->tripsArr);

	L_free(&m->hotelList, freeHotelPtr);
}
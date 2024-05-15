#pragma once

#include "Forcast.h"
#include "Trip.h"
#include "User.h"
#include "Hotel.h"

static const char* TripAreaCities[eNofAreas] =
{ "zefat", "katzrin", "jerusalem", "tel aviv", "beer sheva", "eilat" };

typedef enum {
	eNoSort, eLocation, eDuration, eDifficulty, eNofSortTypes
} eSortType;

static const char* SortTypeStr[eNofSortTypes]
= { "Not Sort","Location", "Duration", "Difficulty" };

#define RANGE_OF_TEMPERATURE 7

typedef struct {
	
	Forecast** allForecast; //Forecasts for all of our cities in the following next week.
	int countForecasts;
	Trip* tripsArr;
	int countTrips;
	eSortType type;
	User** allUsers;
	int countUsers;
	LIST hotelList;
	int countHotels;

}Manager;

int initManager(Manager* m);

int updateForecastAllCites(Manager* m);

int getForcastAllCities(Manager* m);

int addNewUser(Manager* m);

int initTripForUser(Trip* UTrip, Trip* theTrip);

int addTripForUser(User* u,Trip* t, Date* d);

void printForecastForCity(Forecast** allForecasts);

void addNewTrip(Manager* m);

eSortType getSortType();

void  sortTrip(Manager* m);

void findTrip(Manager* m);

void sortUsersArr(Manager* m);

int findUserByID(Manager* m);

void addNewHotel(Manager* m);

int compareUserByID(const void* v1, const void* v2);

int	compareTripByArea(const void* v1, const void* v2);

int	compareTripByDuration(const void* v1, const void* v2);

int	compareTripByDifficulty(const void* v1, const void* v2);

int	compareTripByTitle(const void* v1, const void* v2);

int planTripForUser(Manager* m, User* pUser);

int checkRangeDate(Forecast* f, Date* d);

void freeManager(Manager* m);

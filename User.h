#pragma once
#include "Car.h"
#include "Trip.h"
#include "Date.h"
#include "Hotel.h"

#define MAX_NUM_TRIPS 20
#define LEN_USER_NAME 3

typedef struct {
	char* firstName;
	char* lastName;
	char userName[LEN_USER_NAME+1];
	int id;
	int age;
	char* email;
	char* city;
	Car theCar;
	Trip* tripsArr;
	Date* tripsDates;
	int countTrips;
}User;

int initUser(User* u);

void modifyUserName(User* u);

void printUser(const void* v);

void printTripsArrForUser(const User* u);

void freeUser(User* u);

void freeUserPtr(void* vU);

int writeUserToBinaryFile(const User* user, FILE* fp);

int loadUserFromBinaryFile(User* user, FILE* fp);

int loadUsersArrFromBinaryFile(User** user, int count, FILE* fp);

int writeUsersArrToBinaryFile(const User** user, int count, FILE* fp);

int loadUserFromTxtFile(User* user, FILE* fp);

int writeUserToTxtFile(const User* user, FILE* fp);

int loadUsersArrFromTxtFile(User** user, int count, FILE* fp);

int writeUsersArrToTxtFile(const User** user, const int count, FILE* fp);

int	createTripsArrForUser(User* u);

int	createDatesArrForUser(User* u);


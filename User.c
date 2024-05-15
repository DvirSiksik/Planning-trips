#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <crtdbg.h>
#include "User.h"
#include "General.h"
#include "Macros.h"
#include "fileHelper.h"

int initUser(User* u)
{
	u->firstName = getStrExactName("Enter your first name:\n");

	u->lastName = getStrExactName("Enter your last name:\n");

	modifyUserName(u);

	u->email = getStrExactName("Enter your e-mail:\n");

	u->city = getStrExactName("Enter your city:\n");

	printf("Car information: \n");
	initCar(&u->theCar);

	printf("Enter your ID:\n\t");
	scanf("%d", &u->id);

	printf("Enter your age:\n\t");
	scanf("%d", &u->age);

	u->tripsArr = NULL;
	u->countTrips = 0;

	u->tripsDates = NULL;

	return 1;
}

void modifyUserName(User* u)
{
	u->userName[0] = u->firstName[0];
	u->userName[1] = '_';
	u->userName[2] = u->lastName[0];
	u->userName[3] = '\0';
}

void printUser(const void* v)
{
	const User* u = *(User**)v;
	SET_PRINT_TO_DIFFERENT_COLOR("\033[1;36m");
	SET_PRINT_UNDERLINE_TEXT();
	printf("User: %s %s\n", u->firstName, u->lastName);
	SET_PRINT_TO_DEFAULT();
	printf("User name: %s\n", u->userName);
	printf("ID: %d\n", u->id);
	printf("Age: %d\n", u->age);
	printf("City: %s\n", u->city);
	printf("E-mail: %s\n", u->email);
	SET_PRINT_UNDERLINE_TEXT();
	printf("Car:\n");
	SET_PRINT_TO_DEFAULT();
	printCar(&u->theCar);
	if (u->countTrips != 0)
	{
		SET_PRINT_UNDERLINE_TEXT();
		printf("Trips:\n");
		SET_PRINT_TO_DEFAULT();
		printTripsArrForUser(u);
	}
	else
	{
		SET_PRINT_TO_DIFFERENT_COLOR("\033[1;31m");
		printf("You haven't plan any trips yet.\n");
		SET_PRINT_TO_DEFAULT();
	}
		
	printf("\n");
}

void printTripsArrForUser(const User* u)
{
	for (int i = 0; i < u->countTrips; i++)
	{
		SET_PRINT_TO_DIFFERENT_COLOR("\033[1;35m");
		printf("(%d)	\n", i + 1);
		SET_PRINT_TO_DEFAULT();
		printDate(&u->tripsDates[i]);
		printTrip(&u->tripsArr[i]);
		printf("\n");
	}
}

void freeUser(User* u)
{
	free(u->firstName);

	free(u->lastName);

	free(u->email);

	free(u->city);

	generalVariableArrayFunction(u->tripsArr, u->countTrips, sizeof(Trip), freeTripPtr);
	free(u->tripsArr);

	free(u->tripsDates);

	freeCar(&u->theCar);

	free(u);
}

void freeUserPtr(void* vU)
{
	User* pU = *(User**)vU;
	freeUser(pU);
}

int writeUserToBinaryFile(const User* user, FILE* fp)
{
	writeStringToFile(user->firstName, fp, "ERROR");

	writeStringToFile(user->lastName, fp, "ERROR");

	writeStringToFile(user->userName, fp, "ERROR");

	writeIntToFile(user->id, fp, "ERROR");

	writeIntToFile(user->age, fp, "ERROR");

	writeStringToFile(user->email, fp, "ERROR");

	writeStringToFile(user->city, fp, "ERROR");

	saveCarToBinaryFileCompressed(&user->theCar, fp);

	writeIntToFile(user->countTrips, fp, "ERROR");

	write_tripsArr_to_b_file(user->tripsArr, user->countTrips, fp);

	writeDatesArrToBinaryFile(user->tripsDates, user->countTrips, fp);

	return 1;
}

int loadUserFromBinaryFile(User* user, FILE* fp)
{

	user->firstName= readStringFromFile(fp, "ERRPR");
	if (!user->firstName)
		return 0;
	
	user->lastName= readStringFromFile(fp, "ERRPR");
	if (!user->lastName)
		return 0;

	int tmp;
	if (!readIntFromFile(&tmp, fp, "ERROR"))
		return 0;

	fread(user->userName, sizeof(char), tmp, fp);

	if (!readIntFromFile(&user->id, fp, "ERROR"))
		return 0;

	if (!readIntFromFile(&user->age, fp, "ERROR"))
		return 0;

	user->email= readStringFromFile(fp, "ERRPR");
	if (!user->email)
		return 0;

	user->city= readStringFromFile(fp, "ERRPR");
	if (!user->city)
		return 0;

	if (!readCarFromBinaryFileCompressed(&user->theCar, fp))
		return 0;

	if (!readIntFromFile(&user->countTrips, fp, "ERROR"))
		return 0;

	createTripsArrForUser(user);

	if (!loadTripsArrFromBFile(user->tripsArr, user->countTrips, fp))
		return 0;

	createDatesArrForUser(user);

	if(!loadDateArrToBinaryFile(user->tripsDates,user->countTrips,fp))
		return 0;


	return 1;
}


int loadUsersArrFromBinaryFile(User** user, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!loadUserFromBinaryFile(user[i], fp))
			return 0;
	}
	return 1;
}

int writeUsersArrToBinaryFile(const User** user,const int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!writeUserToBinaryFile(user[i], fp))
			return 0;
	}
	return 1;
}

int loadUserFromTxtFile( User* user, FILE* fp)
{
	char temp[MAX_STR_LEN];

	myGets(temp, MAX_STR_LEN, fp);
	user->firstName = getDynStr(temp);

	myGets(temp, MAX_STR_LEN, fp);
	user->lastName = getDynStr(temp);

	myGets(user->userName, LEN_USER_NAME+1, fp);

	if (fscanf(fp, "%d", &user->id) != 1)
		return 0;
	fgetc(fp);

	if (fscanf(fp, "%d", &user->age) != 1)
		return 0;
	fgetc(fp);

	myGets(temp, MAX_STR_LEN, fp);
	user->email = getDynStr(temp);

	myGets(temp, MAX_STR_LEN, fp);
	user->city= getDynStr(temp);

	if (!readCarToTxtFile(&user->theCar, fp))
		return 0;

	if (fscanf(fp, "%d", &user->countTrips) != 1)
		return 0;
	fgetc(fp);

	if (!createTripsArrForUser(user))
		return 0;

	if (!loadTripsArrFromTxtFile(user->tripsArr, user->countTrips, fp))
		return 0;

	if (!createDatesArrForUser(user))
		return 0;

	if (!loadDateArrToTxtFile(user->tripsDates, user->countTrips, fp))
		return 0;
	
	return 1;

}

int writeUserToTxtFile(const User* user, FILE* fp)
{
	fprintf(fp, "%s\n", user->firstName);
	fprintf(fp, "%s\n", user->lastName);
	fprintf(fp, "%s\n", user->userName);
	fprintf(fp, "%d\n", user->id);
	fprintf(fp, "%d\n", user->age);
	fprintf(fp, "%s\n", user->email);
	fprintf(fp, "%s\n", user->city);
	if (!saveCarToTxtFile(&user->theCar, fp))
		return 0;
	fprintf(fp, "%d\n", user->countTrips);
	if (!write_tripsArr_to_Txt_file(user->tripsArr, user->countTrips, fp))
		return 0;
	writeDateArrToTxtFile(user->tripsDates, user->countTrips, fp);

	return 1;
}


int loadUsersArrFromTxtFile(User** user, int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!loadUserFromTxtFile(user[i], fp))
			return 0;
	}
	return 1;
}

int writeUsersArrToTxtFile(const User** user, const int count, FILE* fp)
{
	for (int i = 0; i < count; i++)
	{
		if (!writeUserToTxtFile(user[i], fp))
			return 0;
	}
	return 1;
}

int	createTripsArrForUser(User* u)
{
	u->tripsArr = (Trip*)malloc(u->countTrips * sizeof(Trip));
	if (!u->tripsArr)
	{
		printf("Alocation error\n");
		return 0;
	}
	return 1;
}


int	createDatesArrForUser(User* u)
{
	u->tripsDates = (Date*)malloc(u->countTrips * sizeof(Date));
	if (!u->tripsDates)
	{
		printf("Alocation error\n");
		return 0;
	}
	return 1;
}


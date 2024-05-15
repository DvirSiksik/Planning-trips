#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <crtdbg.h>
#include <time.h>
#include "Manager.h"
#include "Forcast.h"
#include "Macros.h"
#include "managerFile.h"

#define EXIT	-1
#define TXT_FILE_NAME "MANAGER TXT FILE .txt"
#define BINARY_FILE_NAME "MANAGER BIN FILE.bin"

int menu();
void updateForecast(Manager* m);
int loginOrSignup(Manager* m);

typedef enum
{
	eShowThisUser, ePrintForecastAAllCities, eAddTrip, eAddHotel, eSwitchUser, ePlanTrip,  ePrintUsers, ePrintTrips, ePrintHotels,
	eSortTrips, eFindTrip, eNofOptions
} eMenuOptions;

const char* str[eNofOptions] = {"Show me my data","Print forecast","Add Trip","Add Hotel","Switch user or add new one",
								"plan a trip", "Print all Users",
								"Print all Trips" ,"Print all Hotels", "Sort Trips" , "Find Trip"};


int menu()
{
	int option;
	printf("\n\n");
	printGeneralEnum("Please choose one of the following options\n", str, eNofOptions);
	printf("%d - Quit\n", EXIT+1);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	printf("\n");
	return option-1;
}


int loginOrSignup(Manager* m)
{
	int option = 0;
	int index = -1;
	do {
		do {
			printf("For login - enter 1. \nFor signup - enter 2.\n\t");
			scanf("%d", &option);
		} while ((option != 1) && (option != 2));

		if (option == 1)
		{
			index = findUserByID(m);
			if (index != -1)
				return index;
			else
			{
				SET_PRINT_TO_DIFFERENT_COLOR("\033[1;31m");
				printf("There is no user with this ID\n");
				SET_PRINT_TO_DEFAULT();
			}
		}

		else
		{
			index = addNewUser(m);
			if (index != -1)
			{
				for (int i = 0; i < m->countUsers; i++)
				{
					if (index == m->allUsers[i]->id)
						return i;
				}
			}
		}
	} while (index == (-1));

	return 0;
}

void updateForecast(Manager* m ) 
{
	time_t currentTime = time(NULL);
	struct tm* localTime = localtime(&currentTime);

	//we want to update our forecast only if the day isnt update, and only when the api already update it - 9am
	
	if (localTime->tm_mday == m->allForecast[0]->date.day || localTime->tm_hour<9)
		return;

	if (!updateForecastAllCites(m))
	{
		printf("--EROR UPDATE FORECAST--");
		return;

	}
	return;
}


int main()
{
	Manager m;

	int optionUpload = 0;
	
	printf("From which source do you want to upload the system?\n");
	
	do {
		printf("TXT file - Press 1\n");
		printf("BINARY file - Press 2\n\t");
		scanf("%d", &optionUpload);
	} while (optionUpload != 1 && optionUpload != 2);
	
	if(optionUpload==1)
		initManagerFromTxtFile(&m, TXT_FILE_NAME);
	else
		initManagerFromFile(&m, BINARY_FILE_NAME);
	
	updateForecast(&m);

	int index=loginOrSignup(&m);

	int stop = 0;
	int option;

	do {
		option = menu();
		switch (option)
		{
		case eShowThisUser:
			printUser(&m.allUsers[index]);
			break;

		case ePrintForecastAAllCities:
			printForecastForCity(m.allForecast);
			break;

		case eAddTrip:
			addNewTrip(&m);
			break;

		case eAddHotel:
			addNewHotel(&m);
			break;

		case eSwitchUser:
			index=loginOrSignup(&m);
			break;

		case ePlanTrip:
			planTripForUser(&m, m.allUsers[index]);
			break;

		case ePrintUsers:
			generalArrayFunction(m.allUsers, m.countUsers, sizeof(User*), printUser);
			break;

		case ePrintTrips:
			generalVariableArrayFunction(m.tripsArr, m.countTrips, sizeof(Trip), printTrip);
			break;

		case ePrintHotels:
			L_print(&m.hotelList,printHotel);
			break;

		case eSortTrips:
			sortTrip(&m);
			break;

		case eFindTrip:
			findTrip(&m);
			break;

		case EXIT:
			printf("Bye bye\n");
			saveManagerToBFile(&m, BINARY_FILE_NAME);
			saveManagerToTxtFile(&m, TXT_FILE_NAME);

			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}


	} while (!stop);

	freeManager(&m);


	_CrtDumpMemoryLeaks();
}
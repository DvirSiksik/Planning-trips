#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Date.h"

#define API_HOST "api.openweathermap.org"
#define API_PATH "/data/2.5/forecast?&units=metric&q="
#define API_KEY "4d1578f9cc74d1eaba5cfd7b256bc694"
#define BUFFER_SIZE 100000

#define NUM_OF_FORECASTS 6
#define LEN_OF_WIND_SPEED 10
#define LEN_OF_WEATHER_DESC 50

typedef struct {
    float max_temp;
    char wind_speed[LEN_OF_WIND_SPEED];
    char weather_desc[LEN_OF_WEATHER_DESC];
    Date date;
} Forecast;


void error(const char* msg);

void parse_forecast(char* json_data, int num_items, Forecast** selected_forecasts, int* num_selected);

int write_forecast_to_file(Forecast* selected_forecasts, FILE* fp);

int write_forecast_to_b_file(Forecast* selected_forecasts, FILE* pf);

int loadForecastFromBFile(Forecast* f, FILE* fp);

int loadForecastFromFile(Forecast* f, FILE* fp);

void getForcast(const char* city, Forecast* selected_forecasts);

int getMaxTemperatureForDay(Forecast* f, Date* d);

char* getWindForDay(Forecast* f, Date* d);

char* getWeatherDescForDay(Forecast* f, Date* d);

void printSelectedForecasts(const Forecast* arrForecasts);

void freeForecast(Forecast* pf);

void freeForecastPtr(void* pV);



#pragma once

#define MIN_TEMP 0
#define MAX_TEMP 50


typedef enum {
	eSunny, eRaining, eWindy, eCloudy, eNofWeather
}eWeatherType;

static const char* weatherType[eNofWeather] =
{ "Sunny", "Raining", "Windy", "Cloudy"};

typedef struct {
	float temperature;
	eWeatherType theType;
}Weather;

int initWeather(Weather* w);

int initWeatherForTrip(Weather* w);

eWeatherType getWeatherType();

void printWeather(const Weather* w);

int  writeWeatherToBFile(const Weather* w, FILE* fp);

int loadWeatherToBFile(Weather* w, FILE* fp);

void writeWeatherToTxtFile(const Weather* w, FILE* fp);

int loadWeatherToTxtFile(Weather* w, FILE* fp);


#define _CRTDBG_MAP_ALLOC
#include "Forcast.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "Macros.h"
#include "General.h"
#include "fileHelper.h"



void error(const char* msg)
{
    perror(msg);
    exit(1);
}

void parse_forecast(char* json_data, int num_items, Forecast** selected_forecasts, int* num_selected)
{
    char* item_start = json_data;
    char prev_date[11] = "";
    Forecast selected;
    selected.max_temp = 0;

    for (int i = 0; i < num_items; i++)
    {
        // Extract date
        if (!item_start)
            break;
        char* date_start = strstr(item_start, "\"dt_txt\":\"");
        if (date_start == NULL) 
            break; // Exit if date not found
        
        date_start += strlen("\"dt_txt\":\"");
        char* date_end = strchr(date_start, ' ');
        if (date_end == NULL) 
            break; // Exit if date end not found
        
        char date[11];
        strncpy(date, date_start, date_end - date_start);
        date[date_end - date_start] = '\0'; // Null-terminate the date

        // Extract relevant data
        char* max_temp = strstr(item_start, "\"temp_max\":");
        if (max_temp == NULL) 
            break; // Exit if max temperature not found
        
        max_temp += strlen("\"temp_max\":");
        float max_temp_val;
        sscanf(max_temp, "%f", &max_temp_val);

        char* wind_speed = strstr(item_start, "\"speed\":");
        if (wind_speed == NULL) 
            break; // Exit if wind speed not found
        
        wind_speed += strlen("\"speed\":");
        char* endptr_speed = strchr(wind_speed, ',');
        if (endptr_speed == NULL) 
            break; // Exit if wind speed end not found
        
        char wind_speed_str[10];
        strncpy(wind_speed_str, wind_speed, endptr_speed - wind_speed);
        wind_speed_str[endptr_speed - wind_speed] = '\0'; // Null-terminate the wind speed

        char* weather_desc = strstr(item_start, "\"description\":\"");
        if (weather_desc == NULL) 
            break; // Exit if weather description not found
        
        weather_desc += strlen("\"description\":\"");
        char* endptr_desc = strchr(weather_desc, '"');
        if (endptr_desc == NULL) 
            break; // Exit if weather description end not found
        
        char weather_desc_str[50];
        strncpy(weather_desc_str, weather_desc, endptr_desc - weather_desc);
        weather_desc_str[endptr_desc - weather_desc] = '\0'; // Null-terminate the weather description

        // Update selected forecast if it's the first for the day or if the temperature is higher
        if (strcmp(date, prev_date) != 0)
        {
            checkDate(date, &selected.date);
            selected.max_temp = max_temp_val;
            strcpy(selected.wind_speed, wind_speed_str);
            strcpy(selected.weather_desc, weather_desc_str);
            strcpy(prev_date, date);

            *num_selected += 1;
         
            (*selected_forecasts)[*num_selected - 1] = selected;
        }
   
        else if (max_temp_val > selected.max_temp)
        {
            selected.max_temp = max_temp_val;
            strcpy(selected.wind_speed, wind_speed_str);
            strcpy(selected.weather_desc, weather_desc_str);
            (*selected_forecasts)[*num_selected - 1] = selected;
        }

        // Move to the next item's data
        item_start = strstr(item_start + 1, "\"dt_txt\":\"");
    }
}

void getForcast(const char* city, Forecast* selected_forecasts)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup failed\n");
        return;
    }

    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent* server;

    portno = 80; // HTTP port

    // Create socket
    sockfd = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Get server by hostname
    server = gethostbyname(API_HOST);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR: No such host\n");
        exit(1);
    }

    // Initialize server address struct
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // Build HTTP request
    char* request = malloc(1024 * sizeof(char));
    if (request == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    snprintf(request, 1024, "GET %s%s&appid=%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
        API_PATH, city, API_KEY, API_HOST);

    // Send HTTP request
    n = send(sockfd, request, (int)strlen(request), 0);
    if (n < 0)
        error("ERROR writing to socket");

    // Read HTTP response
    char* buffer = malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(request); // Free previously allocated memory
        return;
    }

    memset(buffer, 0, BUFFER_SIZE * sizeof(char));
    int total_bytes_read = 0;
    while ((n = recv(sockfd, buffer + total_bytes_read, BUFFER_SIZE - total_bytes_read, 0)) > 0)
    {
        total_bytes_read += n;

        if (total_bytes_read >= BUFFER_SIZE - 1)
        {
            fprintf(stderr, "Buffer size exceeded\n");
            break;
        }
    }

    if (n < 0)
        error("ERROR reading from socket");

    // Close socket
    closesocket(sockfd);
    WSACleanup();

    // Parse JSON data for forecast
    int num_selected = 0;
    parse_forecast(buffer, total_bytes_read, &selected_forecasts, &num_selected);

    // Free dynamically allocated memory
    free(request);
    free(buffer);

    return;
}





int getMaxTemperatureForDay(Forecast* f, Date* d)
{
    int locationInArray = d->day-f[0].date.day;

    return (int)(f[locationInArray].max_temp);
}

char* getWindForDay(Forecast* f, Date* d)
{
    int locationInArray = d->day - f[0].date.day;

    return (f[locationInArray].wind_speed);
}

char* getWeatherDescForDay(Forecast* f, Date* d)
{
    int locationInArray = d->day - f[0].date.day;

    return (f[locationInArray].weather_desc);
}

void printSelectedForecasts(const Forecast* arrForecasts)
{
    for (int i = 0; i < NUM_OF_FORECASTS; i++)
    {
        printDate(&arrForecasts[i].date);
        printf("Max Temperature: %.2f°C\n", arrForecasts[i].max_temp);
        printf("Wind Speed: %s m/s\n", arrForecasts[i].wind_speed);
        printf("Weather Description: %s\n\n", arrForecasts[i].weather_desc);
    }
}

int write_forecast_to_file(Forecast* selected_forecasts, FILE* fp)
{
    for (int i = 0; i < NUM_OF_FORECASTS; i++)
    {
        writeDateToFile(&selected_forecasts[i].date, fp);
        fprintf(fp, "%.2f\n", selected_forecasts[i].max_temp);
        fprintf(fp, "%s\n", selected_forecasts[i].wind_speed);
        fprintf(fp, "%s\n", selected_forecasts[i].weather_desc);
    }
    return 1;
}

int write_forecast_to_b_file(Forecast* selected_forecasts, FILE* fp)
{
    int tmp=0;
    for (int i = 0; i < NUM_OF_FORECASTS; i++)
    {
        if (!writeDateToBinaryFile(&selected_forecasts[i].date, fp))
            return 0;

        if (!writeFloatToFile(selected_forecasts[i].max_temp, fp, "ERROR"))
            return 0;

        if (!writeStringToFile(selected_forecasts[i].wind_speed, fp, "ERROR"))
            return 0;

        if (!writeStringToFile(selected_forecasts[i].weather_desc, fp, "ERROR"))
            return 0;
    }
    return 1;
}

int loadForecastFromBFile(Forecast* f, FILE* fp)
{ 
    int tmp;
    
    for (int i = 0; i < NUM_OF_FORECASTS; i++)
    {
        if (!loadDateFromBinaryFile(&f[i].date, fp))
            return 0;
    
        if(!readFloatFromFile(&f[i].max_temp, fp, "ERROR"))
             return 0;

        if (!readIntFromFile(&tmp, fp, "ERROR"))
            return 0;
     
        fread(f[i].wind_speed, sizeof(char), tmp, fp);

        if (!readIntFromFile(&tmp, fp, "ERROR"))
            return 0;
       

        fread(f[i].weather_desc, sizeof(char), tmp, fp);
    }
    return 1;
}

int loadForecastFromFile(Forecast* f, FILE* fp)
{
    for (int i = 0; i < NUM_OF_FORECASTS; i++)
    {
        readDateFromFile(&f[i].date, fp);

        if (fscanf(fp, "%f", &f[i].max_temp) != 1)
            return 0;
        fgetc(fp);

        myGets(f[i].wind_speed, LEN_OF_WIND_SPEED, fp);

        myGets(f[i].weather_desc, LEN_OF_WEATHER_DESC, fp);
    }
        return 1;
    
}

void freeForecast(Forecast* pf) 
{
   free(pf);
}

void freeForecastPtr(void* pV)
{
     Forecast* pF = *(Forecast**)pV;
    free(pF);
}



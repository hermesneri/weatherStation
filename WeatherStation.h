/*************************************************
*                WEATHER STATION                 *
autor: Hermes Neri -  http://github.com/hermesneri
download: https://github.com/hermes/weatherstation
version: 1.0.0 - December, 2023
________________________________________________*/

#ifndef WeatherStation_h
#define WeatherStation_h

#include <HTTPClient.h>
#include <ArduinoJson.h>

class WeatherStation
{
public:
    bool begin(const char *latitude_json, const char *longitude_json, const char *openWeatherMapApiKey_json);

    String update();
    int getClouds();
    String getCity();
    int getHumidity();
    int getPressure();    
    int getTimezone();
    float getTempMin();
    float getTempMax();
    String getCountry();
    String getPayload();
    int getVisibility();    
    float getWindSpeed();    
    float getTemperature();
    int getWindDirection();
    String getWeatherInfo();
    String getDescription();
    float getThermalSensation();

    //Variables
    int timezone=0;
    int humidity=0;
    const char *city;
    float temp_min=0;
    float temp_max=0;
    float pressure=0;
    int clouds_all=0;
    float latitude=0;
    float longitude=0;
    float sea_level=0;
    float wind_speed=0;
    float feels_like=0;
    float grnd_level=0;
    float visibility=0;
    float temperature=0;
    String payload = ""; 
    const char *country;
    unsigned long sunset;
    String weatherinfo="";
    unsigned long sunrise;
    float wind_direction=0;
    const char *description;

private:
    String lat;
    String lon;
    String key;
    void organizeJson(String payload);
    String RequestPayload(String lat, String lon, String key);
};

#endif

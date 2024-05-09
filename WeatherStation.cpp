/*************************************************
*                WEATHER STATION                 *
autor: Hermes Neri -  http://github.com/hermesneri
download: https://github.com/hermes/weatherstation
version: 1.0.0 - December, 2023
________________________________________________*/

#include <WeatherStation.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <math.h>

// #define DEBUG_MODE //uncomment to print debug messages
#define DEBUG_ERROR //uncomment to print

#ifdef DEBUG_MODE
#define LOG(message) Serial.println(message)
#else
#define LOG(message)
#endif

#ifdef DEBUG_ERROR
#define ERROR(message) Serial.println(message)
#else
#define ERROR(message)
#endif

/**
 * @brief Initializes the weather station with geographical coordinates and OpenWeatherMap API key.
 *
 * @param latitude Latitude of the location.
 * @param longitude Longitude of the location.
 * @param weatherMapKey OpenWeatherMap API key.
 * @return Returns true if coordinates and key are set up correctly; false otherwise.
 */
bool WeatherStation::begin(const char *latitude_json, const char *longitude_json, const char *weatherMapKey_json)
{
    lat = latitude_json;
    lon = longitude_json;
    key = weatherMapKey_json;

    // RequestPayload(lat, lon, key);

    // if (lat.length() > 0 && lon.length() > 0 && key.length() > 0 && RequestPayload(lat, lon, key).length() > 0)
    if (RequestPayload(lat, lon, key).length() > 0)
    {
        LOG("[WeatherStation.h] - initialized successfully.");
        return true;
    }
    else
    {
        ERROR("[WeatherStation.h] - initialization failed. Invalid coordinates or API key.");
        return false;
    }
}

/**
 * @brief Makes the request to the OpenWeatherMap API and retrieves the payload of the response.
 *
 * @return Payload of the API response.
 */
String WeatherStation::update()
{
    RequestPayload(lat, lon, key);
    return payload;
}

/**
 * @brief Makes the request of the last payload saved.
 *
 * @return Payload of the API response.
 */
String WeatherStation::getPayload()
{
    organizeJson(payload);
    return payload;
}

/**
 * @brief Makes the request to the OpenWeatherMap API and retrieves the payload of the response.
 *
 * @param lat Latitude of the location.
 * @param lon Longitude of the location.
 * @param key OpenWeatherMap API key.
 * @return Payload of the API response.
 */
String WeatherStation::RequestPayload(String lat, String lon, String key)
{
    const String url = "https://api.openweathermap.org/data/2.5/weather?lat=" + String(lat) + "&lon=" + String(lon) + "&appid=" + String(key);
    
    HTTPClient http;

    http.begin(url);
    delay(10);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0)
    {
        payload = http.getString();
        if(payload.length() > 0)
        {    
            LOG("[WeatherStation.h] - API Response received:");
            LOG(payload);
        }
        else
        {
            ERROR("[WeatherStation.h] - API Response failed");  
        }
        organizeJson(payload);
        return payload;
    }
    else
    {
        ERROR("HTTP ERROR! Code error: " + String(httpResponseCode));
    }

    http.end();
    return "";
}

/**
 * @brief Organizes the JSON data received from the OpenWeatherMap API.
 *
 * @param payload JSON data from the OpenWeatherMap API.
 */
void WeatherStation::organizeJson(String payload)
{
    DynamicJsonDocument jsonDoc(1024);
    DeserializationError error = deserializeJson(jsonDoc, payload);

    if (error)
    {
        Serial.print("Error when parsing JSON: ");
        Serial.println(error.c_str());
    }
    else
    {           
        longitude = jsonDoc["coord"]["lon"];
        latitude = jsonDoc["coord"]["lat"];

        description = jsonDoc["weather"][0]["description"];

        temperature = jsonDoc["main"]["temp"];
        feels_like = jsonDoc["main"]["feels_like"];
        temp_min = jsonDoc["main"]["temp_min"];
        temp_max = jsonDoc["main"]["temp_max"];
        pressure = jsonDoc["main"]["pressure"];
        humidity = jsonDoc["main"]["humidity"];

        visibility = jsonDoc["visibility"];

        wind_speed = jsonDoc["wind"]["speed"];
        wind_direction = jsonDoc["wind"]["deg"];

        clouds_all = jsonDoc["clouds"]["all"];

        country = jsonDoc["sys"]["country"];
        sunrise = jsonDoc["sys"]["sunrise"];
        sunset = jsonDoc["sys"]["sunset"];

        timezone = jsonDoc["timezone"];

        city = jsonDoc["name"];        
    }
}

/**
 * @brief Gets the weather description.
 *
 * @return return the weather description.
 */
String WeatherStation::getDescription()
 {
    return description;
 }

/**
 * @brief Gets the temperature in degrees Celsius.
 *
 * @return Temperature in degrees Celsius.
 */
float WeatherStation::getTemperature()
{
    if(temperature == 0)
    {
        return 0;
    }
    else
    {
        return temperature / 10;
    }
}

/**
 * @brief Gets the thermal sensation in degrees Celsius.
 *
 * @return Thermal sensation in degrees Celsius.
 */
float WeatherStation::getThermalSensation()
{
    if(feels_like == 0)
    {
        return 0;
    }
    else
    {
        return feels_like / 10;
    }
}

/**
 * @brief Gets the minimum temperature in degrees Celsius.
 *
 * @return Minimum temperature in degrees Celsius.
 */
float WeatherStation::getTempMin()
{
    if(temp_min == 0)
    {
        return 0;
    }
    else
    {
        return temp_min / 10;
    }
}

/**
 * @brief Gets the maximum temperature in degrees Celsius.
 *
 * @return Maximum temperature in degrees Celsius.
 */
float WeatherStation::getTempMax()
{
    if(temp_max == 0)
    {
        return 0;
    }
    else
    {
        return temp_max / 10;
    }
}

/**
 * @brief Gets the atmospheric pressure.
 *
 * @return Atmospheric pressure.
 */
int WeatherStation::getPressure()
{
    return pressure;
}

/**
 * @brief Gets the humidity.
 *
 * @return Humidity.
 */
int WeatherStation::getHumidity()
{
    return humidity;
}

/**
 * @brief Gets the visibility.
 *
 * @return Visibility.
 */
int WeatherStation::getVisibility()
{
    return visibility;
}

/**
 * @brief Gets the wind speed.
 *
 * @return Wind speed.
 */
float WeatherStation::getWindSpeed()
{
    return wind_speed;
}

/**
 * @brief Gets the wind direction.
 *
 * @return Wind direction.
 */
int WeatherStation::getWindDirection()
{
    return wind_direction;
}

/**
 * @brief Gets the cloud percentage.
 *
 * @return Cloud percentage.
 */
int WeatherStation::getClouds()
{
    return clouds_all;
}

/**
 * @brief Gets the country code.
 *
 * @return Country code.
 */
String WeatherStation::getCountry()
{
    return country;
}

//TODO: Sunrise

//TODO: Sunset

/**
 * @brief Gets the weather information.
 *
 * @return temperature, humidity and pressure.
 */
String WeatherStation::getWeatherInfo() 
{
    weatherinfo = "Temperature:" + String(getTemperature()) + "°C" + " | " +
                  "Humidity:" + String(getHumidity()) + "%" + " | " +
                  "Pressure:" + String(getPressure()) + "hPa";

    return weatherinfo;
}

/**
 * @brief Gets the timezone.
 *
 * @return Timezone.
 */
int WeatherStation::getTimezone()
{
    return 0;
}

/**
 * @brief Gets the city name.
 *
 * @return City name.
 */
String WeatherStation::getCity()
{
    return city;
}

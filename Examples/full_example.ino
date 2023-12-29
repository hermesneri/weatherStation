#include <WeatherStation.h>
#include <WiFi.h>

WeatherStation WStation;

const char *weatherMapKey = ""; // your weatherKey here
const char *latitude = "";      // latitude
const char *longitude = "";     // longitude

const char *ssid = "";     // your ssid here
const char *password = ""; // you password here

bool WSSTATUS = false;

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    Serial.printf("\nConnecting to %s .", ssid);

    u_int8_t try_connect = 0;

    // Starting wifi connection
    while (WiFi.status() != WL_CONNECTED && try_connect < 10)
    {
        delay(500);
        Serial.print(".");
        try_connect++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.printf("\nConnected to %s \n", ssid);
    }
    else
    {
        Serial.printf("\nError to connect at %s \n", ssid);
    }

    // Starting Weather Station
    if (!WStation.begin(latitude, longitude, weatherMapKey))
    {
        Serial.println("WStation ERROR");
        WSSTATUS = false;
    }
    else
    {
        Serial.println("WStation START successfully");
        WSSTATUS = true;
    }
}

void loop()
{
    if (WSSTATUS == true) //if the WeatherStation start
    {
        Serial.println("\n____Meteorological Data_____");

        Serial.print("Weather info: ");
        Serial.println(WStation.getWeatherInfo());

        Serial.println("----------------------------");

        Serial.print("Description: ");
        Serial.println(WStation.getDescription());

        Serial.print("City: ");
        Serial.println(WStation.getCity());

        Serial.print("Temperature:");
        Serial.print(WStation.getTemperature());
        Serial.println("°C");

        Serial.print("Temp Min: ");
        Serial.print(WStation.getTempMin());
        Serial.println("°C");

        Serial.print("Temp Max: ");
        Serial.print(WStation.getTempMax());
        Serial.println("°C");

        Serial.print("Thermal Sensation: ");
        Serial.print(WStation.getThermalSensation());
        Serial.println("°C");

        Serial.print("Humidity: ");
        Serial.print(WStation.getHumidity());
        Serial.println("%");

        Serial.print("Pressure: ");
        Serial.print(WStation.getPressure());
        Serial.println(" hPa");

        Serial.print("Wind Speed: ");
        Serial.print(WStation.getWindSpeed());
        Serial.println(" km/h");

        Serial.print("Wind Direction: ");
        Serial.print(WStation.getWindDirection());
        Serial.println("°");

        Serial.print("Visibility: ");
        Serial.print(WStation.getVisibility());
        Serial.println(" m");

        Serial.println("----------------------------");

        Serial.print("Payload: ");
        Serial.println(WStation.getPayload());

        Serial.println("____________________________");

        static uint8_t counter;
        counter++;

        if (counter > 10) //after 10 times, update the payload
        {
            counter = 0;
            Serial.print("Updating Payload...");
            Serial.println(WStation.update());
        }
    }
    delay(5000); // wait 5 seconds before next requisition
}
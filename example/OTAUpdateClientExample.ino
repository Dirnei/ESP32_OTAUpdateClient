#include <WiFiClient.h>;
#include "OTAUpdateClient.h";

WiFiClient wifiClient;
OTAUpdateClient updateClient(wifiClient, "update.yourserver.com", 80);

void setup()
{
    int counter = 0;
    Serial.begin(115200);

    WiFi.begin();

    Serial.println("Connect to WIFI");
    while (WiFi.status() != WL_CONNECTED) 
    {      
        if (counter > 30)
        { 
            Serial.println("Restarting...");
            ESP.restart();
        }
        
        delay(500);
    }

    if (!updateClient.isUpdateAvailable("/version.txt", 0))
    {
        Serial.println("No update available");
    }
    else
    {
        Serial.println("Updating...");
        updateClient.update("/yourBinary.bin");
    }
}

void loop()
{
    Serial.println("...");
    delay(3000);
}
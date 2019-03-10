#include "OTAUpdateClient.h";

OTAUpdateClient::OTAUpdateClient(WiFiClient client, const char * host, int16_t port)
{
    this->wifiClient = client;
    this->host = host;
    this->port = port;
}


bool OTAUpdateClient::isUpdateAvailable(const char * versionPath, int16_t currentVersion)
{
    Serial.println("Checking for updates.... ");
    httpClient.begin("http://" + String(host) + versionPath);
    Serial.print("http://");
    Serial.print(host);
    Serial.println(versionPath);
    int code = httpClient.GET();
    if (code == 200)
    {
        int newVersion = httpClient.getString().toInt();
        httpClient.end();
        if (newVersion > currentVersion)
        {
            return true;
        }
    }
    else
    {
        httpClient.end();
        Serial.println("Something went wrong while requesting new version number");
        Serial.print("HTTP Code: ");
        Serial.println(code);
    }

    return false;
}

void OTAUpdateClient::update(const char * filePath)
{
    bool isValidContentType = false;
    int contentLength = 0;

    Serial.print("Connecting to: ");
    Serial.println(host);
    if (wifiClient.connect(host, port)) 
    {
        Serial.println("Fetching Bin: " + String(filePath));
        wifiClient.print(String("GET ") + filePath + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "Cache-Control: no-cache\r\n" +
        "Connection: close\r\n\r\n");

        unsigned long timeout = millis();
        while (wifiClient.available() == 0) 
        {
            if (millis() - timeout > 5000) 
            {
                Serial.println(">>> Client Timeout !");
                wifiClient.stop();
                return;
            }
        }

        while (wifiClient.available())
        {
            String line = wifiClient.readStringUntil('\n');
            line.trim();
            Serial.println(line);

            if (!line.length()) 
            {
                break;
            }

            if (line.startsWith("HTTP/1.1")) 
            {
                if (line.indexOf("200") < 0) 
                {
                    Serial.println("Invalid HTTP status code from server. Exiting OTA Update.");
                    break;
                }
            }

            if (line.startsWith("Content-Length: ")) 
            {
                contentLength = atoi((getHeaderValue(line, "Content-Length: ")).c_str());
            }

            if (line.startsWith("Content-Type: ")) 
            {
                String contentType = getHeaderValue(line, "Content-Type: ");
                isValidContentType = contentType == "application/octet-stream" 
                                    || contentType == "application/macbinary";
            }
        }
    }
    else
    {
        Serial.println("Can't connect to the update server...");
        return;
    }
  
    if (contentLength && isValidContentType) 
    {
        Serial.println("Begin OTA update. This may take a while...");
        if (Update.begin(contentLength)) 
        {
            size_t written = Update.writeStream(wifiClient);
            if (Update.end()) 
            {
                Serial.println("OTA done!");
                if (Update.isFinished()) 
                {
                    Serial.println("Reboot...");
                    ESP.restart();
                }
            } 
        } 
        else 
        {
            Serial.println("Not enough space available.");
        }
    } 
    else 
    {
        Serial.println("There was no content in the response");
    }

    wifiClient.flush();
}


String OTAUpdateClient::getHeaderValue(String header, char * headerName) {
  return header.substring(strlen(headerName));
}
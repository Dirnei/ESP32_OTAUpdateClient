#ifndef _OTAUPDATECLIENT_
#define _OTAUPDATECLIENT_

#include <Arduino.h>
#include <Update.h>;
#include <WiFiClient.h>;
#include <HTTPClient.h>

class OTAUpdateClient {
    public:
        OTAUpdateClient(WiFiClient client, const char * host, int16_t port);

        bool isUpdateAvailable(const char * filePath, int16_t currentVersion);
        void update(const char * filePath);
    private:
        const char * host;
        int16_t port;
        WiFiClient wifiClient;
        HTTPClient httpClient;

        String getHeaderValue(String header, char * headerName);
};
#endif // _OTAUPDATECLIENT_
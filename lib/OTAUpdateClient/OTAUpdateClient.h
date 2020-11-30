#ifndef _OTAUPDATECLIENT_
#define _OTAUPDATECLIENT_

#include <Arduino.h>
#include <Update.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#ifdef OTA_VERSION_JSON
#include <ArduinoJson.h>
#endif

class OTAUpdateClient
{
  public:
    OTAUpdateClient(WiFiClient client, const char *host, int16_t port);

    void setLocalVersion(uint32_t version);
    uint32_t getLocalVersion(void);
    bool isUpdateAvailable(const char *filePath);
    void update(const char *filePath);

  private:
    const char *host;
    int16_t port;
    uint32_t _availableVersion;
    WiFiClient wifiClient;
    HTTPClient httpClient;

    String getHeaderValue(String header, const char *headerName);
};
#endif // _OTAUPDATECLIENT_
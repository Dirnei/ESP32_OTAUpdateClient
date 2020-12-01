# OTAUpdateClient for the ESP32

OTA only makes sense to me when used with a webserver, because only then you really profit of this feature. And because of this I started this lib to encapsulate the code that is always the same. You only need a webserver where you host a textfile, or a json file, for the current availbe version and your compiled binary you wan't to deploy on your devices.

## How it works

The client compares a `integer` value from a file on a webserver with a local integer value stored in the `NVS`. If the local value is smaller then the remote, it will detect an update. It will download the new firmware from the webserver and install it. If it was all succesfull it reboots with the new firmware.

## Usage

The easiest thing is to have a look at the `main.cpp` in the `src` folder. But you can also follow the description here.

First you have to include the library header and create a new instance of the `OTAUpdateClient` class. It does need a `WiFiClient`, the `address` to your webserver and the `port` your webserver is listening on.

> Only the base address is needed. The relative file path is specified alter on.

```cpp
#include <OTAUpdateClient.h>
WiFiClient wifiClient;
OTAUpdateClient updateClient(wifiClient, "update.yourserver.com", 80);
```

After that is done, you only need to check somewhere if a new update is available. This is done by calling the method `isUpdateAvailable(pathToVersionFile)`. It needs the relative path to your version file beginning with a `/`. It will compare the value from the version file to the current version stored in `NVS`. If you run it the first time the value will be `0`. You can override the version with `setVersion(version)`. If the remote version is greater, a new update is available.

> In this documentation the full path would be: http://update.yourserver.com/version.txt

```cpp
if (!updateClient.isUpdateAvailable("/version.txt"))
{
    Serial.println("No update available");
}
else
{
    Serial.println("Updating...");
    updateClient.update("/firmware.bin");
}
```

If an update is available you can call `update(pathToFirmware)` to start the OTA update procedure. When the Update was successfull it will restart right after completion.

## Webserver

To use it you need a running webserver, that hosts the binary file and a version file. You can choose between a simple `text` and a `json` file. In both cases it is relatively simple and only contains a version number that is a simple `integer`.

### Textfile (default)

```
6
```


### JSON

```json
{ "version": 6 }
```

#### How to use json

If you want to use the json file you have to define `OTA_VERSION_JSON`. Only then it will include the reference to `ArduinoJson` and parse the file as a json. If this is not defined, the version file will always be interpreted as an `integer` number.

If you are using **PlatformIO**, you can add the following to your `platformio.ini`:

```
build_flags = -D OTA_VERSION_JSON
```

This will tell the compiler to define the `OTA_VERSION_JSON` and compile the OTAUpdateclient with the json parser.
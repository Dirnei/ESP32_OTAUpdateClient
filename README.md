# ESP32 OTAUpdateClient

OTA only makes sense to me when used with a webserver, because only then you really profit of this feature. And because of this i started this lib to encapsulate the code that is always the same. You only need a webserver where you host a textfile for the current availbe version and your compiled binary you wan't to deploy on your devices.

The textfile contains only an integer with the current version. If this version is higher than you specified in ``` client.isUpdateAvailable("/version.txt", 1); ``` it will return **true** and than you can call ``` client.update("/yourBinary.bin"); ``` to start the download. 
If the download was successfull the Update will start and after that the MC will reboot.

Right now it supports only the ESP32.

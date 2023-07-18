#include <EEPROM.h>

#if defined(ARDUINO_ARCH_ESP32)

#include <WiFi.h>
#include <WebServer.h>

#elif defined(ARDUINO_ARCH_ESP8266)

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#else

#pragma message ("Unknown Device!")

#endif

#ifndef FRONTEND_H
#define FRONTEND_H

class Frontend {
  private:
    // define server depending on device
    #if defined(ARDUINO_ARCH_ESP32)
    WebServer server;
    #elif defined(ARDUINO_ARCH_ESP8266)
    ESP8266WebServer server;
    #endif

    String HTML();
    void handleOnConnect();
    void handleSetCoords();
    void handleNumDrones();
    void startSpoof();
    void handleNotFound();
    unsigned long maxtime = 0.0;
    unsigned long timer = 0.0;

    const int latitude_addr = 100;
    const int longitude_addr = 110;
    const int num_drones_addr = 120;

  public:
    Frontend(unsigned long idletime);
    void handleClient();
    bool do_spoof = false;
    double latitude = 52.439100;
    double longitude = -1.503900;
    int num_drones = 16;
};

#endif
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#ifndef FRONTEND_H
#define FRONTEND_H

class Frontend {
  private:
    ESP8266WebServer server;
    String HTML();
    void handleOnConnect();
    void handleForm();
    void startSpoof();
    void handleNotFound();
    unsigned long maxtime = 0.0;
    unsigned long timer = 0.0;

  public:
    Frontend(unsigned long idletime);
    void handleClient();
    bool do_spoof = false;
    double latitude = 52.439100;
    double longitude = -1.503900;
};

#endif
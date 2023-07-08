#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#ifndef FRONTEND_H
#define FRONTEND_H

class Frontend {
  private:
    ESP8266WebServer server;
    String HTML();
    void handleOnConnect();
    void handleNotFound();
    void startSpoof();
    void handleForm();

  public:
    Frontend();
    void handleClient();
    bool do_spoof = false;
    double latitude = 52.439100;
    double longitude = -1.503900;
};

#endif
// ESP8266 RemoteID spoofer

#include "frontend.h"
#include "spoofer.h"

// for some reason I can't get vector to work so... this hacky thing for now...
int num_spoofers = 0;
Spoofer spoofers[16];

void setup() {
  Serial.begin(115200);
  
  // start the frontend and don't exit until either timer elapse or user ends it
  // the timer here is set at 2 minutes
  Frontend frontend(120000);
  while (!frontend.do_spoof) {
    frontend.handleClient();
  }

  // instantiate the spoofers and change locations
  Serial.println("Starting Spoofers");
  num_spoofers = frontend.num_drones;
  for (int i = 0; i < num_spoofers; i++) {
    spoofers[i].init();
    spoofers[i].updateLocation(frontend.latitude, frontend.longitude);
  }
}

void loop() {
  // do the spoofing
  for (int i = 0; i < num_spoofers; i++) {
    spoofers[i].update();
    delay(200 / num_spoofers);
  }
}

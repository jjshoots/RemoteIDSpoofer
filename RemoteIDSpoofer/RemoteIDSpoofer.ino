// ESP8266 RemoteID spoofer

#include "frontend.h"
#include "spoofer.h"

const int num_spoofers = 16;
Spoofer spoofers[num_spoofers];

void setup() {
  Serial.begin(115200);
  
  // start the frontend and don't exit until either timer elapse or user ends it
  // the timer here is set at 2 minutes
  unsigned long timer = millis();
  Frontend frontend;
  while (!frontend.do_spoof && (millis() - timer < 120000)) {
    frontend.handleClient();
  }

  // instantiate the spoofers and change locations
  Serial.println("Starting Spoofers");
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

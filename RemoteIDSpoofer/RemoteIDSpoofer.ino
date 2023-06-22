// ESP8266 RemoteID spoofer
// Heavily adapted from https://github.com/sxjack/uav_electronic_ids

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "spoofer.h"

// we are limited by how fast we can broadcast things
const int num_spoofers = 16;
static Spoofer spoofers[num_spoofers];

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < num_spoofers; i++) {
    spoofers[i].update();
    delay(500 / num_spoofers);
  }
}

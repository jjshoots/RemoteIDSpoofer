#include <Arduino.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <Esp.h>

#include "id_open.h"

#ifndef SPOOFER_H
#define SPOOFER_H

class Spoofer {
  private:
    ID_OpenDrone squitter;
    UTM_Utilities utm_utils;

    struct UTM_parameters utm_parameters;
    struct UTM_data utm_data;

    int speed_kn = (int) (rand() % 10);
    float x = 0.0, y = 0.0, z = 0.0;
    float speed_m_x, max_dir_change = 75.0;
    double deg2rad = (4.0 * atan(1.0)) / 180.0;
    double m_deg_lat = 0.0, m_deg_long = 0.0;

    double lat_d, long_d;
    time_t time_2;
    struct tm clock_tm;
    struct timeval tv = {0,0};
    struct timezone utc = {0,0};

    uint32_t last_update = 0;

    // random ID generator
    String getID();

  public:
    Spoofer();
    void update();
};

#endif
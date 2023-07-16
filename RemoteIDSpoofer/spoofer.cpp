#include "spoofer.h"

void Spoofer::init() {
  // time things
  memset(&clock_tm,0,sizeof(struct tm));
  clock_tm.tm_hour  =  10;
  clock_tm.tm_mday  =  16;
  clock_tm.tm_mon   =  11;
  clock_tm.tm_year  = 122;
  tv.tv_sec =
  time_2 = mktime(&clock_tm);
  settimeofday(&tv,&utc);

  // utm things
  memset(&utm_parameters,0,sizeof(utm_parameters));
  strcpy(utm_parameters.UAS_operator, getID().c_str());
  utm_parameters.region      = 1;
  utm_parameters.EU_category = 1;
  utm_parameters.EU_class    = 5;
  squitter.init(&utm_parameters);
  memset(&utm_data,0,sizeof(utm_data));
  Serial.println("inited");
}

void Spoofer::updateLocation(float latitude, float longitude) {
  // define location plus some noise
  lat_d =
  utm_data.latitude_d =
  utm_data.base_latitude = latitude + (float) (rand() % 10 - 5) / 10000.0;
  long_d =
  utm_data.longitude_d =
  utm_data.base_longitude = longitude + (float) (rand() % 10 - 5) / 10000.0;
  utm_data.base_alt_m = (float) (rand() % 1000) / 10.0;

  utm_data.base_valid = 1;
  utm_utils.calc_m_per_deg(lat_d,&m_deg_lat,&m_deg_long);
}

void Spoofer::update() {
  // FAA says minimum rate is 1 Hz, we do 2 Hz here
  if ((millis() - last_update) < 200) {
    return;
  }

  // number of satellites
  utm_data.satellites = rand() % 8 + 8;

  // randomly update the velocity
  utm_data.speed_kn = constrain(utm_data.speed_kn + (rand() % 5) - 2, 1, 20);
  speed_m_x = ((float) utm_data.speed_kn) * 0.514444 * 0.2; // Because we update every 200 ms.

  // randomly pick a direction to head
  float ranf = (float) (rand() % 1000 - 500) / 1000.0;
  int dir_change = (int) (max_dir_change * ranf);
  utm_data.heading = (utm_data.heading + dir_change + 360) % 360;

  // calculate the new x, y
  float rads = (deg2rad * (float) utm_data.heading);
  x += speed_m_x * sin(rads);
  y += speed_m_x * cos(rads);

  // calculate new height
  float climbrate = (float) (rand() % 200 - 100) / 10.0;
  z = constrain(z + climbrate, 10.0, 300.0);
  utm_data.alt_msl_m = utm_data.base_alt_m + z;
  utm_data.alt_agl_m = z;

  // update the x, y 
  utm_data.latitude_d  = utm_data.base_latitude  + (y / m_deg_lat);
  utm_data.longitude_d = utm_data.base_longitude + (x / m_deg_long);

  // transmit things
  squitter.transmit(&utm_data);
  last_update = millis();
}

String Spoofer::getID() {
  String characters = String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
  String ID = "";
  for (int i = 0; i < 16; i++)
  {
    ID.concat(characters[(rand() % characters.length())]);
  }
  return ID;
}

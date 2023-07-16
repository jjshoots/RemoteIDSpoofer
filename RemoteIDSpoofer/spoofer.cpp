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
  double lat_d =
  utm_data.latitude_d =
  utm_data.base_latitude = latitude + (float) (rand() % 10 - 5) / 10000.0;

  double long_d =
  utm_data.longitude_d =
  utm_data.base_longitude = longitude + (float) (rand() % 10 - 5) / 10000.0;

  utm_data.base_valid = 1;
  utm_data.base_alt_m = (float) (rand() % 1000) / 10.0;

  utm_utils.calc_m_per_deg(lat_d,&m_deg_lat,&m_deg_long);
}

void Spoofer::update() {
  // FAA says minimum rate is 1 Hz, we do 2 Hz here
  if ((millis() - last_update) < 200) {
    return;
  }

  // update time calculations
  double time_elapsed_secs = double(millis() - last_update) / 1000.0;
  last_update = millis();

  // random number of satellites
  utm_data.satellites = rand() % 8 + 8;

  // random acceleration to change speed
  speed_m_x += float(rand() % int(2 * max_accel) - max_accel) / 1000.0;
  speed_m_y += float(rand() % int(2 * max_accel) - max_accel) / 1000.0;
  speed_m_x = constrain(speed_m_x, -max_speed, max_speed);
  speed_m_y = constrain(speed_m_y, -max_speed, max_speed);

  // make the drone favour being around the start point, basically try to keep x, y = 0
  // the radius that the drone is allowed to be in is basically 20 * max_speed
  speed_m_x = 0.95 * speed_m_x - 0.05 * x;
  speed_m_y = 0.95 * speed_m_y - 0.05 * y;

  // update the actual speed in knots
  // double absolute_speed = sqrt(pow(speed_m_x, 2) + pow(speed_m_y, 2));
  double absolute_speed = std::abs((speed_m_x, speed_m_y));
  utm_data.speed_kn = speed_ms2kn * absolute_speed;

  // compute the heading based on speed
  double heading_rads = atan2(speed_m_y, speed_m_x);
  int heading_degs = int(heading_rads * angle_rad2deg);
  utm_data.heading = heading_degs % 360;

  // calculate the new x, y
  x += speed_m_x * time_elapsed_secs;
  y += speed_m_y * time_elapsed_secs;

  // calculate new height
  float climbrate = float(rand() % int(2 * max_climbrate) - max_climbrate) / 1000.0;
  z = constrain(z + climbrate, 1.0, max_height);
  utm_data.alt_msl_m = utm_data.base_alt_m + z;
  utm_data.alt_agl_m = z;

  // update the lat long degrees
  utm_data.latitude_d  = utm_data.base_latitude  + (y / m_deg_lat);
  utm_data.longitude_d = utm_data.base_longitude + (x / m_deg_long);

  // transmit things
  squitter.transmit(&utm_data);
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

#include "frontend.h"

Frontend::Frontend(unsigned long idletime) 
  : server(80)
{
  // AP parameters
  Serial.print("Setting soft-AP ... ");
  if(WiFi.softAP("ESP_RIDS", "makkauhijau")) {
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("Ready");
  } else {
    Serial.println("Failed!");
  }

  // Specify the functions which will be executed upon corresponding GET request
  server.on("/", std::bind(&Frontend::handleOnConnect, this));
  server.on("/getlocation", std::bind(&Frontend::handleSetCoords, this)); 
  server.on("/numdrones", std::bind(&Frontend::handleNumDrones, this)); 
  server.on("/start", std::bind(&Frontend::startSpoof, this));
  server.onNotFound(std::bind(&Frontend::handleNotFound, this));

  // read parameters from EEPROM
  EEPROM.begin(512);
  // if the value at address 42 is 42, then we know that we have past data
  if (EEPROM.read(42) == 42) {
    Serial.println("EEPROM found, reusing old values...");
    EEPROM.get(latitude_addr, latitude);
    EEPROM.get(longitude_addr, longitude);
    EEPROM.get(num_drones_addr, num_drones);
  } else {
    Serial.println("EEPROM data not written before...");
  }

  // Starting the Server
  server.begin();

  // record the time the server started and when to end
  timer = millis();
  maxtime = timer + idletime;
}

void Frontend::handleClient() {
  server.handleClient();

  // automatically start spoofing if we've passed the maxtime
  if (millis() > maxtime) {
    startSpoof();
  }
}

void Frontend::handleOnConnect() {
  Serial.println("Client Connected");
  server.send(200, "text/html", HTML()); 
}

void Frontend::handleSetCoords() {
  latitude = server.arg("latitude").toFloat(); 
  longitude = server.arg("longitude").toFloat();
  EEPROM.put(latitude_addr, latitude);
  EEPROM.put(longitude_addr, longitude);
  server.send(200, "text/html", HTML());
}

void Frontend::handleNumDrones() {
  num_drones = server.arg("numdrones").toInt();
  EEPROM.put(num_drones_addr, num_drones);
  server.send(200, "text/html", HTML());
}

void Frontend::startSpoof() {
  do_spoof = true;
  server.stop();
  WiFi.softAPdisconnect (true);
  EEPROM.put(42, 42);
  EEPROM.end();
}

void Frontend::handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

String Frontend::HTML() {
  String msg = R"rawliteral(
    <!DOCTYPE html>
      <html>
      <head>
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">
        <title>Remote ID Spoofer</title>
        <style>
          html{font-family:Helvetica; display:inline-block; margin:0px auto; text-align:center;}
          body{margin-top: 50px;}
          h1{color: #444444; margin: 50px auto 30px; font-size:6em;}
          h3{color:#444444; margin-bottom: 50px;}
          .configurator{font-size:4.0em; margin-bottom: 50px;}
          .selection{font-size:1.0em;}
          .button{display:block; background-color:#f48100; border:none; color:white; padding: 13px 30px; text-decoration:none; font-size:6em; margin: 0px auto 35px; cursor:pointer; border-radius:4px;}
          .button-on{background-color:#f48100;}
          .button-on:active{background-color:#f48100;}
          .button-off{background-color:#26282d;}
          .button-off:active{background-color:#26282d;}
        </style>
      </head>
      <body onload="updateTime();">
        <h1>Remote ID Spoofer</h1>
        <form class="configurator" action="/getlocation">
          Latitude: <input class="selection" type="text" name="latitude">
          <br>
          Longitude: <input class="selection" type="text" name="longitude">
          <br>
          <input class="selection" type="submit" value="Submit">
        </form>
        <form class="configurator" action="/numdrones">
          No. of Drones:
          <select class="selection" name="numdrones">
            <option value="1">1</option>
            <option value="2">2</option>
            <option value="3">3</option>
            <option value="4">4</option>
            <option value="5">5</option>
            <option value="6">6</option>
            <option value="7">7</option>
            <option value="8">8</option>
            <option value="9">9</option>
            <option value="10">10</option>
            <option value="11">11</option>
            <option value="12">12</option>
            <option value="13">13</option>
            <option value="14">14</option>
            <option value="15">15</option>
            <option value="16">16</option>
          </select>
          <input class="selection" type="submit" value="Submit">
        </form>
  )rawliteral";

  msg += "<p><b>Current Coordinates:</b><br>";
  msg += String(latitude, 10);
  msg += ", ";
  msg += String(longitude, 10);
  msg += "</p>\n";

  msg += "<p><b>Current No. of Drones:</b>";
  msg += String(num_drones);
  msg += "</p>\n";

  msg += "<a class=\"button button-on\" href=\"/start\">Start Spoofing</a>\n";
  msg += "<p>Pressing this button will cause the device to turn off the web server and enter spoofing only mode.\n";
  msg += "Please confirm your GPS coordinates before doing so.\n";
  msg += "You will not be able to reconnect to this page without a power cycle.</p>\n";

  msg += R"rawliteral(
      <script>
        function updateTime() {
          var today = new Date();
          return [today.getHours(), today.getMinutes(), today.getSeconds()];
        }
      </script>
    </body>
    </html>
  )rawliteral";

  return msg;
}
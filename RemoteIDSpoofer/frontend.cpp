#include "frontend.h"

Frontend::Frontend() 
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
  server.on("/getlocation", std::bind(&Frontend::handleForm, this)); 
  server.on("/start", std::bind(&Frontend::startSpoof, this));
  server.onNotFound(std::bind(&Frontend::handleNotFound, this));

  // Starting the Server
  server.begin();
}

void Frontend::handleClient() {
  server.handleClient();
}

void Frontend::handleOnConnect() {
  Serial.println("Client Connected");
  server.send(200, "text/html", HTML()); 
}

void Frontend::handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void Frontend::startSpoof() {
  do_spoof = true;
  WiFi.softAPdisconnect (true);
}

void Frontend::handleForm() {
  latitude = server.arg("latitude").toFloat(); 
  longitude = server.arg("longitude").toFloat();
  server.send(200, "text/html", HTML());
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
          body{margin-top: 50px;} h1{color: #444444; margin: 50px auto 30px;} h3{color:#444444; margin-bottom: 50px;}
          .button{display:block; width:80px; background-color:#f48100; border:none; color:white; padding: 13px 30px; text-decoration:none; font-size:25px; margin: 0px auto 35px; cursor:pointer; border-radius:4px;}
          .button-on{background-color:#f48100;}
          .button-on:active{background-color:#f48100;}
          .button-off{background-color:#26282d;}
          .button-off:active{background-color:#26282d;}
        </style>
      </head>
      <body>
        <h1>Remote ID Spoofer</h1>
        <form action="/getlocation">
          Latitude: <input type="text" name="latitude">
          Longitude: <input type="text" name="longitude">
          <input type="submit" value="Submit">
        </form>
  )rawliteral";

  msg += "<p>Current Coordinates: ";
  msg += String(latitude, 10);
  msg += ", ";
  msg += String(longitude, 10);
  msg += "</p>\n";

  msg += "<p>Pressing this button will cause the device to turn off the web server and enter spoofing only mode.</p>\n";
  msg += "<p>Please confirm your GPS coordinates before doing so.</p>\n";
  msg += "<p>You will not be able to reconnect to this page without a power cycle.</p>\n";
  msg += "<a class=\"button button-on\" href=\"/start\">Start Spoofing</a>\n";

  msg += R"rawliteral(
    </body>
    </html>
  )rawliteral";

  return msg;
}
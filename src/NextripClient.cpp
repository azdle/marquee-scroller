/** The MIT License (MIT)

Copyright (c) 2018 David Payne

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "NextripClient.h"

NextripClient::NextripClient(String _stopId) {
  stopId = stopId;
}

void NextripClient::updateDepartures() {
  WiFiClient client;
  String httpRequestLine = "GET /NexTrip/56038" + stopId + "?format=json HTTP/1.1";

  success = false;
  error = "internal error";

  Serial.println("Getting Nextrip Data");
  Serial.println(httpRequestLine);
  if (client.connect(servername, 80)) {
    client.println(httpRequestLine);
    client.print("Host: ");
    client.println(servername);
    client.print("User-Agent: Arduino/");
    client.print(ARDUINO);
    client.println(" MarqueeScroller/2.2");
    client.println("Connection: close");
    client.println();
  }
  else {
    error = "failed to connect";
    Serial.print(F("Nextrip: "));
    Serial.println(error);
    return;
  }

  while(client.connected() && !client.available()) delay(1); //waits for data

  Serial.println("Waiting for data");

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  Serial.println("Response Header: " + String(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    error = String("unexpected response: ") + status;
    Serial.print(F("Nextrip: "));
    Serial.println(error);

    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    error = "invalid response";
    Serial.print(F("Nextrip: "));
    Serial.println(error);

    return;
  }

  const size_t bufferSize = 2048;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonArray& data = jsonBuffer.parseArray(client);
  if (!data.success()) {
    error = "unable to parse response";
    Serial.print(F("Nextrip: "));
    Serial.println(error);
    return;
  } else {
    Serial.println("parsed");
  }

  client.stop();

  if (data.size() == 0) {
    error = "no departures returned";
    Serial.print(F("Nextrip: "));
    Serial.println(error);
    return;
  } else {
    Serial.print("Len: ");
    Serial.println(data.size());
  }

  success = true;

  count = min(data.size(), (size_t)5);

  for (size_t i = 0; i < count; i++) {
    departures[i].actual = data[i]["Actual"];
    departures[i].blockNumber = data[i]["BlockNumber"];
    departures[i].departureText = (const char*)data[i]["DepartureText"];
    departures[i].departureTime = (const char*)data[i]["departureTime"];
    departures[i].description = (const char*)data[i]["Description"];
    departures[i].gate = (const char*)data[i]["Gate"];
    departures[i].route = (const char*)data[i]["Route"];
    departures[i].routeDirection = (const char*)data[i]["RouteDirection"];
    departures[i].terminal = (const char*)data[i]["Terminal"];
    departures[i].vehicleHeading = (const char*)data[i]["VehicleHeading"];
    departures[i].vehicleLatitude = (const char*)data[i]["VehicleLatitude"];
    departures[i].vehicleLongitude = (const char*)data[i]["VehicleLongitude"];
  }
}

void NextripClient::updateStopId(String newStopId) {
  stopId = newStopId;
}

String reformatDepartureText(String orig) {
  if (orig.endsWith(" Min")) {
    return orig.substring(0, orig.length()-4) + "m";
  } else {
    return orig;
  }
}

String NextripClient::getDepartures() {
  String msg = "";

  for (size_t i = 0; i < count; i++) {
    if (i != 0) {
      if (!departures[i].actual)
        break;
      msg += ",  ";
    }
    msg += reformatDepartureText(departures[i].departureText);
  }
  return msg;
}

String NextripClient::getFirstDeparture() {
  if (success && count > 0) {
    return reformatDepartureText(departures[0].departureText);
  } else {
    return "???";
  }
}

boolean NextripClient::getActual(int index) {
  return departures[index].actual;
}

uint16 NextripClient::getBlockNumber(int index) {
  return departures[index].blockNumber;
}

String NextripClient::getDepartureText(int index) {
  return departures[index].departureText;
}

String NextripClient::getDepartureTime(int index) {
  return departures[index].departureTime;
}

String NextripClient::getDescription(int index) {
  return departures[index].description;
}

String NextripClient::getGate(int index) {
  return departures[index].gate;
}

String NextripClient::getRoute(int index) {
  return departures[index].route;
}

String NextripClient::getRouteDirection(int index) {
  return departures[index].routeDirection;
}

String NextripClient::getTerminal(int index) {
  return departures[index].terminal;
}

String NextripClient::getVehicleHeading(int index) {
  return departures[index].vehicleHeading;
}

String NextripClient::getVehicleLatitude(int index) {
  return departures[index].vehicleLatitude;
}

String NextripClient::getVehicleLongitude(int index) {
  return departures[index].vehicleLongitude;
}


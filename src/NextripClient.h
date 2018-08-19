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

#pragma once
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

class NextripClient {

private:
  String stopId = "";

  boolean success = false;
  String error = "";

  const char* servername = "svc.metrotransit.org";

  typedef struct {
    boolean actual;
    uint16 blockNumber;
    String departureText;
    String departureTime;
    String description;
    String gate;
    String route;
    String routeDirection;
    String terminal;
    String vehicleHeading;
    String vehicleLatitude;
    String vehicleLongitude;
  } departure;

  size_t count = 0;
  departure departures[5];

public:
  NextripClient(String stopId);
  void updateDepartures();
  void updateStopId(String stopId);

  String getDepartures();
  String getFirstDeparture();

  boolean getActual(int index);
  uint16 getBlockNumber(int index);
  String getDepartureText(int index);
  String getDepartureTime(int index);
  String getDescription(int index);
  String getGate(int index);
  String getRoute(int index);
  String getRouteDirection(int index);
  String getTerminal(int index);
  String getVehicleHeading(int index);
  String getVehicleLatitude(int index);
  String getVehicleLongitude(int index);
};


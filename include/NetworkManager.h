#pragma once

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif


class NetworkManager
{
public:
  NetworkManager() = default;
  ~NetworkManager() = default;

  void begin(const char *ssid, const char *password)
  {
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);

    Serial.printf("Connecting...");
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.printf(".");
    }
    Serial.printf("\n");
    Serial.printf("Connected! IP: %s\n", WiFi.localIP().toString().c_str());
  }

  void setHostname(const char *name)
  {
#ifdef ESP32
    WiFi.setHostname(name);
#elif defined(ESP8266)
    WiFi.hostname(name);
#endif
  }
  
private:

};

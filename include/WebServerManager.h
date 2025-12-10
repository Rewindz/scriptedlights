#pragma once

#include <Arduino.h>
#include <vector>

#ifdef ESP32
  #include <WebServer.h>
  typedef WebServer WebServerClass;
#elif defined(ESP8266)
  #include <ESP8266WebServer.h>
  typedef ESP8266WebServer WebServerClass;
#endif

typedef void (*ScriptSubmitCb) (const String&);

class WebServerManager
{
public:
  WebServerManager() :
    _server(80)
    {

    }

    void begin()
    {

      _server.on("/", HTTP_GET, [this](){
        handleRoot();
      });

      _server.on("/update", HTTP_POST, [this](){
        handleUpdate();
      });

      _server.onNotFound([this](){
        _server.send(404, "text/plain", "404: Not Found");
      });

      _server.begin();
      Serial.printf("Webserver started!\n");
    }

    inline void addSubmitCallback(ScriptSubmitCb callback)
    {
      _callbacks.push_back(callback);
    }

    inline void handleClient() 
    {
      _server.handleClient();
    }

private:

  void handleRoot()
  {
    const char *html = R"(<!DOCTYPE html><html>
    <head>
      <title>Scripted Lights</title>
    </head>
    <body>
      <h1>Script</h1>
      <form action='update' method='POST'>
        <label for='script'>Script</label><br>
        <textarea id='script' name='script' rows='10' cols='50'></textarea><br>
        <input type='submit' value='Submit'>
      </form>
    </body>
    </html>)";

    _server.send(200, "text/html", html);
  }

  void handleUpdate()
  {
    if(_server.hasArg("script")){
      String receivedScript = _server.arg("script");
      Serial.printf("Received Script:\n%s\n", receivedScript.c_str());
      for(const auto& cb : _callbacks)
        cb(receivedScript);
    }
    _server.sendHeader("Location", "/");
    _server.send(303);
  }

  WebServerClass _server;
  std::vector<ScriptSubmitCb> _callbacks;

};
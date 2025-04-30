#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>

extern float target_x;  // Khai báo biến toàn cục
extern float target_y;

class WiFiConfig {
public:
    WiFiConfig();
    void begin();
    void startConfigMode();
    void handleClient();
    bool isConnected();
    String getLocalIP();
private:
    WebServer server;
    void setupRoutes();
};

#endif

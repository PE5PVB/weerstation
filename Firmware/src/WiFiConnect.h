/*!
   @file WiFiConnect.h

   WiFi Connection Manager with Captive Portal for ESP32.

   Based on WiFiConnect by Stuart Blair
   (https://github.com/smurf0969/WiFiConnect), derived from
   WiFiManager by tzapu (https://github.com/tzapu/WiFiManager).

   Simplified for ESP32-only use.

   GNU General Public License v3.0
*/
#ifndef WiFiConnect_h
#define WiFiConnect_h

#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
using fs::FS;
#include <WebServer.h>
#include <DNSServer.h>
#include <esp_wifi.h>
#include <memory>

#define WiFiConnect_MAX_PARAMS 10
#define ESP_getChipId() ((uint32_t)ESP.getEfuseMac())

class WiFiConnect;

class WiFiConnectParam {
public:
  WiFiConnectParam(const char *custom);
  WiFiConnectParam(const char *id, const char *placeholder, const char *defaultValue, int length);
  ~WiFiConnectParam();

  const char *getID();
  const char *getValue();
  const char *getPlaceholder();
  int         getValueLength();
  const char *getCustomHTML();

private:
  const char *_id;
  const char *_placeholder;
  char       *_value;
  int         _length;
  const char *_customHTML;

  void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);
  void setValue(const char *newValue);

  friend class WiFiConnect;
};

class WiFiConnect {
public:
  WiFiConnect();

  boolean autoConnect();
  void addParameter(WiFiConnectParam *p);
  boolean startConfigurationPortal(int8_t cancelPin = -1);

private:
  static const int RETRY_ATTEMPTS = 3;
  static const int CONNECTION_TIMEOUT_SECS = 10;
  static const byte DNS_PORT = 53;
  static const int MINIMUM_QUALITY = 8;

  int _paramsCount = 0;
  boolean _readyToConnect = false;
  String _ssid;
  String _password;

  WiFiConnectParam* _params[WiFiConnect_MAX_PARAMS];

  std::unique_ptr<DNSServer> dnsServer;
  std::unique_ptr<WebServer> server;

  char _apName[33];

  void setAPName();

  boolean autoConnect(const char *ssidName, const char *ssidPassword, WiFiMode_t acWiFiMode);

  void handleRoot();
  void handleWifi();
  void handleScan();
  void handleWifiSave();
  void handleNotFound();

  boolean captivePortal();
  boolean isIp(String str);
  String  toStringIp(IPAddress ip);
  int     getRSSIasQuality(int RSSI);
};

#endif

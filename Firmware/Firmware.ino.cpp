# 1 "C:\\Users\\pe5pv\\AppData\\Local\\Temp\\tmp3q5k3b4e"
#include <Arduino.h>
# 1 "C:/Users/pe5pv/OneDrive - Vereniging van Radio Zendamateurs/GitHub/weerstation/Firmware/Firmware.ino"
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <EEPROM.h>
#include <esp_task_wdt.h>
#include "src/AS3935_ESP32.h"
#include "src/maidenhead.h"
#include "src/WiFiConnect.h"
#include <ArduinoJson.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <DFRobot_SGP40.h>
#include "DHT.h"
#include <EasyNextionLibrary.h>

#define SOFTWAREVERSION 15



#define NL_TIMEZONE "CET-1CEST,M3.5.0/2,M10.5.0/3"

WiFiConnect wc;
TinyGPSPlus gps;
EasyNex Display(Serial2);
DHT dht(32, DHT22);
SoftwareSerial ss(12, 13);
DFRobot_SGP40 mySgp40;
AS3935_ESP32 lightning(AS3935_ADDR_DEFAULT);

bool beeper;
bool beeper_alarm = false;
bool beeper_lightning = false;
bool display_alarm = false;
bool display_gps = false;
bool display_radio = false;
bool display_weather = true;
bool menu;
bool radio;
bool weeralarm;
bool weeralarmold;
bool wifistatus = false;
byte alarmswitch;
byte autodim;
byte autoloc;
byte contrast = 100;
byte currenttrigger;
byte d1image;
byte d1imageold;
byte d1windk;
byte d1windkold;
byte d2image;
byte d2imageold;
byte d2windk;
byte d2windkold;
byte image;
byte imageold;
byte indoor;
int indooroffset;
int dauw;
int dauwold;
byte lv;
byte lvold;
byte ntp;
byte spike;
byte threshold;
long rssiold;
byte winds;
byte windsold;
int zicht;
int zichtold;
byte kmu;
double latitude;
double longitude;
int br_mmpu[24];
char br_tijd[24][6];
int d0tmax;
int d0tmaxold;
int d0tmin;
int d0tminold;
int d1neerslag;
int d1neerslagold;
int d1tmax;
int d1tmaxold;
int d1tmin;
int d1tminold;
int d1zon;
int d1zonold;
int d2neerslag;
int d2neerslagold;
int d2tmax;
int d2tmaxold;
int d2tmin;
int d2tminold;
int d2zon;
int d2zonold;
int indoorhum;
int indoorhumold;
int indoortemp;
int indoortempold;
int indoorvoc;
int indoorvocold;
int lightningcount = -1;
int luchtd;
int luchtdold;
int temp;
int tempold;
int windkmh;
int windkmhold;
int windms;
int windmsold;
String aindex;
String aindexold;
String alarmtxt;
String alarmtxtold = "Dummy";
String aurora;
String auroraold;
String br_tijdold;
String d1210m;
String d1210mold;
String d1715m;
String d1715mold;
String d1windr;
String d1windrold;
String d2windr;
String d2windrold;
String d3020m;
String d3020mold;
String d8040m;
String d8040mold;
String datetime;
String datetimeold;
String datetimestamp;
String dayname1old;
String dayname2old;
String electonflux;
String electonfluxold;
String es2m;
String es2mold;
String es4m;
String es4mold;
String es6m;
String es6mold;
String geomagfield;
String geomagfieldold;
String key;
String kindex;
String kindexold;
String latdegree;
String latdegreeold;
String latitudeold;
String lightningtext[6];
String lightningtextmerge = "Geen meldingen";
String lightningtextmergeold;
String longitudeold;
String magneticfield;
String magneticfieldold;
String muf_string[9];
String muf_stringold[9];
String n1210m;
String n1210mold;
String n1715m;
String n1715mold;
String n3020m;
String n3020mold;
String n8040m;
String n8040mold;
String normalisation;
String normalisationold;
String plaats;
String plaatsold;
String protonflux;
String protonfluxold;
String qthold;
String samenv;
String samenvold;
String signalnoise;
String signalnoiseold;
String solarflux;
String solarfluxold;
String solarwind;
String solarwindold;
String sunspots;
String sunspotsold;
String verw;
String verwold;
String weercode;
String weercodeold;
String windr;
String windrold;
String zononder;
String zononderold;
String zonop;
String zonopold;
unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long time_3 = 0;
unsigned long time_4 = 0;
unsigned long time_5 = 0;
unsigned long time_6 = 0;
const int intPin = 19;
void setup(void);
void loop(void);
void setLightning();
void trigger1();
void trigger2();
void trigger3();
void trigger4();
void trigger5();
void trigger6();
void trigger7();
void trigger8();
void trigger9();
void trigger10();
void trigger11();
void trigger12();
void trigger13();
void trigger14();
void ResetScreenData();
void showRSSI();
void setWifi();
void getPropagation();
byte imageFromString(String img);
void getWeather();
void getMUF(void);
void showData(void);
void buildMUFticker();
void showGPS();
void getIndoor();
void getGPS();
void FormatTime();
void buzzer(byte times);
void defaults();
void buienradar();
int splitString(const String& input, char delimiter, String* output, int maxItems);
#line 203 "C:/Users/pe5pv/OneDrive - Vereniging van Radio Zendamateurs/GitHub/weerstation/Firmware/Firmware.ino"
void setup(void) {
  pinMode(2, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(intPin, INPUT);
  ss.begin(9600);
  Display.begin(115200);
  Serial.begin(115200);
  EEPROM.begin(47);
  if (EEPROM.readByte(41) != 11) defaults();
  latitude = EEPROM.readDouble(9);
  longitude = EEPROM.readDouble(1);
  key = EEPROM.readString(17);
  beeper = EEPROM.readByte(29);
  radio = EEPROM.readByte(30);
  kmu = EEPROM.readByte(32);
  autoloc = EEPROM.readByte(33);
  indoor = EEPROM.readByte(34);
  threshold = EEPROM.readByte(35);
  autodim = EEPROM.readByte(36);
  contrast = EEPROM.readByte(37);
  spike = EEPROM.readByte(38);
  alarmswitch = EEPROM.readByte(39);
  ntp = EEPROM.readByte(40);
  indooroffset = EEPROM.readInt(42);
  Display.writeNum("dim", contrast);
  buzzer(0);
  Display.writeStr("page 0");
  Display.writeNum("tm0.en", 0);
  Display.writeStr("version.txt", ("v " + String(SOFTWAREVERSION / 10) + "." + String(SOFTWAREVERSION % 10)));


  Wire.begin();
  Display.writeStr("info.txt", "Lightning sensor");
  Display.writeStr("status.txt", " ");
  if (!lightning.begin(intPin)) {
    Serial.println("Geen Lightning sensor");
    Display.writeStr("status.txt", "FOUT!");
  } else {
    Serial.println("Lightning sensor OK");
    Display.writeStr("status.txt", "OK!");
    AS3935_TuneResult tune = lightning.tuneAntenna();
    if (!tune.success) {
      Serial.println("Antenne calibratie mislukt!");
    }
    setLightning();
  }
  delay(1000);


  Display.writeStr("info.txt", "Verbinden met WiFi...");
  Display.writeStr("status.txt", " ");
  Serial.print("WiFi verbinding opbouwen.....");
  if (wc.autoConnect()) {
    Serial.println("WiFi verbonden");
    Display.writeStr("status.txt", "OK!");
    wifistatus = true;
  } else {
    Serial.println("Geen verbinding, opnieuw instellen");
    Display.writeStr("page 6");
    while (!wifistatus) Display.NextionListen();
    Display.writeStr("page 0");
  }
  delay(1000);
  Display.writeStr("info.txt", "GPS module");
  Display.writeStr("status.txt", " ");
  if (!ss.available()) {
    Serial.println("Geen GPS module");
    Display.writeStr("status.txt", "FOUT!");
  } else {
    Serial.println("GPS module OK");
    Display.writeStr("status.txt", "OK!");
  }
  delay(1000);
  Display.writeStr("info.txt", "Temperatuur sensor");
  Display.writeStr("status.txt", " ");
  dht.begin();
  if (isnan(dht.readTemperature())) {
    Serial.println("Geen temperatuur sensor");
    Display.writeStr("status.txt", "FOUT!");
  } else {
    Serial.println("Temperatuursensor OK");
    Display.writeStr("status.txt", "OK!");
  }
  delay(1000);
  Display.writeStr("info.txt", "VOC sensor");
  Display.writeStr("status.txt", " ");
  if (!mySgp40.begin(10000)) {
    Serial.println("Geen VOC sensor");
    Display.writeStr("status.txt", "FOUT!");
  } else {
    Serial.println("VOC sensor OK");
    Display.writeStr("status.txt", "OK!");
  }
  delay(1000);
  if (radio == 1) Display.writeNum("radiobutton", 1);
  Serial.println("Setup klaar");
  Display.writeStr("info.txt", "Gegevens downloaden...");
  Display.writeStr("status.txt", " ");
  getWeather();
  getIndoor();
  showRSSI();
  delay(1000);
  configTzTime(NL_TIMEZONE, "europe.pool.ntp.org");
  Display.writeNum("setkmu", kmu);
  Display.writeStr("page 1");
  showData();
  buienradar();


  esp_task_wdt_init(30, true);
  esp_task_wdt_add(NULL);
}

void loop(void) {
  esp_task_wdt_reset();
  digitalWrite(2, HIGH);
  Display.NextionListen();
  getGPS();

  if (!menu) {
    if (millis() - time_1 >= 600000) {
      time_1 = millis();
      getPropagation();
    }

    if (millis() - time_2 >= 600000) {
      time_2 = millis();
      getMUF();
    }

    if (millis() - time_3 >= 300000) {
      time_3 = millis();
      getWeather();
      buienradar();
    }

    if (millis() - time_4 >= 5000) {
      time_4 = millis();
      showRSSI();
      if (display_weather) {
        if (WiFi.status() != WL_CONNECTED) {
          if (wifistatus) {
            Serial.println("WiFi connection lost. Reconnecting...");
            wifistatus = false;
            WiFi.reconnect();
            Display.writeNum("rssi", 0);
          }
        } else if (!wifistatus) {
          Serial.println("WiFi verbonden");
          wifistatus = true;
        }
      }
      getIndoor();
      showData();
      showGPS();


      uint32_t largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
      if (largestBlock < 60000) {
        Serial.printf("Heap kritiek: grootste blok %d bytes, herstart!\n", largestBlock);
        ESP.restart();
      }
    }

    if (millis() - time_6 >= 600000) {
      time_6 = millis();
      beeper_lightning = false;
      if (!weeralarm) {
        Display.writeNum("b_alarm.pic", 67);
      }
    }

    if (millis() - time_5 >= 300000) {
      if (autodim) Display.writeStr("dim=3");
      time_5 = millis();
    }

    if (weeralarm == 1 || alarmswitch == 0) {
      if (digitalRead(intPin) == HIGH) {
        byte x = lightning.readInterruptReg();

        if (x == 0x01) {
          Serial.println("Noise detected!");
        }

        if (x == 0x04) {
          Serial.println("Disturber detected!");
        }

        if (x == 0x08) {
          unsigned int lightningDistKm = lightning.distanceToStorm();
          Serial.println("Lightning occurs!");
          Serial.print("Distance: ");
          Serial.print(lightningDistKm);
          Serial.println(" km");

          unsigned int lightningEnergyVal = lightning.lightningEnergy();
          Serial.print("Intensity: ");
          Serial.print(lightningEnergyVal);
          Serial.println("");

          if (lightningDistKm > 1) {
            lightningcount++;
            if (!beeper_lightning) {
              buzzer(0);
              beeper_lightning = true;
              time_6 += 600000;
              Display.writeNum("b_alarm.pic", 49);
            }
            if (lightningcount > 5) {
              for (int i = 0; i < 5; i++) lightningtext[i] = lightningtext[i + 1];
              lightningtext[5] = (datetimestamp + " Afstand: " + String(lightningDistKm) + "km");
              lightningcount = 5;
            } else {
              lightningtext[lightningcount] = (datetimestamp + " Afstand: " + String(lightningDistKm) + "km");
            }
            lightningtextmerge = (lightningtext[0] + "\\r" + lightningtext[1] + "\\r" + lightningtext[2] + "\\r" + lightningtext[3] + "\\r" + lightningtext[4] + "\\r" + lightningtext[5] + "\\r");
          }
        }
      }
    }
  }

  digitalWrite(2, LOW);
}

void setLightning() {
  lightning.setMaskDisturber(true);
  if (indoor) lightning.setIndoorOutdoor(AS3935_INDOOR); else lightning.setIndoorOutdoor(AS3935_OUTDOOR);
  lightning.setNoiseLevel(2);
  lightning.setWatchdogThreshold(threshold);
  lightning.setSpikeRejection(spike);
  lightning.setLightningThreshold(1);

  Serial.println("Lightning watchdog threshold set to: " + String(lightning.readWatchdogThreshold()));
  Serial.println("Lightning spike rejection set to: " + String(lightning.readSpikeRejection()));
  Serial.println("Lightning threshold set to: " + String(lightning.readLightningThreshold()));
  Serial.println("Lightning noise level set to: " + String(lightning.readNoiseLevel()));
}

void trigger1() {
  if (currenttrigger != 1) {
    Display.writeNum("dim", contrast);
    time_5 = millis();
    ResetScreenData();
    display_weather = true;
    display_gps = false;
    display_radio = false;
    display_alarm = false;
    if (radio == 0) Display.writeNum("radiobutton", 0);
    if (radio == 1) Display.writeNum("radiobutton", 1);
    Display.writeStr("page 1");
    showData();
    currenttrigger = 1;
  }
}

void trigger2() {
  if (currenttrigger != 2) {
    Display.writeNum("dim", contrast);
    time_5 = millis();
    ResetScreenData();
    display_weather = false;
    display_radio = true;
    Display.writeStr("page 3");
    showData();
    currenttrigger = 2;
  }
}

void trigger3() {
  if (currenttrigger != 3) {
    Display.writeNum("dim", contrast);
    time_5 = millis();
    ResetScreenData();
    display_weather = false;
    display_gps = true;
    Display.writeStr("page 2");
    showData();
    showGPS();
    currenttrigger = 3;
  }
}

void trigger4() {
  if (currenttrigger != 4) {
    menu = true;
    Display.writeNum("dim", contrast);
    time_5 = millis();
    ResetScreenData();
    display_weather = false;
    Display.writeStr("page 9");
    delay(100);
    if (!alarmswitch) {
      Display.writeNum("b_weeralarm.pic", 50);
    } else {
      Display.writeNum("b_weeralarm.pic", 51);
    }
    if (!indoor) {
      Display.writeNum("b_indoor.pic", 50);
    } else {
      Display.writeNum("b_indoor.pic", 51);
    }
    Display.writeNum("thresholdval.val", threshold);
    Display.writeNum("bliksemval.val", spike);
    Display.writeStr("api.txt", key);
    showData();
    currenttrigger = 4;
  }
}

void trigger5() {
  if (currenttrigger != 5) {
    Display.writeNum("dim", contrast);
    time_5 = millis();
    delay(100);
    if (Display.readNumber("b_weeralarm.pic") == 50) {
      alarmswitch = false;
    } else {
      alarmswitch = true;
    }
    delay(100);
    if (Display.readNumber("b_indoor.pic") == 50) {
      indoor = false;
    } else {
      indoor = true;
    }
    delay(100);
    threshold = Display.readNumber("thresholdval.val");
    delay(100);
    spike = Display.readNumber("bliksemval.val");
    EEPROM.writeByte(39, alarmswitch);
    EEPROM.writeByte(34, indoor);
    EEPROM.writeByte(35, threshold);
    EEPROM.writeByte(38, spike);
    EEPROM.commit();

    Display.writeStr("page 4");
    delay(100);
    ResetScreenData();
    if (!beeper) {
      Display.writeNum("b_beeper.pic", 50);
    } else {
      Display.writeNum("b_beeper.pic", 51);
    }
    if (!radio) {
      Display.writeNum("b_radio.pic", 50);
    } else {
      Display.writeNum("b_radio.pic", 51);
    }
    if (!kmu) {
      Display.writeNum("kmu.pic", 50);
    } else {
      Display.writeNum("kmu.pic", 51);
    }
    Display.writeNum("contrastval.val", contrast);
    Display.writeStr("api.txt", key);
    showData();
    currenttrigger = 5;
  }
}

void trigger6() {
  Display.writeNum("dim", contrast);
  time_5 = millis();
  if (Display.readNumber("b_radio.pic") == 50) {
    radio = false;
  } else {
    radio = true;
  }
  ResetScreenData();
  showData();
}

void trigger7() {
  if (currenttrigger != 7) {
    Display.writeNum("dim", contrast);
    time_5 = millis();
    Display.writeStr("page 5");
    Display.writeStr("qr.txt", (String("WIFI:T:nopass;S:ESP_" + String(ESP_getChipId()) + ";P:;H:;")));
    Display.writeStr("SSID_AP.txt", (String("ESP_" + String(ESP_getChipId()))));
    setWifi();
    ESP.restart();
    currenttrigger = 7;
  }
}

void trigger8() {
  if (currenttrigger != 8) {
    if (wc.autoConnect()) {
      wifistatus = true;
    } else {
      Display.writeStr("page 6");
    }
    currenttrigger = 8;
  }
}

void trigger9() {
  if (currenttrigger != 9) {
    Display.writeNum("dim", contrast);
    time_5 = millis();
    ResetScreenData();
    display_weather = false;
    display_alarm = true;
    Display.writeStr("page 7");
    Display.writeStr("warning.txt", alarmtxt);
    showData();
    currenttrigger = 9;
  }
}

void trigger10() {
  if (currenttrigger != 10) {
    currenttrigger = 10;
    trigger1();
  }
}

void trigger11() {
  if (currenttrigger != 11) {
    Display.writeNum("dim", contrast);
    time_5 = millis();
    delay(100);
    if (Display.readNumber("b_autoloc.pic") == 50) {
      autoloc = false;
    } else {
      autoloc = true;
    }
    delay(100);
    if (Display.readNumber("b_ntp.pic") == 50) {
      ntp = false;
    } else {
      ntp = true;
    }
    delay(100);
    if (Display.readNumber("b_dimmer.pic") == 50) {
      autodim = false;
    } else {
      autodim = true;
    }
    delay(100);
    indooroffset = Display.readNumber("indooroffset.val");
    EEPROM.writeByte(33, autoloc);
    EEPROM.writeByte(40, ntp);
    EEPROM.writeInt(42, indooroffset);
    EEPROM.writeByte(36, autodim);
    EEPROM.commit();
    currenttrigger = 11;
    menu = false;
    setLightning();
    trigger1();
  }
}

void trigger12() {
  Display.writeNum("dim", contrast);
  time_5 = millis();
}

void trigger13() {
  buzzer(1);
}

void trigger14() {
  if (currenttrigger != 14) {
    Display.writeNum("dim", contrast);
    time_5 = millis();
    delay(100);
    if (Display.readNumber("b_beeper.pic") == 50) {
      beeper = false;
    } else {
      beeper = true;
    }
    delay(100);
    if (Display.readNumber("b_radio.pic") == 50) {
      radio = false;
    } else {
      radio = true;
    }
    delay(100);
    if (Display.readNumber("kmu.pic") == 50) {
      kmu = false;
    } else {
      kmu = true;
    }
    delay(100);
    contrast = Display.readNumber("contrastval.val");
    EEPROM.writeByte(29, beeper);
    EEPROM.writeByte(30, radio);
    EEPROM.writeByte(32, kmu);
    EEPROM.writeByte(37, contrast);
    EEPROM.commit();
    Display.writeNum("setkmu", kmu);
    delay(100);
    Display.writeStr("page 8");
    delay(100);
    ResetScreenData();
    if (!autoloc) {
      Display.writeNum("b_autoloc.pic", 50);
    } else {
      Display.writeNum("b_autoloc.pic", 51);
    }
    if (!ntp) {
      Display.writeNum("b_ntp.pic", 50);
    } else {
      Display.writeNum("b_ntp.pic", 51);
    }
    if (!autodim) {
      Display.writeNum("b_dimmer.pic", 50);
    } else {
      Display.writeNum("b_dimmer.pic", 51);
    }
    Display.writeNum("indooroffset.val", indooroffset);
    Display.writeStr("api.txt", key);
    showData();
    currenttrigger = 5;
  }
}

void ResetScreenData() {
  lightningtextmergeold = " ";
  dayname1old = " ";
  dayname2old = " ";
  longitudeold = " ";
  latitudeold = " ";
  datetimeold = " ";
  qthold = " ";
  muf_stringold[0] = " ";
  muf_stringold[1] = " ";
  muf_stringold[2] = " ";
  muf_stringold[3] = " ";
  muf_stringold[4] = " ";
  muf_stringold[5] = " ";
  muf_stringold[6] = " ";
  muf_stringold[7] = " ";
  muf_stringold[8] = " ";
  solarfluxold = " ";
  aindexold = " ";
  kindexold = " ";
  sunspotsold = " ";
  protonfluxold = " ";
  electonfluxold = " ";
  zonopold = " ";
  zononderold = " ";
  auroraold = " ";
  normalisationold = " ";
  latdegreeold = " ";
  solarwindold = " ";
  magneticfieldold = " ";
  d8040mold = " ";
  n8040mold = " ";
  d3020mold = " ";
  n3020mold = " ";
  d1715mold = " ";
  n1715mold = " ";
  d1210mold = " ";
  n1210mold = " ";
  es2mold = " ";
  es6mold = " ";
  es4mold = " ";
  geomagfieldold = " ";
  signalnoiseold = " ";
  plaatsold = " ";
  windkmhold = 0;
  windmsold = 0;
  tempold = 0;
  samenvold = " ";
  lvold = 0;
  dauwold = 0;
  windrold = " ";
  windsold = 0;
  luchtdold = 0;
  zichtold = 0;
  verwold = " ";
  imageold = 0;
  weeralarmold = 0;
  d1imageold = 0;
  d2imageold = 0;
  d1zonold = 0;
  d1neerslagold = 0;
  d2zonold = 0;
  d2neerslagold = 0;
  d0tminold = 0;
  d0tmaxold = 0;
  d1tminold = 0;
  d1tmaxold = 0;
  d1windkold = 0;
  d1windrold = " ";
  d2tminold = 0;
  d2tmaxold = 0;
  d2windkold = 0;
  d2windrold = " ";
  indoorhumold = 0;
  indoortempold = 0;
  indoorvocold = 0;
  weeralarmold = false;
  weercodeold = " ";
  br_tijdold = " ";
}

void showRSSI()
{
  long rssi = WiFi.RSSI();
  if (rssi != rssiold) {
    rssiold = rssi;
    byte level;
    if (rssi == 0) level = 0;
    else if (rssi > -50) level = 4;
    else if (rssi > -60) level = 3;
    else if (rssi > -70) level = 2;
    else level = 1;
    Display.writeNum("rssi", level);
  }
}

void setWifi() {
  char api_key [11];
  key.toCharArray(api_key, 11);
  WiFiConnectParam api_key_text("Enter weerlive.nl API key:");
  WiFiConnectParam api_key_input("api_key", "API Key", api_key, 11);
  wc.addParameter(&api_key_text);
  wc.addParameter(&api_key_input);

  wc.startConfigurationPortal();
  Serial.println("Ontvangen waarde: ");
  Serial.println(api_key_input.getValue());
  EEPROM.writeString(17, String(api_key_input.getValue()));
  EEPROM.commit();
  key = String(api_key_input.getValue());
  if (!wc.autoConnect()) {
    Serial.println("Geen verbinding, opnieuw instellen");
  }
}

void getPropagation() {
  Serial.println("Data ophalen van https://hamqsl.com/solarxml.php");
  HTTPClient http;
  http.setTimeout(2000);
  http.begin("https://hamqsl.com/solarxml.php");
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String xmlContent = http.getString();
    int startPos, endPos;


    startPos = xmlContent.indexOf("<solarflux>") + 11;
    endPos = xmlContent.indexOf("</solarflux>");
    solarflux = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<aindex>") + 8;
    endPos = xmlContent.indexOf("</aindex>");
    aindex = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<kindex>") + 8;
    endPos = xmlContent.indexOf("</kindex>");
    kindex = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<sunspots>") + 10;
    endPos = xmlContent.indexOf("</sunspots>");
    sunspots = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<protonflux>") + 12;
    endPos = xmlContent.indexOf("</protonflux>");
    protonflux = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<electonflux>") + 13;
    endPos = xmlContent.indexOf("</electonflux>");
    electonflux = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<aurora>") + 8;
    endPos = xmlContent.indexOf("</aurora>");
    aurora = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<normalization>") + 15;
    endPos = xmlContent.indexOf("</normalization>");
    normalisation = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<latdegree>") + 11;
    endPos = xmlContent.indexOf("</latdegree>");
    latdegree = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<solarwind>") + 11;
    endPos = xmlContent.indexOf("</solarwind>");
    solarwind = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<magneticfield>") + 15;
    endPos = xmlContent.indexOf("</magneticfield>");
    magneticfield = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<calculatedconditions>");
    endPos = xmlContent.indexOf("</calculatedconditions>");
    String bandConditions = xmlContent.substring(startPos, endPos);


    int bandStartPos = 0;
    int bandEndPos = 0;
    String dayBandName, nightBandName, bandName, bandTime, bandStatus;

    while (true) {
      bandStartPos = bandConditions.indexOf("<band", bandEndPos);
      if (bandStartPos == -1) {
        break;
      }

      bandStartPos = bandConditions.indexOf("name=\"", bandStartPos) + 6;
      bandEndPos = bandConditions.indexOf("\"", bandStartPos);
      bandName = bandConditions.substring(bandStartPos, bandEndPos);

      bandStartPos = bandConditions.indexOf("time=\"", bandEndPos) + 6;
      bandEndPos = bandConditions.indexOf("\"", bandStartPos);
      bandTime = bandConditions.substring(bandStartPos, bandEndPos);

      bandStartPos = bandConditions.indexOf(">", bandEndPos) + 1;
      bandEndPos = bandConditions.indexOf("</band>", bandStartPos);
      bandStatus = bandConditions.substring(bandStartPos, bandEndPos);


      if (bandTime.equals("day")) {
        dayBandName = bandName;
      } else if (bandTime.equals("night")) {
        nightBandName = bandName;
      }


      if (bandName.equals("12m-10m") && bandTime.equals("day")) {
        d1210m = bandStatus;
      } else if (bandName.equals("17m-15m") && bandTime.equals("day")) {
        d1715m = bandStatus;
      } else if (bandName.equals("30m-20m") && bandTime.equals("day")) {
        d3020m = bandStatus;
      } else if (bandName.equals("80m-40m") && bandTime.equals("day")) {
        d8040m = bandStatus;
      } else if (bandName.equals("12m-10m") && bandTime.equals("night")) {
        n1210m = bandStatus;
      } else if (bandName.equals("17m-15m") && bandTime.equals("night")) {
        n1715m = bandStatus;
      } else if (bandName.equals("30m-20m") && bandTime.equals("night")) {
        n3020m = bandStatus;
      } else if (bandName.equals("80m-40m") && bandTime.equals("night")) {
        n8040m = bandStatus;
      }
    }


    startPos = xmlContent.indexOf("<calculatedvhfconditions>");
    endPos = xmlContent.indexOf("</calculatedvhfconditions>");
    String vhfConditions = xmlContent.substring(startPos, endPos);


    int phenomenonStartPos = 0;
    int phenomenonEndPos = 0;
    String phenomenonName, phenomenonLocation, phenomenonStatus;

    while (true) {
      phenomenonStartPos = vhfConditions.indexOf("<phenomenon", phenomenonEndPos);
      if (phenomenonStartPos == -1) {
        break;
      }

      phenomenonStartPos = vhfConditions.indexOf("name=\"", phenomenonStartPos) + 6;
      phenomenonEndPos = vhfConditions.indexOf("\"", phenomenonStartPos);
      phenomenonName = vhfConditions.substring(phenomenonStartPos, phenomenonEndPos);

      phenomenonStartPos = vhfConditions.indexOf("location=\"", phenomenonEndPos) + 10;
      phenomenonEndPos = vhfConditions.indexOf("\"", phenomenonStartPos);
      phenomenonLocation = vhfConditions.substring(phenomenonStartPos, phenomenonEndPos);

      phenomenonStartPos = vhfConditions.indexOf(">", phenomenonEndPos) + 1;
      phenomenonEndPos = vhfConditions.indexOf("</phenomenon>", phenomenonStartPos);
      phenomenonStatus = vhfConditions.substring(phenomenonStartPos, phenomenonEndPos);


      if (phenomenonLocation.equals("europe") && phenomenonName.equals("E-Skip")) {
        es2m = phenomenonStatus;
      } else if (phenomenonLocation.equals("europe_4m") && phenomenonName.equals("E-Skip")) {
        es4m = phenomenonStatus;
      } else if (phenomenonLocation.equals("europe_6m") && phenomenonName.equals("E-Skip")) {
        es6m = phenomenonStatus;
      }
    }


    startPos = xmlContent.indexOf("<geomagfield>") + 13;
    endPos = xmlContent.indexOf("</geomagfield>");
    geomagfield = xmlContent.substring(startPos, endPos);


    startPos = xmlContent.indexOf("<signalnoise>") + 13;
    endPos = xmlContent.indexOf("</signalnoise>");
    signalnoise = xmlContent.substring(startPos, endPos);
  } else {
    Serial.printf("Verbinding met https://hamqsl.com/solarxml.php mislukt! Error code: %d", httpCode);
  }
}

byte imageFromString(String img) {
  if (img == "zonnig") return 16;
  if (img == "bliksem") return 17;
  if (img == "regen") return 18;
  if (img == "buien") return 19;
  if (img == "hagel") return 20;
  if (img == "mist") return 21;
  if (img == "sneeuw") return 22;
  if (img == "bewolkt") return 23;
  if (img == "lichtbewolkt") return 24;
  if (img == "halfbewolkt") return 25;
  if (img == "halfbewolkt_regen") return 26;
  if (img == "zwaarbewolkt") return 27;
  if (img == "nachtmist") return 28;
  if (img == "helderenacht") return 29;
  if (img == "wolkennacht" || img == "nachtbewolkt") return 30;
  return 16;
}

void getWeather() {

  WiFiClientSecure http;
  HTTPClient https;
  http.setInsecure();
  http.setTimeout(2000);


  String request = "/api/weerlive_api_v2.php?key=" + key + "&locatie=" + String(latitude, 6) + "," + String(longitude, 6);


  if (!http.connect("weerlive.nl", 443)) return;

  https.begin(http, "weerlive.nl", 443, request);
  int httpCode = https.GET();
  if (httpCode != HTTP_CODE_OK) {
    https.end();
    return;
  }



  StaticJsonDocument<200> filter;
  filter["liveweer"][0] = true;
  filter["wk_verw"] = true;

  DynamicJsonDocument weer(4096);
  DeserializationError error = deserializeJson(weer, https.getStream(), DeserializationOption::Filter(filter));
  if (error) {
    https.end();
    return;
  }


  JsonObject live = weer["liveweer"][0];
  JsonArray wk_verw = weer["wk_verw"];


  plaats = live["plaats"] | "";
  temp = int(live["temp"].as<float>() * 10);
  samenv = live["samenv"] | "";
  lv = live["lv"] | 0;
  dauw = int(live["dauwp"].as<float>() * 10);
  windr = live["windr"] | "";
  winds = live["windbft"] | 0;
  windkmh = int(live["windkmh"].as<float>() * 10);
  windms = int(live["windms"].as<float>() * 10);
  luchtd = int(live["luchtd"].as<float>() * 10);
  zicht = live["zicht"] | 0;
  verw = live["verw"] | "";
  zonop = live["sup"] | "";
  zononder = live["sunder"] | "";
  alarmtxt = live["ltekst"] | "";


  weercode = live["wrschklr"] | "groen";
  weeralarm = (weercode != "groen");


  image = imageFromString(live["image"] | "");


  if (wk_verw.size() > 0) {
    d0tmin = int(wk_verw[0]["min_temp"].as<int>());
    d0tmax = int(wk_verw[0]["max_temp"].as<int>());
  }
  if (wk_verw.size() > 1) {
    d1tmin = int(wk_verw[1]["min_temp"].as<int>());
    d1tmax = int(wk_verw[1]["max_temp"].as<int>());
    d1windk = wk_verw[1]["windbft"] | 0;
    d1windr = wk_verw[1]["windr"] | "";
    d1zon = wk_verw[1]["zond_perc_dag"] | 0;
    d1neerslag = wk_verw[1]["neersl_perc_dag"] | 0;
    d1image = imageFromString(wk_verw[1]["image"] | "") + 15;
  }
  if (wk_verw.size() > 2) {
    d2tmin = int(wk_verw[2]["min_temp"].as<int>());
    d2tmax = int(wk_verw[2]["max_temp"].as<int>());
    d2windk = wk_verw[2]["windbft"] | 0;
    d2windr = wk_verw[2]["windr"] | "";
    d2zon = wk_verw[2]["zond_perc_dag"] | 0;
    d2neerslag = wk_verw[2]["neersl_perc_dag"] | 0;
    d2image = imageFromString(wk_verw[2]["image"] | "") + 15;
  }


  if (windr == "Noord") windr = "N";
  else if (windr == "Oost") windr = "O";
  else if (windr == "Zuid") windr = "Z";
  else if (windr == "West") windr = "W";

  https.end();
}

void getMUF(void) {
  HTTPClient http;


  http.setTimeout(2000);

  String url = "http://ionosphere.meteo.be/ionosphere/MUF/latest-MUF-DB049.php";


  if (!http.begin(url)) {
    Serial.println("HTTP begin failed");
    return;
  }


  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {

    String payload = http.getString();


    int startIndex = payload.indexOf("MUF(MHz)");
    if (startIndex != -1) {

      int lineEnd = payload.indexOf('\n', startIndex);
      String mufData = payload.substring(lineEnd + 1);

      int lineCount = 0;


      while (lineCount < 9) {
        int nextLine = mufData.indexOf('\n');
        if (nextLine == -1) break;


        String line = mufData.substring(0, nextLine);
        line.trim();


        if (line.length() > 5) {

          muf_string[lineCount] = line.substring(5);
        } else {

          muf_string[lineCount] = "0";
        }


        mufData = mufData.substring(nextLine + 1);
        lineCount++;
      }
    } else {
      Serial.println("MUF header niet gevonden!");

      for (int i = 0; i < 9; i++) {
        muf_string[i] = "0";
      }
    }
  } else {

    Serial.printf("HTTP GET mislukt. Code: %d\n", httpCode);

    for (int i = 0; i < 9; i++) {
      muf_string[i] = "0";
    }
  }


  http.end();
}

void showData(void) {
  if (lightningtextmerge != lightningtextmergeold) {
    if (display_alarm) Display.writeStr("lightning.txt", lightningtextmerge);
    lightningtextmergeold = lightningtextmerge;
  }

  if (solarflux != solarfluxold) {
    Serial.print("Solarflux               : ");
    Serial.println(solarflux);
    if (display_radio) Display.writeStr("solarflux.txt", solarflux); delay(10);
    solarfluxold = solarflux;
  }
  if (aindex != aindexold) {
    Serial.print("A-index                 : ");
    Serial.println(aindex);
    if (display_radio) Display.writeStr("aindex.txt", aindex); delay(10);
    aindexold = aindex;
  }
  if (kindex != kindexold) {
    Serial.print("K-index                 : ");
    Serial.println(kindex);
    if (display_radio) Display.writeStr("kindex.txt", kindex); delay(10);
    kindexold = kindex;
  }
  if (sunspots != sunspotsold) {
    Serial.print("Aantal zonnevlekken     : ");
    Serial.println(sunspots);
    if (display_radio) Display.writeStr("sunspots.txt", sunspots); delay(10);
    sunspotsold = sunspots;
  }
  if (protonflux != protonfluxold) {
    Serial.print("Protonflux              : ");
    Serial.println(protonflux);
    if (display_radio) Display.writeStr("protonflux.txt", protonflux); delay(10);
    protonfluxold = protonflux;
  }
  if (electonflux != electonfluxold) {
    Serial.print("Electonflux             : ");
    Serial.println(electonflux);
    if (display_radio) Display.writeStr("electonflux.txt", electonflux); delay(10);
    electonfluxold = electonflux;
  }
  if (aurora != auroraold) {
    Serial.print("Aurora                  : ");
    Serial.println(aurora);
    if (display_radio) Display.writeStr("aurora.txt", aurora); delay(10);
    auroraold = aurora;
  }
  if (normalisation != normalisationold) {
    Serial.print("Aurora normalisatie     : ");
    Serial.println(normalisation);
    if (display_radio) Display.writeStr("normalisation.txt", normalisation); delay(10);
    normalisationold = normalisation;
  }
  if (latdegree != latdegreeold) {
    Serial.print("Aurora breedtegraad     : ");
    Serial.println(latdegree);
    if (display_radio) Display.writeStr("latdegree.txt", latdegree); delay(10);
    latdegreeold = latdegree;
  }
  if (solarwind != solarwindold) {
    Serial.print("Zonnewind               : ");
    Serial.println(solarwind);
    if (display_radio) Display.writeStr("solarwind.txt", solarwind); delay(10);
    solarwindold = solarwind;
  }
  if (magneticfield != magneticfieldold) {
    Serial.print("Magnetisch veld         : ");
    Serial.println(magneticfield);
    if (display_radio) Display.writeStr("magneticfield.txt", magneticfield); delay(10);
    magneticfieldold = magneticfield;
  }
  if (geomagfield != geomagfieldold) {
    Serial.print("Geomagnetisch veld      : ");
    Serial.println(geomagfield);
    if (display_radio) Display.writeStr("geomagfield.txt", geomagfield); delay(10);
    geomagfieldold = geomagfield;
  }
  if (d8040m != d8040mold) {
    Serial.print("80-40m overdag          : ");
    Serial.println(d8040m);
    if (display_radio) Display.writeStr("d8040.txt", d8040m); delay(10);
    d8040mold = d8040m;
  }
  if (n8040m != n8040mold) {
    Serial.print("80-40m 's nachts        : ");
    Serial.println(n8040m);
    if (display_radio) Display.writeStr("n8040.txt", n8040m); delay(10);
    n8040mold = n8040m;
  }
  if (d3020m != d3020mold) {
    Serial.print("30-20m overdag          : ");
    Serial.println(d3020m);
    if (display_radio) Display.writeStr("d3020.txt", d3020m); delay(10);
    d3020mold = d3020m;
  }
  if (n3020m != n3020mold) {
    Serial.print("30-20m 's nachts        : ");
    Serial.println(n3020m);
    if (display_radio) Display.writeStr("n3020.txt", n3020m); delay(10);
    n3020mold = n3020m;
  }
  if (d1715m != d1715mold) {
    Serial.print("17-15m overdag          : ");
    Serial.println(d1715m);
    if (display_radio) Display.writeStr("d1715.txt", d1715m); delay(10);
    d1715mold = d1715m;
  }
  if (n1715m != n1715mold) {
    Serial.print("17-15m 's nachts        : ");
    Serial.println(n1715m);
    if (display_radio) Display.writeStr("n1715.txt", n1715m); delay(10);
    n1715mold = n1715m;
  }
  if (d1210m != d1210mold) {
    Serial.print("12-10m overdag          : ");
    Serial.println(d1210m);
    if (display_radio) Display.writeStr("d1210.txt", d1210m); delay(10);
    d1210mold = d1210m;
  }
  if (n1210m != n1210mold) {
    Serial.print("12-10m 's nachts        : ");
    Serial.println(n1210m);
    if (display_radio) Display.writeStr("n1210.txt", n1210m); delay(10);
    n1210mold = n1210m;
  }
  if (signalnoise != signalnoiseold) {
    Serial.print("HF ruisvloer            : ");
    Serial.println(signalnoise);
    if (display_radio) Display.writeStr("signalnoise.txt", signalnoise); delay(10);
    signalnoiseold = signalnoise;
  }
  if (es2m != es2mold) {
    Serial.print("Sporadische E 2m        : ");
    Serial.println(es2m);
    if (display_radio) Display.writeStr("vhf2.txt", es2m); delay(10);
    es2mold = es2m;
  }
  if (es6m != es6mold) {
    Serial.print("Sporadische E 6m        : ");
    Serial.println(es6m);
    if (display_radio) Display.writeStr("vhf6.txt", es6m); delay(10);
    es6mold = es6m;
  }
  if (es4m != es4mold) {
    Serial.print("Sporadische E 4m        : ");
    Serial.println(es4m);
    if (display_radio) Display.writeStr("vhf4.txt", es4m); delay(10);
    es4mold = es4m;
  }
  if (muf_string[8] != muf_stringold[8]) {
    Serial.print("MUF < 100km             : ");
    Serial.println(muf_string[0]);
    buildMUFticker();
    muf_stringold[0] = muf_string[0];
  }
  if (muf_string[1] != muf_stringold[1]) {
    Serial.print("MUF 100-200km           : ");
    Serial.println(muf_string[1]);
    buildMUFticker();
    muf_stringold[1] = muf_string[1];
  }
  if (muf_string[2] != muf_stringold[2]) {
    Serial.print("MUF 200-400km           : ");
    Serial.println(muf_string[2]);
    buildMUFticker();
    muf_stringold[2] = muf_string[2];
  }
  if (muf_string[3] != muf_stringold[3]) {
    Serial.print("MUF 400-600km           : ");
    Serial.println(muf_string[3]);
    buildMUFticker();
    muf_stringold[3] = muf_string[3];
  }
  if (muf_string[4] != muf_stringold[4]) {
    Serial.print("MUF 600-800km           : ");
    Serial.println(muf_string[4]);
    buildMUFticker();
    muf_stringold[4] = muf_string[4];
  }
  if (muf_string[5] != muf_stringold[5]) {
    Serial.print("MUF 800-1000km          : ");
    Serial.println(muf_string[5]);
    buildMUFticker();
    muf_stringold[5] = muf_string[5];
  }
  if (muf_string[6] != muf_stringold[6]) {
    Serial.print("MUF 1000-1500km         : ");
    Serial.println(muf_string[6]);
    buildMUFticker();
    muf_stringold[6] = muf_string[6];
  }
  if (muf_string[7] != muf_stringold[7]) {
    Serial.print("MUF 1500-3000km         : ");
    Serial.println(muf_string[7]);
    buildMUFticker();
    muf_stringold[7] = muf_string[7];
  }
  if (muf_string[8] != muf_stringold[8]) {
    Serial.print("MUF >3000km             : ");
    Serial.println(muf_string[8]);
    buildMUFticker();
    muf_stringold[8] = muf_string[8];
  }

  if (plaats != plaatsold || String(get_mh(latitude, longitude, 10)) != qthold) {
    Serial.print("Locatie                 : ");
    Serial.println(plaats);
    Display.writeStr("plaats.txt", plaats + (radio ? " " + String(get_mh(latitude, longitude, 10)) : ""));
    plaatsold = plaats;
  }

  if (String(get_mh(latitude, longitude, 10)) != qthold) {
    Serial.print("QTH locator             : ");
    Serial.println(get_mh(latitude, longitude, 10));
    qthold = String(get_mh(latitude, longitude, 10));
  }

  if (weercode != weercodeold) {
    Serial.print("Weercode                : ");
    Serial.println(weercode);

    if (display_weather) {
      if (weercode == "groen") Display.writeNum("weercode.pic", 69);
      else if (weercode == "geel") Display.writeNum("weercode.pic", 70);
      else if (weercode == "oranje") Display.writeNum("weercode.pic", 71);
      else if (weercode == "rood") Display.writeNum("weercode.pic", 72);
    }

    weercodeold = weercode;
  }

  if (temp != tempold) {
    Serial.print("Temperatuur (/10)       : ");
    Serial.println(temp);
    if (display_weather) Display.writeNum("actualtemp.val", temp);
    tempold = temp;
  }
  if (samenv != samenvold) {
    Serial.print("Actueel weerbeeld       : ");
    Serial.println(samenv);
    if (display_weather) Display.writeStr("actualtext.txt", samenv);
    samenvold = samenv;
  }
  if (lv != lvold) {
    Serial.print("Luchtvochtigheid        : ");
    Serial.println(lv);
    if (display_weather) Display.writeNum("luchtv.val", lv);
    lvold = lv;
  }
  if (dauw != dauwold) {
    Serial.print("Dauwpunt (/10)          : ");
    Serial.println(dauw);
    if (display_weather) Display.writeNum("dauwp.val", dauw);
    dauwold = dauw;
  }
  if (zonop != zonopold) {
    Serial.print("Zon op                  : ");
    Serial.println(zonop);
    if (display_weather) Display.writeStr("zonop.txt", zonop);
    zonopold = zonop;
  }
  if (zononder != zononderold) {
    Serial.print("Zon onder               : ");
    Serial.println(zononder);
    if (display_weather) Display.writeStr("zononder.txt", zononder);
    zononderold = zononder;
  }
  if (windr != windrold) {
    Serial.print("Windrichting            : ");
    Serial.println(windr);
    if (display_weather) {
      if (windr == "N") Display.writeNum("actualwindr.pic", 0);
      if (windr == "NNO") Display.writeNum("actualwindr.pic", 1);
      if (windr == "NO") Display.writeNum("actualwindr.pic", 2);
      if (windr == "ONO") Display.writeNum("actualwindr.pic", 3);
      if (windr == "O") Display.writeNum("actualwindr.pic", 4);
      if (windr == "OZO") Display.writeNum("actualwindr.pic", 5);
      if (windr == "ZO") Display.writeNum("actualwindr.pic", 6);
      if (windr == "ZZO") Display.writeNum("actualwindr.pic", 7);
      if (windr == "Z") Display.writeNum("actualwindr.pic", 8);
      if (windr == "ZZW") Display.writeNum("actualwindr.pic", 9);
      if (windr == "ZW") Display.writeNum("actualwindr.pic", 10);
      if (windr == "WZW") Display.writeNum("actualwindr.pic", 11);
      if (windr == "W") Display.writeNum("actualwindr.pic", 12);
      if (windr == "WNW") Display.writeNum("actualwindr.pic", 13);
      if (windr == "NW") Display.writeNum("actualwindr.pic", 14);
      if (windr == "NNW") Display.writeNum("actualwindr.pic", 15);

    }
    windrold = windr;
  }
  if (winds != windsold) {
    Serial.print("Windkracht bft          : ");
    Serial.println(winds);
    if (display_weather) Display.writeNum("actualbft.val", winds);
    windsold = winds;
  }
  if (windkmh != windkmhold) {
    Serial.print("Windsnelheid km/u (/10) : ");
    Serial.println(windkmh);
    if (display_weather && kmu) Display.writeNum("actualwindkmh.val", windkmh);
    windkmhold = windkmh;
  }
  if (windms != windmsold) {
    Serial.print("Windsnelheid m/s        : ");
    Serial.println(windms);
    if (display_weather && !kmu) Display.writeNum("actualwindkmh.val", windms);
    windmsold = windms;
  }
  if (luchtd != luchtdold) {
    Serial.print("Luchtdruk (/10)         : ");
    Serial.println(luchtd);
    if (display_weather) Display.writeNum("luchtd.val", luchtd);
    luchtdold = luchtd;
  }
  if (zicht != zichtold) {
    Serial.print("Zicht in km.            : ");
    Serial.println(zicht);
    zichtold = zicht;
  }
  if (verw != verwold) {
    Serial.print("Weersverwachting        : ");
    Serial.println(verw);
    if (display_weather) Display.writeStr("d0text.txt", verw);
    verwold = verw;
  }
  if (image != imageold) {
    Serial.print("Weer icoon              : ");
    Serial.println(image);
    if (display_weather) Display.writeNum("weerimage.pic", image);
    imageold = image;
  }
  if (weeralarm != weeralarmold) {
    Serial.print("Waarschuwing            : ");
    Serial.println(weeralarm);
    if (display_weather) {
      if (weeralarm) {
        Display.writeNum("b_alarm.pic", 49);
        if (!beeper_alarm) {
          buzzer(1);
          beeper_alarm = true;
        }
      } else {
        Display.writeNum("b_alarm.pic", 67);
        beeper_alarm = false;
      }
    }
    weeralarmold = weeralarm;
  }
  if (alarmtxt != alarmtxtold) {
    Serial.print("Waarschuwingstekst      : ");
    Serial.println(alarmtxt);
    alarmtxtold = alarmtxt;
  }
  if (d1zon != d1zonold) {
    Serial.print("Kans op zon morgen      : ");
    Serial.println(d1zon);
    if (display_weather) Display.writeNum("d1zon.val", d1zon);
    d1zonold = d1zon;
  }
  if (d1neerslag != d1neerslagold) {
    Serial.print("Kans op regen morgen    : ");
    Serial.println(d1neerslag);
    if (display_weather) Display.writeNum("d1neerslag.val", d1neerslag);
    d1neerslagold = d1neerslag;
  }
  if (d2zon != d2zonold) {
    Serial.print("Kans op zon overmorgen  : ");
    Serial.println(d2zon);
    if (display_weather) Display.writeNum("d2zon.val", d2zon);
    d2zonold = d2zon;
  }
  if (d2neerslag != d2neerslagold) {
    Serial.print("Kans op regen overmorgen: ");
    Serial.println(d2neerslag);
    if (display_weather) Display.writeNum("d2neerslag.val", d2neerslag);
    d2neerslagold = d2neerslag;
  }
  if (d1image != d1imageold) {
    Serial.print("Weer icoon morgen       : ");
    Serial.println(d1image);
    if (display_weather) Display.writeNum("d1image.pic", d1image);
    d1imageold = d1image;
  }
  if (d2image != d2imageold) {
    Serial.print("Weer icoon overmorgen   : ");
    Serial.println(d2image);
    if (display_weather) Display.writeNum("d2image.pic", d2image);
    d2imageold = d2image;
  }
  if (d0tmin != d0tminold) {
    Serial.print("Minimum temp. vandaag   : ");
    Serial.println(d0tmin);
    if (display_weather) Display.writeNum("d0min.val", d0tmin);
    d0tminold = d0tmin;
  }
  if (d0tmax != d0tmaxold) {
    Serial.print("Maximum temp. Vandaag   : ");
    Serial.println(d0tmax);
    if (display_weather) Display.writeNum("d0max.val", d0tmax);
    d0tmaxold = d0tmax;
  }
  if (d1tmin != d1tminold || d1tmax != d1tmaxold) {
    Serial.print("Min/max temp. morgen    : ");
    if (display_weather) Display.writeStr("d1temp.txt", (String(d1tmin) + "/" + String(d1tmax) + "\xB0"));
    Serial.println(String(d1tmin) + "/" + String(d1tmax) + "°");
    d1tminold = d1tmin;
    d1tmaxold = d1tmax;
  }
  if (d2tmin != d2tminold || d2tmax != d2tmaxold) {
    Serial.print("Min/max temp. morgen    : ");
    if (display_weather) Display.writeStr("d2temp.txt", (String(d2tmin) + "/" + String(d2tmax)) + "\xB0");
    Serial.println(String(d2tmin) + "/" + String(d2tmax) + "°");
    d2tminold = d2tmin;
    d2tmaxold = d2tmax;
  }
  if (d1windk != d1windkold) {
    Serial.print("Windkracht morgen bft.  : ");
    Serial.println(d1windk);
    if (display_weather) Display.writeNum("d1windk.val", d1windk);
    d1windkold = d1windk;
  }
  if (d2windk != d2windkold) {
    Serial.print("Windkr. overmorgen bft. : ");
    Serial.println(d2windk);
    if (display_weather) Display.writeNum("d2windk.val", d2windk);
    d2windkold = d2windk;
  }
  if (d1windr != d1windrold) {
    Serial.print("Windrichting morgen     : ");
    if (display_weather) {
      if (d1windr == "N") Display.writeNum("d1windr.pic", 0);
      if (d1windr == "NNO") Display.writeNum("d1windr.pic", 1);
      if (d1windr == "NO") Display.writeNum("d1windr.pic", 2);
      if (d1windr == "ONO") Display.writeNum("d1windr.pic", 3);
      if (d1windr == "O") Display.writeNum("d1windr.pic", 4);
      if (d1windr == "OZO") Display.writeNum("d1windr.pic", 5);
      if (d1windr == "ZO") Display.writeNum("d1windr.pic", 6);
      if (d1windr == "ZZO") Display.writeNum("d1windr.pic", 7);
      if (d1windr == "Z") Display.writeNum("d1windr.pic", 8);
      if (d1windr == "ZZW") Display.writeNum("d1windr.pic", 9);
      if (d1windr == "ZW") Display.writeNum("d1windr.pic", 10);
      if (d1windr == "WZW") Display.writeNum("d1windr.pic", 11);
      if (d1windr == "W") Display.writeNum("d1windr.pic", 12);
      if (d1windr == "WNW") Display.writeNum("d1windr.pic", 13);
      if (d1windr == "NW") Display.writeNum("d1windr.pic", 14);
      if (d1windr == "NNW") Display.writeNum("d1windr.pic", 15);

    }
    Serial.println(d1windr);
    d1windrold = d1windr;
  }
  if (d2windr != d2windrold) {
    Serial.print("Windrichting overmorgen : ");
    if (display_weather) {
      if (d2windr == "N") Display.writeNum("d2windr.pic", 0);
      if (d2windr == "NNO") Display.writeNum("d2windr.pic", 1);
      if (d2windr == "NO") Display.writeNum("d2windr.pic", 2);
      if (d2windr == "ONO") Display.writeNum("d2windr.pic", 3);
      if (d2windr == "O") Display.writeNum("d2windr.pic", 4);
      if (d2windr == "OZO") Display.writeNum("d2windr.pic", 5);
      if (d2windr == "ZO") Display.writeNum("d2windr.pic", 6);
      if (d2windr == "ZZO") Display.writeNum("d2windr.pic", 7);
      if (d2windr == "Z") Display.writeNum("d2windr.pic", 8);
      if (d2windr == "ZZW") Display.writeNum("d2windr.pic", 9);
      if (d2windr == "ZW") Display.writeNum("d2windr.pic", 10);
      if (d2windr == "WZW") Display.writeNum("d2windr.pic", 11);
      if (d2windr == "W") Display.writeNum("d2windr.pic", 12);
      if (d2windr == "WNW") Display.writeNum("d2windr.pic", 13);
      if (d2windr == "NW") Display.writeNum("d2windr.pic", 14);
      if (d2windr == "NNW") Display.writeNum("d2windr.pic", 15);

    }
    Serial.println(d2windr);
    d2windrold = d2windr;
  }

  if (indoortemp != indoortempold) {
    Serial.print("Temperatuur (/10)       : ");
    Serial.println(indoortemp);
    if (display_weather && indoortemp != -9990) Display.writeNum("indoortemp.val", indoortemp);
    indoortempold = indoortemp;
  }
  if (indoorhum != indoorhumold) {
    Serial.print("Luchtvochtigheid (/10)  : ");
    Serial.println(indoorhum);
    if (display_weather && indoorhum != -9990) Display.writeNum("indoorhum.val", indoorhum);
    indoorhumold = indoorhum;
  }
  if (indoorvoc != indoorvocold) {
    Serial.print("VOC                     : ");
    Serial.println(indoorvoc);
    if (display_weather) Display.writeNum("vocgauge.val", indoorvoc / 2.77);
    if (display_weather) Display.writeNum("voc.val", indoorvoc);
    if (display_weather) Display.writeStr("voctxt.txt", "VOC");
    indoorvocold = indoorvoc;
  }

  if (String(br_tijd[0]) != br_tijdold) {
    if (display_weather) Display.writeStr("br_mmu.txt", (br_mmpu[0] > 0 ? String(float(pow(10, (float(br_mmpu[0]) - 109) / 32)), 1) : "0.0") + "mm/u");

    int max_value = br_mmpu[0];
    for (int i = 1; i < 24; i++) {
      if (br_mmpu[i] > max_value) {
        max_value = br_mmpu[i];
      }
    }

    int new_array[24];
    for (int i = 0; i < 24; i++) {
      if (max_value == 0) {
        new_array[i] = 0;
      } else {
        new_array[i] = map(br_mmpu[i], 0, max_value, 0, 24);
      }
    }

    if (display_weather) {
      for (int i = 0; i < 24; i++) {
        Display.writeStr("fill " + String(i + 48) + ",88,1," + String(24 - new_array[i]) + ",33840");

        Display.writeStr("fill " + String(i + 48) + "," + String(112 - new_array[i]) + ",1," + String(new_array[i]) + ",26271");

      }
      Display.writeStr("line 48,113,72,113,26271");
      br_tijdold = String(br_tijd[0]);
    }
  }
}

void buildMUFticker() {
  if (display_radio) {
    Display.writeStr("muf.txt", String("MUF QRB: <100km: " + String(muf_string[0]) + "MHz  100-200km: " + String(muf_string[1]) + "MHz  200-400km: " + String(muf_string[2]) + "MHz  400-600km: " + String(muf_string[3]) + "MHz  600-800km: " + String(muf_string[4]) + "MHz  800-1000km: " + String(muf_string[5]) + "MHz  1000-1500km: " + String(muf_string[6]) + "MHz  1500-3000km: " + String(muf_string[7]) + "MHz  >3000km: " + String(muf_string[8]) + "MHz     ")); delay(10);
  }
}

void showGPS() {
  if (String(gps.location.lat(), 6) != latitudeold) {
    Serial.print("Breedtegraad            : ");
    Serial.println(gps.location.lat(), 6);
    latitudeold = String(gps.location.lat(), 6);
  }
  if (String(gps.location.lng(), 6) != longitudeold) {
    Serial.print("Lengtegraad             : ");
    Serial.println(gps.location.lng(), 6);
    longitudeold = String(gps.location.lng(), 6);
  }

  if (display_gps) {
    if (gps.location.isValid()) {
      Display.writeStr("locator.txt", String(get_mh(latitude, longitude, 10)));
      Display.writeStr("longitude.txt", String(gps.location.lng(), 10));
      Display.writeStr("latitude.txt", String(gps.location.lat(), 10));
      Display.writeStr("height.txt", String(gps.altitude.meters()) + "m");
      Display.writeStr("hdop.txt", String(gps.hdop.hdop()));
      Display.writeStr("sats.txt", String(gps.satellites.value()));
    } else {
      Display.writeStr("locator.txt", "Geen data");
      Display.writeStr("longitude.txt", "Geen data");
      Display.writeStr("latitude.txt", "Geen data");
      Display.writeStr("height.txt", "Geen data");
      Display.writeStr("hdop.txt", "Geen data");
      Display.writeStr("utctime.txt", "Geen data");
      Display.writeStr("sats.txt", "Geen data");
    }
  }
}

void getIndoor() {
  indoorhum = dht.readHumidity() * 10;
  indoortemp = (dht.readTemperature() * 10) + indooroffset;
  if (indoorhum != -9990 || indoortemp != -9990) mySgp40.setRhT(indoorhum / 10, indoortemp / 10);
  indoorvoc = mySgp40.getVoclndex();
}

void getGPS() {
  while (ss.available() > 0) {
    gps.encode(ss.read());

    if (gps.location.isValid())
    {
      if (autoloc && gps.location.lng() > 3.4 && gps.location.lng() < 7.3 && gps.location.lat() > 50.7 && gps.location.lat() < 53.6) {
        longitude = gps.location.lng();
        latitude = gps.location.lat();
        EEPROM.writeDouble(1, longitude);
        EEPROM.writeDouble(9, latitude);
        EEPROM.commit();
      }
    }
    if (!ntp) {
      if (gps.time.isValid() && gps.date.isValid()) {

        struct tm gpsUtc = {};
        gpsUtc.tm_year = gps.date.year() - 1900;
        gpsUtc.tm_mon = gps.date.month() - 1;
        gpsUtc.tm_mday = gps.date.day();
        gpsUtc.tm_hour = gps.time.hour();
        gpsUtc.tm_min = gps.time.minute();
        gpsUtc.tm_sec = gps.time.second();

        time_t epoch = mktime(&gpsUtc);
        struct tm *check = localtime(&epoch);
        epoch -= (mktime(check) - mktime(gmtime(&epoch)));
        struct timeval tv = { .tv_sec = epoch, .tv_usec = 0 };
        settimeofday(&tv, NULL);
      }
    }

  }
  FormatTime();
}

void FormatTime() {

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  const char* dagen[] = {"Zondag", "Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrijdag", "Zaterdag"};
  const char* dagen_kort[] = {"Zo", "Ma", "Di", "Wo", "Do", "Vr", "Za"};
  const char* maanden[] = {"jan", "feb", "mrt", "apr", "mei", "jun", "jul", "aug", "sep", "okt", "nov", "dec"};

  String dayname = dagen[t->tm_wday];
  String dayname1 = dagen_kort[(t->tm_wday + 1) % 7];
  String dayname2 = dagen_kort[(t->tm_wday + 2) % 7];
  String monthname = maanden[t->tm_mon];

  char tijdbuf[40];
  sprintf(tijdbuf, "%s %d %s %d %d:%02d:%02d ", dayname.c_str(), t->tm_mday, monthname.c_str(), t->tm_year + 1900, t->tm_hour, t->tm_min, t->tm_sec);
  datetime = String(tijdbuf);

  sprintf(tijdbuf, "%d/%d %d:%02d ", t->tm_mday, t->tm_mon + 1, t->tm_hour, t->tm_min);
  datetimestamp = String(tijdbuf);

  if (gps.location.isValid() || ntp)
  {
    if (datetime != datetimeold) {
      Display.writeStr("tijd.txt", datetime);
      datetimeold = datetime;
    }
    if (dayname1 != dayname1old) {
      if (display_weather) Display.writeStr("d1name.txt", dayname1);
      dayname1old = dayname1;
    }
    if (dayname2 != dayname2old) {
      if (display_weather) Display.writeStr("d2name.txt", dayname2);
      dayname2old = dayname2;
    }
    if (gps.location.isValid() && display_gps) Display.writeStr("utctime.txt", String(gps.time.hour()) + ":" + (gps.time.minute() < 10 ? "0" : "") + String(gps.time.minute()) + ":" + (gps.time.second() < 10 ? "0" : "") + String(gps.time.second()));
  }
}

void buzzer(byte times) {
  if (beeper) {
    for (int x = -1; x < times; x++) {
      digitalWrite(33, HIGH);
      delay(100);
      digitalWrite(33, LOW);
      delay(100);
    }
  }
}

void defaults() {
  EEPROM.writeDouble(1, 4.891256);
  EEPROM.writeDouble(9, 52.37393);
  EEPROM.writeString(17, "demo");
  EEPROM.writeByte(29, 1);
  EEPROM.writeByte(30, 1);
  EEPROM.writeByte(31, 0);
  EEPROM.writeByte(32, 1);
  EEPROM.writeByte(33, 1);
  EEPROM.writeByte(34, 1);
  EEPROM.writeByte(35, 2);
  EEPROM.writeByte(36, 0);
  EEPROM.writeByte(37, 100);
  EEPROM.writeByte(38, 2);
  EEPROM.writeByte(39, 1);
  EEPROM.writeByte(40, 0);
  EEPROM.writeByte(41, 11);
  EEPROM.writeInt(42, 0);
  EEPROM.commit();
  trigger7();
}

void buienradar() {
  WiFiClientSecure http;
  HTTPClient httpsClient;
  http.setInsecure();
  String webdata;
  char client_buffer[128];
  int bytesRead;

  if (http.connect("gpsgadget.buienradar.nl", 443)) {
    http.print("GET /data/raintext?lat=" + String(latitude, 2) + "&lon=" + String(longitude, 2) + " HTTP/1.1\r\n");
    http.print("Host: gpsgadget.buienradar.nl\r\n");
    http.print("Connection: close\r\n");
    http.print("\r\n");

    int httpCode = http.connected() ? 200 : 0;
    if (httpCode == 200) {
      char endOfHeaders[] = "\r\n\r\n";
      if (!http.find(endOfHeaders)) {
        http.stop();
        Serial.println("Buienradar: Failed to find end of headers");
        return;
      }

      while (http.connected() && (bytesRead = http.readBytes(client_buffer, sizeof(client_buffer))) > 0) {
        webdata += String(client_buffer).substring(0, bytesRead);
      }

      String lines[24];
      int numLines = splitString(webdata, '\n', lines, 24);

      for (int i = 0; i < numLines; i++) {
        String line = lines[i];

        int separatorIndex = line.indexOf('|');
        if (separatorIndex != -1) {
          br_mmpu[i] = line.substring(0, separatorIndex).toInt();
          line.substring(separatorIndex + 1).toCharArray(br_tijd[i], 6);
        } else {
          Serial.println("Buienradar: Failed to parse line " + String(i));
        }
      }
    } else {
      Serial.println("Buienradar: Failed to make HTTP request. Error code: " + String(httpCode));
    }
  } else {
    Serial.println("Buienradar: Failed to establish a connection to the server");
  }
  http.stop();
}

int splitString(const String& input, char delimiter, String* output, int maxItems) {
  int itemCount = 0;
  int lastIndex = -1;
  int length = input.length();

  for (int i = 0; i < length && itemCount < maxItems; i++) {
    if (input.charAt(i) == delimiter) {
      output[itemCount++] = input.substring(lastIndex + 1, i);
      lastIndex = i;
    }
  }

  if (lastIndex + 1 < length && itemCount < maxItems) {
    output[itemCount++] = input.substring(lastIndex + 1);
  }

  return itemCount;
}
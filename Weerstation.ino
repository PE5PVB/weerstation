// Weerstation
// (c) 2023 PE5PVB Sjef Verhoeven
//
// Getest met board manager v2.0.7
//
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <Wire.h>
#include "WiFiConnect.h"
#include <ArduinoJson.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <maidenhead.h>
#include <EEPROM.h>
#include <TextFinder.h>
#include <WiFiConnectParam.h>
#include <DFRobot_SGP40.h>
#include "AS3935-Lightning-sensor-SOLDERED.h" // https://github.com/SolderedElectronics/Soldered-AS3935-Lightning-detect-Arduino-Library
#include "DHT.h"                              // https://github.com/adafruit/DHT-sensor-library
#include <EasyNextionLibrary.h>               // https://github.com/Seithan/EasyNextionLibrary
#include <Timezone.h>                         // https://github.com/JChristensen/Timezone
#include <NTPClient.h>                        // https://github.com/arduino-libraries/NTPClient

#define SOFTWAREVERSION 10
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};
Timezone CE(CEST, CET);
WiFiConnect wc;
WiFiClient jsonclient;
WiFiClient mufclient;
WiFiClientSecure propclient;
TinyGPSPlus gps;
TextFinder finder(propclient);
EasyNex Display(Serial2);
DHT dht(32, DHT22);
SoftwareSerial ss(12, 13);
WiFiUDP ntpUDP;
NTPClient NTPtimeClient(ntpUDP, "europe.pool.ntp.org");
DFRobot_SGP40 mySgp40;
AS3935 lightning(0x03);

bool beeper;
bool beeper_alarm = false;
bool beeper_lightning = false;
bool display_alarm = false;
bool display_gps = false;
bool display_radio = false;
bool display_weather = true;
bool lightningoutdoors = false;
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
byte lv;
byte lvold;
byte ntp;
byte spike;
byte swver;
byte threshold;
byte winds;
byte windsold;
byte zicht;
byte zichtold;
byte kmu;
double latitude;
double longitude;
int d0neerslag;
int d0neerslagold;
int d0tmax;
int d0tmaxold;
int d0tmin;
int d0tminold;
int d0zon;
int d0zonold;
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
String lightningtextold[6];
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

void buzzer(byte times) {
  if (beeper == true) {
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
  EEPROM.writeByte(35, 5);
  EEPROM.writeByte(36, 0);
  EEPROM.writeByte(37, 100);
  EEPROM.writeByte(38, 5);
  EEPROM.writeByte(39, 1);
  EEPROM.writeByte(40, 0);
  EEPROM.writeByte(41, SOFTWAREVERSION);
  EEPROM.writeInt(42, 0);
  EEPROM.commit();
  trigger7();
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(intPin, INPUT);
  ss.begin(9600);
  Display.begin(115200);
  Serial.begin(115200);
  EEPROM.begin(47);
  swver = EEPROM.readByte(41);
  if (swver != SOFTWAREVERSION) defaults();
  latitude = EEPROM.readDouble(9), 6;
  longitude = EEPROM.readDouble(1), 6;
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
  Display.writeStr("version.txt", ("v " + String(swver / 10) + "." + String(swver % 10)));
  Display.writeStr("info.txt", "Verbinden met WiFi...");
  Serial.print("WiFi verbinding opbouwen.....");
  if (wc.autoConnect()) {
    WiFi.begin();
    Serial.println("WiFi verbonden");
    Display.writeStr("status.txt", "OK!");
    wifistatus = true;
  } else {
    Serial.println("Geen verbinding, opnieuw instellen");
    Display.writeStr("page 6");
    while (wifistatus == false) Display.NextionListen();
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
  Wire.begin();
  Display.writeStr("info.txt", "Lightning sensor");
  Display.writeStr("status.txt", " ");
  if (!lightning.begin()) {
    Serial.println("Geen Lightning sensor");
    Display.writeStr("status.txt", "FOUT!");
  } else {
    Serial.println("Lightning sensor OK");
    Display.writeStr("status.txt", "OK!");
    lightning.tuneAntenna();
    setLightning();
  }
  delay(1000);
  if (radio == 1) Display.writeNum("radiobutton", 1);
  Serial.println("Setup klaar");
  Display.writeStr("info.txt", "Gegevens downloaden...");
  Display.writeStr("status.txt", " ");
  getWeather();
  delay(1000);
  NTPtimeClient.begin();
  Display.writeNum("setkmu", kmu);
  Display.writeStr("page 1");
}

void loop(void) {
  digitalWrite(LED_BUILTIN, HIGH);
  Display.NextionListen();
  getGPS();
  if (menu == false) {
    if (millis() >= time_1 + 30000) {
      time_1 += 30000;
      getPropagation();
    }

    if (millis() >= time_2 + 20000) {
      time_2 += 20000;
      getMUF();
    }

    if (millis() >= time_3 + 300000) {
      time_3 += 300000;
      getWeather();
    }

    if (millis() >= time_4 + 5000) {
      time_4 += 5000;
      getIndoor();
      showData();
      showGPS();
    }

    if (millis() >= time_6 + 600000) {
      time_6 += 600000;
      beeper_lightning = false;
      if (weeralarm == false) {
        Display.writeNum("b_alarm.pic", 67);
      }
    }

    if (millis() >= time_5 + 300000) {
      if (autodim == true) Display.writeStr("dim=10");
      time_5 += 300000;
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
          if (beeper_lightning == false) {
            buzzer(0);
            beeper_lightning = true;
            Display.writeNum("b_alarm.pic", 49);
          }
          unsigned int lightningDistKm = lightning.distanceToStorm();
          Serial.println("Lightning occurs!");
          Serial.print("Distance: ");
          Serial.print(lightningDistKm);
          Serial.println(" km");

          unsigned int lightningEnergyVal = lightning.lightningEnergy();
          Serial.print("Intensity: ");
          Serial.print(lightningEnergyVal);
          Serial.println("");

          lightningcount++;
          if (lightningcount > 6) {
            lightningtext[0] = lightningtext[1];
            lightningtext[1] = lightningtext[2];
            lightningtext[2] = lightningtext[3];
            lightningtext[3] = lightningtext[4];
            lightningtext[4] = lightningtext[5];
            lightningtext[5] = (datetimestamp + " Afstand: " + String(lightningDistKm) + "km");
            lightningcount = 7;
          } else {
            lightningtext[lightningcount] = (datetimestamp + " Afstand: " + String(lightningDistKm) + "km");
          }
          lightningtextmerge = (lightningtext[0] + "\\r" + lightningtext[1] + "\\r" + lightningtext[2] + "\\r" + lightningtext[3] + "\\r" + lightningtext[4] + "\\r" + lightningtext[5] + "\\r");
        }
      }
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void setLightning() {
  lightning.maskDisturber(true);
  delay(50);
  if (indoor == true) lightning.setIndoorOutdoor(0x12); else lightning.setIndoorOutdoor(0xE);
  delay(50);
  lightning.setNoiseLevel(2);
  delay(50);
  lightning.watchdogThreshold(threshold);
  delay(50);
  lightning.spikeRejection(spike);
  delay(50);
  lightning.lightningThreshold(1);
  delay(50);
  //  lightning.resetSettings();
  Serial.println("Lightning watchdog threshold set to: " + String(lightning.readWatchdogThreshold()));
  Serial.println("Lightning spike rejection set to: " + String(lightning.readSpikeRejection()));
  Serial.println("Lightning threshold set to: " + String(lightning.readLightningThreshold()));
  Serial.println("Lightning noise level set to: " + String(lightning.readNoiseLevel()));
}

void trigger1() {   // Weather view
  if (currenttrigger != 1) {
    Display.writeNum("dim", contrast);
    time_5 = millis();
    ResetScreenData();
    display_weather = true;
    display_gps = false;
    display_radio = false;
    display_alarm = false;
    Display.writeStr("page 1");
    if (radio == 0) Display.writeNum("radiobutton", 0);
    if (radio == 1) Display.writeNum("radiobutton", 1);
    showData();
    currenttrigger = 1;
  }
}

void trigger2() {   // Radio view
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

void trigger3() {   // GPS view
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

void trigger4() {   // Config1 view
  if (currenttrigger != 4) {
    menu = true;
    Display.writeNum("dim", contrast);
    time_5 = millis();
    ResetScreenData();
    display_weather = false;
    Display.writeStr("page 9");
    delay(100);
    if (alarmswitch == false) {
      Display.writeNum("b_weeralarm.pic", 50);
    } else {
      Display.writeNum("b_weeralarm.pic", 51);
    }
    if (indoor == false) {
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

void trigger5() {   // Config1 afsluiten
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
    if (beeper == false) {
      Display.writeNum("b_beeper.pic", 50);
    } else {
      Display.writeNum("b_beeper.pic", 51);
    }
    if (radio == false) {
      Display.writeNum("b_radio.pic", 50);
    } else {
      Display.writeNum("b_radio.pic", 51);
    }
    if (kmu == false) {
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

void trigger6() {    // Update locatie
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

void trigger7() {   // WiFi configuratiescherm
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

void trigger8() {   // WiFi retry
  if (currenttrigger != 8) {
    if (wc.autoConnect()) {
      WiFi.begin();
      wifistatus = true;
    } else {
      Display.writeStr("page 6");
    }
    currenttrigger = 8;
  }
}

void trigger9() {   // Warning view
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

void trigger10() {   // Warning afsluiten
  if (currenttrigger != 10) {
    currenttrigger = 10;
    trigger1();
  }
}

void trigger11() {  // Config 3 afsluiten
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

void trigger14() {  // Config 2 afsluiten
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
    if (autoloc == false) {
      Display.writeNum("b_autoloc.pic", 50);
    } else {
      Display.writeNum("b_autoloc.pic", 51);
    }
    if (ntp == false) {
      Display.writeNum("b_ntp.pic", 50);
    } else {
      Display.writeNum("b_ntp.pic", 51);
    }
    if (autodim == false) {
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
  windrold = " ";
  windsold = 0;
  luchtdold = 0;
  zichtold = 0;
  verwold = " ";
  imageold = 0;
  weeralarmold = 0;
  d1imageold = 0;
  d0zonold = 0;
  d2imageold = 0;
  d0neerslagold = 0;
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
}

String ParseText(String p_buff, String from, String to) {
  String output;
  output = p_buff.substring(p_buff.indexOf(from) + from.length(), p_buff.indexOf(to));
  if (output.indexOf("<solardata>") > 0) output = " ";
  return output;
}

void setWifi() {
  char api_key [11];
  key.toCharArray(api_key, 11);
  WiFiConnectParam api_key_text("Enter weerlive.nl API key:");
  WiFiConnectParam api_key_input("api_key", "API Key", api_key, 11);
  wc.addParameter(&api_key_text);
  wc.addParameter(&api_key_input);

  wc.startConfigurationPortal(AP_WAIT);
  Serial.println("Ontvangen waarde: ");
  Serial.println(api_key_input.getValue());
  EEPROM.writeString(17, String(api_key_input.getValue()));
  EEPROM.commit();
  key = String(api_key_input.getValue());
  if (wc.autoConnect()) {
    WiFi.begin();
  } else {
    Serial.println("Geen verbinding, opnieuw instellen");
    setWifi();
  }
}

void getPropagation() {
  String prop_buf;
  char prop_char;
  uint16_t pos;
  String t;

  propclient.setInsecure();
  propclient.setTimeout(2000);
  if (!propclient.connect("hamqsl.com", 443)) {
    return;
  }
  propclient.println("GET https://hamqsl.com/solarxml.php HTTP/1.0");
  propclient.println("Host: hamqsl.com");
  propclient.println("Connection: close");
  propclient.println();

  if (propclient.println() == 0) {
    propclient.stop();
    return;
  }

  char status[32] = {0};
  propclient.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status + 9, "200 OK") != 0) {
    propclient.stop();
    return;
  }

  char endOfHeaders[] = "\r\n\r\n";
  if (!propclient.find(endOfHeaders)) {
    propclient.stop();
    return;
  }


  for (uint16_t i = 0; i < 2000; i++)
  {
    prop_char = propclient.read();
    if (prop_char != 0xFF) prop_buf += prop_char;
  }

  solarflux = ParseText(prop_buf, "<solarflux>", "</solarflux>");
  aindex = ParseText(prop_buf, "<aindex>", "</aindex>");
  kindex = ParseText(prop_buf, "<kindex>", "</kindex>");
  sunspots = ParseText(prop_buf, "<sunspots>", "</sunspots>");
  protonflux = ParseText(prop_buf, "<protonflux>", "</protonflux>");
  electonflux = ParseText(prop_buf, "<electonflux>", "</electonflux>");
  aurora = ParseText(prop_buf, "<aurora>", "</aurora>");
  normalisation = ParseText(prop_buf, "<normalization>", "</normalization>");
  latdegree = ParseText(prop_buf, "<latdegree>", "</latdegree>");
  solarwind = ParseText(prop_buf, "<solarwind>", "</solarwind>");
  magneticfield = ParseText(prop_buf, "<magneticfield>", "</magneticfield>");
  String hfconditions = ParseText(prop_buf, "<calculatedconditions>", "</calculatedconditions>");
  d8040m = hfconditions.substring(37, 41);
  d3020m = hfconditions.substring(85, 89);
  d1715m = hfconditions.substring(133, 137);
  d1210m = hfconditions.substring(181, 185);
  n8040m = hfconditions.substring(231, 235);
  n3020m = hfconditions.substring(281, 285);
  n1715m = hfconditions.substring(331, 335);
  n1210m = hfconditions.substring(381, 385);
  String vhfconditions = ParseText(prop_buf, "<calculatedvhfconditions>", "</calculatedvhfconditions>");
  es2m = vhfconditions.substring(vhfconditions.indexOf("location=\"europe\"") + 18 , vhfconditions.indexOf("location=\"europe\"") + 40);
  es2m = es2m.substring(0, es2m.lastIndexOf("<"));
  es6m = vhfconditions.substring(vhfconditions.indexOf("location=\"europe_6m\"") + 21 , vhfconditions.indexOf("location=\"europe_6m\"") + 40);
  es6m = es6m.substring(0, es6m.lastIndexOf("<"));
  es4m = vhfconditions.substring(vhfconditions.indexOf("location=\"europe_4m\"") + 21 , vhfconditions.indexOf("location=\"europe_4m\"") + 40);
  es4m = es4m.substring(0, es4m.lastIndexOf("<"));
  geomagfield = ParseText(prop_buf, "<geomagfield>", "</geomagfield>");
  signalnoise = ParseText(prop_buf, "<signalnoise>", "</signalnoise>");
}

void getWeather() {
  jsonclient.setTimeout(2000);
  if (!jsonclient.connect("weerlive.nl", 80)) {
    return;
  }
  jsonclient.println("GET /api/json-data-10min.php?key=" + String(key) + "&locatie=" + String(latitude, 6) + "," + String(longitude, 6) + " HTTP/1.0");
  Serial.println("weerlive.nl/api/json-data-10min.php?key=" + String(key) + "&locatie=" + String(latitude, 6) + "," + String(longitude, 6) + " HTTP/1.0");
  jsonclient.println("Host: weerlive.nl");
  jsonclient.println("Connection: close");
  if (jsonclient.println() == 0) {
    jsonclient.stop();
    return;
  }
  char status[32] = {0};
  jsonclient.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status + 9, "200 OK") != 0) {
    jsonclient.stop();
    return;
  }
  char endOfHeaders[] = "\r\n\r\n";
  if (!jsonclient.find(endOfHeaders)) {
    jsonclient.stop();
    return;
  }
  DynamicJsonDocument weer(4096);
  DeserializationError error = deserializeJson(weer, jsonclient);
  if (error) {
    jsonclient.stop();
    return;
  }
  plaats = weer["liveweer"][0]["plaats"].as<String>();
  float t = weer["liveweer"][0]["temp"].as<float>();
  temp = weer["liveweer"][0]["temp"].as<float>() * 10;
  samenv = weer["liveweer"][0]["samenv"].as<String>();
  lv = weer["liveweer"][0]["lv"].as<byte>();
  windr = weer["liveweer"][0]["windr"].as<String>();
  winds = weer["liveweer"][0]["winds"].as<byte>();
  windkmh = weer["liveweer"][0]["windkmh"].as<float>() * 10;
  windms = weer["liveweer"][0]["windms"].as<int>();
  luchtd = weer["liveweer"][0]["luchtd"].as<float>() * 10;
  zicht = weer["liveweer"][0]["zicht"].as<byte>();
  verw = weer["liveweer"][0]["verw"].as<String>();
  weeralarm = weer["liveweer"][0]["alarm"].as<byte>();
  alarmtxt = weer["liveweer"][0]["alarmtxt"].as<String>();
  zonop = weer["liveweer"][0]["sup"].as<String>();
  zononder = weer["liveweer"][0]["sunder"].as<String>();
  if (weer["liveweer"][0]["image"].as<String>() == "zonnig") {
    image = 16;
  } else if (weer["liveweer"][0]["image"].as<String>() == "bliksem") {
    image = 17;
  } else if (weer["liveweer"][0]["image"].as<String>() == "regen") {
    image = 18;
  } else if (weer["liveweer"][0]["image"].as<String>() == "buien") {
    image = 19;
  } else if (weer["liveweer"][0]["image"].as<String>() == "hagel") {
    image = 20;
  } else if (weer["liveweer"][0]["image"].as<String>() == "mist") {
    image = 21;
  } else if (weer["liveweer"][0]["image"].as<String>() == "sneeuw") {
    image = 22;
  } else if (weer["liveweer"][0]["image"].as<String>() == "bewolkt") {
    image = 23;
  } else if (weer["liveweer"][0]["image"].as<String>() == "lichtbewolkt") {
    image = 24;
  } else if (weer["liveweer"][0]["image"].as<String>() == "halfbewolkt") {
    image = 25;
  } else if (weer["liveweer"][0]["image"].as<String>() == "halfbewolkt_regen") {
    image = 26;
  } else if (weer["liveweer"][0]["image"].as<String>() == "zwaarbewolkt") {
    image = 27;
  } else if (weer["liveweer"][0]["image"].as<String>() == "nachtmist") {
    image = 28;
  } else if (weer["liveweer"][0]["image"].as<String>() == "helderenacht") {
    image = 29;
  } else if (weer["liveweer"][0]["image"].as<String>() == "wolkennacht") {
    image = 30;
  } else if (weer["liveweer"][0]["image"].as<String>() == "nachtbewolkt") {
    image = 30;
  }
  if (weer["liveweer"][0]["d1weer"].as<String>() == "zonnig") d1image = 31;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "bliksem") d1image = 32;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "regen") d1image = 33;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "buien") d1image = 34;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "hagel") d1image = 35;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "mist") d1image = 36;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "sneeuw") d1image = 37;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "bewolkt") d1image = 38;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "lichtbewolkt") d1image = 39;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "halfbewolkt") d1image = 40;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "halfbewolkt_regen") d1image = 41;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "zwaarbewolkt") d1image = 42;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "nachtmist") d1image = 43;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "helderenacht") d1image = 44;
  if (weer["liveweer"][0]["d1weer"].as<String>() == "nachtbewolkt") d1image = 45;

  if (weer["liveweer"][0]["d2weer"].as<String>() == "zonnig") d2image = 31;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "bliksem") d2image = 32;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "regen") d2image = 33;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "buien") d2image = 34;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "hagel") d2image = 35;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "mist") d2image = 36;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "sneeuw") d2image = 37;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "bewolkt") d2image = 38;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "lichtbewolkt") d2image = 39;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "halfbewolkt") d2image = 40;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "halfbewolkt_regen") d2image = 41;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "zwaarbewolkt") d2image = 42;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "nachtmist") d2image = 43;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "helderenacht") d2image = 44;
  if (weer["liveweer"][0]["d2weer"].as<String>() == "nachtbewolkt") d2image = 45;

  d0zon = weer["liveweer"][0]["d0zon"].as<int>();
  d0neerslag = weer["liveweer"][0]["d0neerslag"].as<int>();
  d1zon = weer["liveweer"][0]["d1zon"].as<int>();
  d1neerslag = weer["liveweer"][0]["d1neerslag"].as<int>();
  d2zon = weer["liveweer"][0]["d2zon"].as<int>();
  d2neerslag = weer["liveweer"][0]["d2neerslag"].as<int>();
  d0tmin = weer["liveweer"][0]["d0tmin"].as<int>();
  d0tmax = weer["liveweer"][0]["d0tmax"].as<int>();
  d1tmin = weer["liveweer"][0]["d1tmin"].as<int>();
  d1tmax = weer["liveweer"][0]["d1tmax"].as<int>();
  d1windk = weer["liveweer"][0]["d1windk"].as<byte>();
  d1windr = weer["liveweer"][0]["d1windr"].as<String>();
  d2tmin = weer["liveweer"][0]["d2tmin"].as<int>();
  d2tmax = weer["liveweer"][0]["d2tmax"].as<int>();
  d2windk = weer["liveweer"][0]["d2windk"].as<byte>();
  d2windr = weer["liveweer"][0]["d2windr"].as<String>();

  jsonclient.stop();
  if (windr == "Noord") {
    windr = "N";
  } else if (windr == "West") {
    windr = "W";
  } else if (windr == "Oost") {
    windr = "O";
  } else if (windr == "Zuid") {
    windr = "Z";
  }
}

void getMUF() {
  String muf_buf;
  char muf_char;
  uint16_t pos;
  String t;

  mufclient.setTimeout(2000);
  if (!mufclient.connect("ionosphere.meteo.be", 80)) {
    return;
  }

  mufclient.println("GET /ionosphere/MUF/latest-MUF-DB049.php HTTP/1.1");
  mufclient.println("Host: ionosphere.meteo.be");
  mufclient.println("Connection: close");
  if (mufclient.println() == 0) {
    mufclient.stop();
    return;
  }

  char status[32] = {0};
  mufclient.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status + 9, "200 OK") != 0) {
    mufclient.stop();
    return;
  }
  char endOfHeaders[] = "\r\n\r\n";
  if (!mufclient.find(endOfHeaders)) {
    mufclient.stop();
    return;
  }

  for (uint16_t i = 0; i < 520; i++)
  {
    muf_char = mufclient.read();
    if (muf_char != 0xFF) muf_buf += muf_char;
  }

  pos = muf_buf.lastIndexOf("3000");
  pos = muf_buf.indexOf("\n", pos);
  muf_buf = muf_buf.substring(0, pos + 1);

  for (uint8_t z = 0; z < 9; z++)
  {
    pos = muf_buf.lastIndexOf("\n");
    t = muf_buf.substring(pos - 5 , pos);
    muf_buf = muf_buf.substring(0, pos - 5);
    t.trim();
    muf_string[z] = t;
  }
}

void showData() {
  if (lightningtextmerge != lightningtextmergeold) {
    if (display_alarm == true) Display.writeStr("lightning.txt", lightningtextmerge);
    lightningtextmergeold = lightningtextmerge;
  }

  if (solarflux != solarfluxold) {
    Serial.print("Solarflux               : ");
    Serial.println(solarflux);
    if (display_radio == true) Display.writeStr("solarflux.txt", solarflux);
    solarfluxold = solarflux;
  }
  if (aindex != aindexold) {
    Serial.print("A-index                 : ");
    Serial.println(aindex);
    if (display_radio == true) Display.writeStr("aindex.txt", aindex);
    aindexold = aindex;
  }
  if (kindex != kindexold) {
    Serial.print("K-index                 : ");
    Serial.println(kindex);
    if (display_radio == true) Display.writeStr("kindex.txt", kindex);
    kindexold = kindex;
  }
  if (sunspots != sunspotsold) {
    Serial.print("Aantal zonnevlekken     : ");
    Serial.println(sunspots);
    if (display_radio == true) Display.writeStr("sunspots.txt", sunspots);
    sunspotsold = sunspots;
  }
  if (protonflux != protonfluxold) {
    Serial.print("Protonflux              : ");
    Serial.println(protonflux);
    if (display_radio == true) Display.writeStr("protonflux.txt", protonflux);
    protonfluxold = protonflux;
  }
  if (electonflux != electonfluxold) {
    Serial.print("Electonflux             : ");
    Serial.println(electonflux);
    if (display_radio == true) Display.writeStr("electonflux.txt", electonflux);
    electonfluxold = electonflux;
  }
  if (aurora != auroraold) {
    Serial.print("Aurora                  : ");
    Serial.println(aurora);
    if (display_radio == true) Display.writeStr("aurora.txt", aurora);
    auroraold = aurora;
  }
  if (normalisation != normalisationold) {
    Serial.print("Aurora normalisatie     : ");
    Serial.println(normalisation);
    if (display_radio == true) Display.writeStr("normalisation.txt", normalisation);
    normalisationold = normalisation;
  }
  if (latdegree != latdegreeold) {
    Serial.print("Aurora breedtegraad     : ");
    Serial.println(latdegree);
    if (display_radio == true) Display.writeStr("latdegree.txt", latdegree);
    latdegreeold = latdegree;
  }
  if (solarwind != solarwindold) {
    Serial.print("Zonnewind               : ");
    Serial.println(solarwind);
    if (display_radio == true) Display.writeStr("solarwind.txt", solarwind);
    solarwindold = solarwind;
  }
  if (magneticfield != magneticfieldold) {
    Serial.print("Magnetisch veld         : ");
    Serial.println(magneticfield);
    if (display_radio == true) Display.writeStr("magneticfield.txt", magneticfield);
    magneticfieldold = magneticfield;
  }
  if (geomagfield != geomagfieldold) {
    Serial.print("Geomagnetisch veld      : ");
    Serial.println(geomagfield);
    if (display_radio == true) Display.writeStr("geomagfield.txt", geomagfield);
    geomagfieldold = geomagfield;
  }
  if (d8040m != d8040mold) {
    Serial.print("80-40m overdag          : ");
    Serial.println(d8040m);
    if (display_radio == true) Display.writeStr("d8040.txt", d8040m);
    d8040mold = d8040m;
  }
  if (n8040m != n8040mold) {
    Serial.print("80-40m 's nachts        : ");
    Serial.println(n8040m);
    if (display_radio == true) Display.writeStr("n8040.txt", n8040m);
    n8040mold = n8040m;
  }
  if (d3020m != d3020mold) {
    Serial.print("30-20m overdag          : ");
    Serial.println(d3020m);
    if (display_radio == true) Display.writeStr("d3020.txt", d3020m);
    d3020mold = d3020m;
  }
  if (n3020m != n3020mold) {
    Serial.print("30-20m 's nachts        : ");
    Serial.println(n3020m);
    if (display_radio == true) Display.writeStr("n3020.txt", n3020m);
    n3020mold = n3020m;
  }
  if (d1715m != d1715mold) {
    Serial.print("17-15m overdag          : ");
    Serial.println(d1715m);
    if (display_radio == true) Display.writeStr("d1715.txt", d1715m);
    d1715mold = d1715m;
  }
  if (n1715m != n1715mold) {
    Serial.print("17-15m 's nachts        : ");
    Serial.println(n1715m);
    if (display_radio == true) Display.writeStr("n1715.txt", n1715m);
    n1715mold = n1715m;
  }
  if (d1210m != d1210mold) {
    Serial.print("12-10m overdag          : ");
    Serial.println(d1210m);
    if (display_radio == true) Display.writeStr("d1210.txt", d1210m);
    d1210mold = d1210m;
  }
  if (n1210m != n1210mold) {
    Serial.print("12-10m 's nachts        : ");
    Serial.println(n1210m);
    if (display_radio == true) Display.writeStr("n1210.txt", n1210m);
    n1210mold = n1210m;
  }
  if (signalnoise != signalnoiseold) {
    Serial.print("HF ruisvloer            : ");
    Serial.println(signalnoise);
    if (display_radio == true) Display.writeStr("signalnoise.txt", signalnoise);
    signalnoiseold = signalnoise;
  }
  if (es2m != es2mold) {
    Serial.print("Sporadische E 2m        : ");
    Serial.println(es2m);
    if (display_radio == true) Display.writeStr("vhf2.txt", es2m);
    es2mold = es2m;
  }
  if (es6m != es6mold) {
    Serial.print("Sporadische E 6m        : ");
    Serial.println(es6m);
    if (display_radio == true) Display.writeStr("vhf6.txt", es6m);
    es6mold = es6m;
  }
  if (es4m != es4mold) {
    Serial.print("Sporadische E 4m        : ");
    Serial.println(es4m);
    if (display_radio == true) Display.writeStr("vhf4.txt", es4m);
    es4mold = es4m;
  }
  if (muf_string[8] != muf_stringold[8]) {
    Serial.print("MUF < 100km             : ");
    Serial.println(muf_string[8]);
    buildMUFticker();
    muf_stringold[8] = muf_string[8];
  }
  if (muf_string[7] != muf_stringold[7]) {
    Serial.print("MUF 100-200km           : ");
    Serial.println(muf_string[7]);
    buildMUFticker();
    muf_stringold[7] = muf_string[7];
  }
  if (muf_string[6] != muf_stringold[6]) {
    Serial.print("MUF 200-400km           : ");
    Serial.println(muf_string[6]);
    buildMUFticker();
    muf_stringold[6] = muf_string[6];
  }
  if (muf_string[5] != muf_stringold[5]) {
    Serial.print("MUF 400-600km           : ");
    Serial.println(muf_string[5]);
    buildMUFticker();
    muf_stringold[5] = muf_string[5];
  }
  if (muf_string[4] != muf_stringold[4]) {
    Serial.print("MUF 600-800km           : ");
    Serial.println(muf_string[4]);
    buildMUFticker();
    muf_stringold[4] = muf_string[4];
  }
  if (muf_string[3] != muf_stringold[3]) {
    Serial.print("MUF 800-1000km          : ");
    Serial.println(muf_string[3]);
    buildMUFticker();
    muf_stringold[3] = muf_string[3];
  }
  if (muf_string[2] != muf_stringold[2]) {
    Serial.print("MUF 1000-1500km         : ");
    Serial.println(muf_string[2]);
    buildMUFticker();
    muf_stringold[2] = muf_string[2];
  }
  if (muf_string[1] != muf_stringold[1]) {
    Serial.print("MUF 1500-3000km         : ");
    Serial.println(muf_string[1]);
    buildMUFticker();
    muf_stringold[1] = muf_string[1];
  }
  if (muf_string[0] != muf_stringold[0]) {
    Serial.print("MUF >3000km             : ");
    Serial.println(muf_string[0]);
    buildMUFticker();
    muf_stringold[0] = muf_string[0];
  }

  String plaatsqth = plaats;
  if (radio == true && autoloc == true) {
    if (gps.location.isValid() == true) plaatsqth = plaats + " (" + String(get_mh(gps.location.lat(), gps.location.lng(), 10)) + ")";
  } else {
    if (gps.location.isValid() == true) plaatsqth = plaats;
  }
  if (plaats != plaatsold || String(get_mh(gps.location.lat(), gps.location.lng(), 10)) != qthold) {
    Serial.print("Locatie                 : ");
    Serial.println(plaats);
    Display.writeStr("plaats.txt", plaatsqth);
    plaatsold = plaats;
  }
  if (String(get_mh(gps.location.lat(), gps.location.lng(), 10)) != qthold) {
    Serial.print("QTH locator             : ");
    Serial.println(get_mh(gps.location.lat(), gps.location.lng(), 10));
    qthold = String(get_mh(gps.location.lat(), gps.location.lng(), 10));
  }

  if (temp != tempold) {
    Serial.print("Temperatuur (/10)       : ");
    Serial.println(temp);
    if (display_weather == true) Display.writeNum("actualtemp.val", temp);
    tempold = temp;
  }
  if (samenv != samenvold) {
    Serial.print("Actueel weerbeeld       : ");
    Serial.println(samenv);
    if (display_weather == true) Display.writeStr("actualtext.txt", samenv);
    samenvold = samenv;
  }
  if (lv != lvold) {
    Serial.print("Luchtvochtigheid        : ");
    Serial.println(lv);
    if (display_weather == true) Display.writeNum("luchtv.val", lv);
    lvold = lv;
  }
  if (zonop != zonopold) {
    Serial.print("Zon op                  : ");
    Serial.println(zonop);
    if (display_weather == true) Display.writeStr("zonop.txt", zonop);
    zonopold = zonop;
  }
  if (zononder != zononderold) {
    Serial.print("Zon onder               : ");
    Serial.println(zononder);
    if (display_weather == true) Display.writeStr("zononder.txt", zononder);
    zononderold = zononder;
  }
  if (windr != windrold) {
    Serial.print("Windrichting            : ");
    Serial.println(windr);
    if (display_weather == true) {
      if (windr == "N") Display.writeNum("actualwindr.pic", 0);
      if (windr == "NNO") Display.writeNum("actualwindr.pic", 1);
      if (windr == "NO") Display.writeNum("actualwindr.pic", 2);
      if (windr == "ONO") Display.writeNum("actualwindr.pic", 3);
      if (windr == "O") Display.writeNum("actualwindr.pic", 4);
      if (windr == "OZO") Display.writeNum("actualwindr.pic", 5);
      if (windr == "ZO") Display.writeNum("actualwindr.pic", 6);
      if (windr == "OZO") Display.writeNum("actualwindr.pic", 7);
      if (windr == "Z") Display.writeNum("actualwindr.pic", 8);
      if (windr == "ZZW") Display.writeNum("actualwindr.pic", 9);
      if (windr == "ZW") Display.writeNum("actualwindr.pic", 10);
      if (windr == "WZW") Display.writeNum("actualwindr.pic", 11);
      if (windr == "W") Display.writeNum("actualwindr.pic", 12);
      if (windr == "WNW") Display.writeNum("actualwindr.pic", 13);
      if (windr == "NW") Display.writeNum("actualwindr.pic", 14);
      if (windr == "NWN") Display.writeNum("actualwindr.pic", 15);
    }
    windrold = windr;
  }
  if (winds != windsold) {
    Serial.print("Windkracht bft          : ");
    Serial.println(winds);
    if (display_weather == true) Display.writeNum("actualbft.val", winds);
    windsold = winds;
  }
  if (windkmh != windkmhold) {
    Serial.print("Windsnelheid km/u (/10) : ");
    Serial.println(windkmh);
    if (display_weather == true && kmu == true) Display.writeNum("actualwindkmh.val", windkmh);
    windkmhold = windkmh;
  }
  if (windms != windmsold) {
    Serial.print("Windsnelheid m/s        : ");
    Serial.println(windms);
    if (display_weather == true && kmu == false) Display.writeNum("actualwindkmh.val", windms);
    windmsold = windms;
  }
  if (luchtd != luchtdold) {
    Serial.print("Luchtdruk (/10)         : ");
    Serial.println(luchtd);
    if (display_weather == true) Display.writeNum("luchtd.val", luchtd);
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
    if (display_weather == true) Display.writeStr("d0text.txt", verw);
    verwold = verw;
  }
  if (image != imageold) {
    Serial.print("Weer icoon              : ");
    Serial.println(image);
    if (display_weather == true) Display.writeNum("weerimage.pic", image);
    imageold = image;
  }
  if (weeralarm != weeralarmold) {
    Serial.print("Waarschuwing            : ");
    Serial.println(weeralarm);
    if (display_weather == true) {
      if (weeralarm == true) {
        Display.writeNum("b_alarm.pic", 49);
        if (beeper_alarm == false) {
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
    if (weeralarm == 0) alarmtxt = "Er zijn geen waarschuwingen van kracht.";
    Serial.println(alarmtxt);
    alarmtxtold = alarmtxt;
  }
  if (d0zon != d0zonold) {
    Serial.print("Kans op zon             : ");
    Serial.println(d0zon);
    if (display_weather == true) Display.writeNum("d0zon.val", d0zon);
    d0zonold = d0zon;
  }
  if (d0neerslag != d0neerslagold) {
    Serial.print("Kans op regen           : ");
    Serial.println(d0neerslag);
    if (display_weather == true) Display.writeNum("d0neerslag.val", d0neerslag);
    d0neerslagold = d0neerslag;
  }
  if (d1zon != d1zonold) {
    Serial.print("Kans op zon morgen      : ");
    Serial.println(d1zon);
    if (display_weather == true) Display.writeNum("d1zon.val", d1zon);
    d1zonold = d1zon;
  }
  if (d1neerslag != d1neerslagold) {
    Serial.print("Kans op regen morgen    : ");
    Serial.println(d1neerslag);
    if (display_weather == true) Display.writeNum("d1neerslag.val", d1neerslag);
    d1neerslagold = d1neerslag;
  }
  if (d2zon != d2zonold) {
    Serial.print("Kans op zon overmorgen  : ");
    Serial.println(d2zon);
    if (display_weather == true) Display.writeNum("d2zon.val", d2zon);
    d2zonold = d2zon;
  }
  if (d2neerslag != d2neerslagold) {
    Serial.print("Kans op regen overmorgen: ");
    Serial.println(d2neerslag);
    if (display_weather == true) Display.writeNum("d2neerslag.val", d2neerslag);
    d2neerslagold = d2neerslag;
  }
  if (d1image != d1imageold) {
    Serial.print("Weer icoon morgen       : ");
    Serial.println(d1image);
    if (display_weather == true) Display.writeNum("d1image.pic", d1image);
    d1imageold = d1image;
  }
  if (d2image != d2imageold) {
    Serial.print("Weer icoon overmorgen   : ");
    Serial.println(d2image);
    if (display_weather == true) Display.writeNum("d2image.pic", d2image);
    d2imageold = d2image;
  }
  if (d0tmin != d0tminold) {
    Serial.print("Minimum temp. vandaag   : ");
    Serial.println(d0tmin);
    if (display_weather == true) Display.writeNum("d0min.val", d0tmin);
    d0tminold = d0tmin;
  }
  if (d0tmax != d0tmaxold) {
    Serial.print("Maximum temp. Vandaag   : ");
    Serial.println(d0tmax);
    if (display_weather == true) Display.writeNum("d0max.val", d0tmax);
    d0tmaxold = d0tmax;
  }
  if (d1tmin != d1tminold || d1tmax != d1tmaxold) {
    Serial.print("Min/max temp. morgen    : ");
    if (display_weather == true) Display.writeStr("d1temp.txt", (String(d1tmin) + "/" + String(d1tmax) + "\xB0"));
    Serial.println(String(d1tmin) + "/" + String(d1tmax) + "°");
    d1tminold = d1tmin;
    d1tmaxold = d1tmax;
  }
  if (d2tmin != d2tminold || d2tmax != d2tmaxold) {
    Serial.print("Min/max temp. morgen    : ");
    if (display_weather == true) Display.writeStr("d2temp.txt", (String(d2tmin) + "/" + String(d2tmax)) + "\xB0");
    Serial.println(String(d2tmin) + "/" + String(d2tmax) + "°");
    d2tminold = d2tmin;
    d2tmaxold = d2tmax;
  }
  if (d1windk != d1windkold) {
    Serial.print("Windkracht morgen bft.  : ");
    Serial.println(d1windk);
    if (display_weather == true) Display.writeNum("d1windk.val", d1windk);
    d1windkold = d1windk;
  }
  if (d2windk != d2windkold) {
    Serial.print("Windkr. overmorgen bft. : ");
    Serial.println(d2windk);
    if (display_weather == true) Display.writeNum("d2windk.val", d2windk);
    d2windkold = d2windk;
  }
  if (d1windr != d1windrold) {
    Serial.print("Windrichting morgen     : ");
    if (display_weather == true) {
      if (d1windr == "N") Display.writeNum("d1windr.pic", 0);
      if (d1windr == "NNO") Display.writeNum("d1windr.pic", 1);
      if (d1windr == "NO") Display.writeNum("d1windr.pic", 2);
      if (d1windr == "ONO") Display.writeNum("d1windr.pic", 3);
      if (d1windr == "O") Display.writeNum("d1windr.pic", 4);
      if (d1windr == "OZO") Display.writeNum("d1windr.pic", 5);
      if (d1windr == "ZO") Display.writeNum("d1windr.pic", 6);
      if (d1windr == "OZO") Display.writeNum("d1windr.pic", 7);
      if (d1windr == "Z") Display.writeNum("d1windr.pic", 8);
      if (d1windr == "ZZW") Display.writeNum("d1windr.pic", 9);
      if (d1windr == "ZW") Display.writeNum("d1windr.pic", 10);
      if (d1windr == "WZW") Display.writeNum("d1windr.pic", 11);
      if (d1windr == "W") Display.writeNum("d1windr.pic", 12);
      if (d1windr == "WNW") Display.writeNum("d1windr.pic", 13);
      if (d1windr == "NW") Display.writeNum("d1windr.pic", 14);
      if (d1windr == "NWN") Display.writeNum("d1windr.pic", 15);
    }
    Serial.println(d1windr);
    d1windrold = d1windr;
  }
  if (d2windr != d2windrold) {
    Serial.print("Windrichting overmorgen : ");
    if (display_weather == true) {
      if (d2windr == "N") Display.writeNum("d2windr.pic", 0);
      if (d2windr == "NNO") Display.writeNum("d2windr.pic", 1);
      if (d2windr == "NO") Display.writeNum("d2windr.pic", 2);
      if (d2windr == "ONO") Display.writeNum("d2windr.pic", 3);
      if (d2windr == "O") Display.writeNum("d2windr.pic", 4);
      if (d2windr == "OZO") Display.writeNum("d2windr.pic", 5);
      if (d2windr == "ZO") Display.writeNum("d2windr.pic", 6);
      if (d2windr == "OZO") Display.writeNum("d2windr.pic", 7);
      if (d2windr == "Z") Display.writeNum("d2windr.pic", 8);
      if (d2windr == "ZZW") Display.writeNum("d2windr.pic", 9);
      if (d2windr == "ZW") Display.writeNum("d2windr.pic", 10);
      if (d2windr == "WZW") Display.writeNum("d2windr.pic", 11);
      if (d2windr == "W") Display.writeNum("d2windr.pic", 12);
      if (d2windr == "WNW") Display.writeNum("d2windr.pic", 13);
      if (d2windr == "NW") Display.writeNum("d2windr.pic", 14);
      if (d2windr == "NWN") Display.writeNum("d2windr.pic", 15);
    }
    Serial.println(d2windr);
    d2windrold = d2windr;
  }

  if (indoortemp != indoortempold) {
    Serial.print("Temperatuur (/10)       : ");
    Serial.println(indoortemp);
    if (display_weather == true && indoortemp != -9990) Display.writeNum("indoortemp.val", indoortemp);
    indoortempold = indoortemp;
  }
  if (indoorhum != indoorhumold) {
    Serial.print("Luchtvochtigheid (/10)  : ");
    Serial.println(indoorhum);
    if (display_weather == true && indoorhum != -9990) Display.writeNum("indoorhum.val", indoorhum);
    indoorhumold = indoorhum;
  }
  if (indoorvoc != indoorvocold) {
    Serial.print("VOC                     : ");
    Serial.println(indoorvoc);
    if (display_weather == true) Display.writeNum("voc.val", indoorvoc);
    if (display_weather == true) Display.writeNum("vocgauge.val", indoorvoc / 2.77);
    if (display_weather == true) Display.writeStr("voctxt.txt", "VOC");
    indoorvocold = indoorvoc;
  }
}

void buildMUFticker() {
  if (display_radio == true) {
    Display.writeStr("muf.txt", String("MUF QRB: <100km: " + String(muf_string[8]) + "MHz  100-200km: " + String(muf_string[7]) + "MHz  200-400km: " + String(muf_string[6]) + "MHz  400-600km: " + String(muf_string[5]) + "MHz  600-800km: " + String(muf_string[4]) + "MHz  800-1000km: " + String(muf_string[3]) + "MHz  1000-1500km: " + String(muf_string[2]) + "MHz  1500-3000km: " + String(muf_string[1]) + "MHz  >3000km: " + String(muf_string[0]) + "MHz     "));
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
  if (String(get_mh(gps.location.lat(), gps.location.lng(), 10)) != qthold) {
    Serial.print("QTH locator             : ");
    Serial.println(get_mh(gps.location.lat(), gps.location.lng(), 10));
    qthold = String(get_mh(gps.location.lat(), gps.location.lng(), 10));
  }

  if (display_gps == true) {
    if (gps.location.isValid() == true) {
      Display.writeStr("longitude.txt", String(gps.location.lng(), 10));
      Display.writeStr("latitude.txt", String(gps.location.lat(), 10));
      Display.writeStr("height.txt", String(gps.altitude.meters()) + "m");
      Display.writeStr("hdop.txt", String(gps.hdop.hdop()));
      Display.writeStr("sats.txt", String(gps.satellites.value()));
    } else {
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

    if (gps.location.isValid() == true)
    {
      if (autoloc == true) {
        longitude = gps.location.lng(), 6;
        latitude = gps.location.lat(), 6;
        EEPROM.writeDouble(1, longitude);
        EEPROM.writeDouble(9, latitude);
        EEPROM.commit();
      }
    }
    if (ntp == false) {
      if (gps.time.isValid() && gps.date.isValid()) {
        setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
      }
    } else {
      NTPtimeClient.update();
      time_t epochTime = NTPtimeClient.getEpochTime();
      struct tm *ptm = gmtime ((time_t *)&epochTime);
      int monthDay = ptm->tm_mday;
      int currentMonth = ptm->tm_mon + 1;
      int currentYear = ptm->tm_year + 1900;
      setTime(NTPtimeClient.getHours(), NTPtimeClient.getMinutes(), NTPtimeClient.getSeconds(), monthDay, currentMonth, currentYear);
    }
  }
  FormatTime(CE);
}

void FormatTime(Timezone tz) {
  TimeChangeRule *tcr;
  time_t t = tz.toLocal(now(), &tcr);

  String dayname;
  if (weekday(t) == 1) dayname = "Zondag";
  if (weekday(t) == 2) dayname = "Maandag";
  if (weekday(t) == 3) dayname = "Dinsdag";
  if (weekday(t) == 4) dayname = "Woensdag";
  if (weekday(t) == 5) dayname = "Donderdag";
  if (weekday(t) == 6) dayname = "Vrijdag";
  if (weekday(t) == 7) dayname = "Zaterdag";
  String dayname1;
  if (weekday(t) == 1) dayname1 = "Ma";
  if (weekday(t) == 2) dayname1 = "Di";
  if (weekday(t) == 3) dayname1 = "Wo";
  if (weekday(t) == 4) dayname1 = "Do";
  if (weekday(t) == 5) dayname1 = "Vr";
  if (weekday(t) == 6) dayname1 = "Za";
  if (weekday(t) == 7) dayname1 = "Zo";
  String dayname2;
  if (weekday(t) == 1) dayname2 = "Di";
  if (weekday(t) == 2) dayname2 = "Wo";
  if (weekday(t) == 3) dayname2 = "Do";
  if (weekday(t) == 4) dayname2 = "Vr";
  if (weekday(t) == 5) dayname2 = "Za";
  if (weekday(t) == 6) dayname2 = "Zo";
  if (weekday(t) == 7) dayname2 = "Ma";
  String monthname;
  if (month(t) == 1) monthname = "jan";
  if (month(t) == 2) monthname = "feb";
  if (month(t) == 3) monthname = "mrt";
  if (month(t) == 4) monthname = "apr";
  if (month(t) == 5) monthname = "mei";
  if (month(t) == 6) monthname = "jun";
  if (month(t) == 7) monthname = "jul";
  if (month(t) == 8) monthname = "aug";
  if (month(t) == 9) monthname = "sep";
  if (month(t) == 10) monthname = "okt";
  if (month(t) == 11) monthname = "nov";
  if (month(t) == 12) monthname = "dec";

  datetime = String(dayname + " " + String(day(t)) + " " + monthname + " " + String(year(t)) + " " + String(hour(t)) + ":" + (minute(t) < 10 ? "0" : "") + String(minute(t)) + ":" + (second(t) < 10 ? "0" : "") + String(second(t)));
  datetimestamp = (String(day(t)) + "/" + String(month(t)) + " " + String(hour(t)) + ":" + (minute(t) < 10 ? "0" : "")  + String(minute(t)) + " ");

  if (gps.location.isValid() == true || ntp == true)
  {
    if (datetime != datetimeold) {
      Display.writeStr("tijd.txt", datetime);
      datetimeold = datetime;
    }
    if (dayname1 != dayname1old) {
      if (display_weather == true) Display.writeStr("d1name.txt", dayname1);
      dayname1old = dayname1;
    }
    if (dayname2 != dayname2old) {
      if (display_weather == true) Display.writeStr("d2name.txt", dayname2);
      dayname2old = dayname2;
    }
    if (gps.location.isValid() == true && display_gps == true) Display.writeStr("utctime.txt", String(gps.time.hour()) + ":" + (gps.time.minute() < 10 ? "0" : "") + String(gps.time.minute()) + ":" + (gps.time.second() < 10 ? "0" : "") + String(gps.time.second()));
  }
}

#ifndef testLib_h
#define testLib_h
////////////////////////////////////////////////////////
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FileData.h>
#include <LittleFS.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRac.h>
#include <IRutils.h>
#include <IRsend.h>
#include <FastBot.h>
////////////////////////////////////////////////////////
#include "html.h"
////////////////////////////////////////////////////////
#define NUM_BUTTONS 14
#define LENGHT_NAME 40
////////////////////////////////////////////////////////
struct Data {
  char ssid[40];
  char pass[40];
  bool tg = 0;
  char token[47];
  char names[NUM_BUTTONS][LENGHT_NAME];
  uint16_t codes[NUM_BUTTONS][199];
};
////////////////////////////////////////////////////////
//Штуки для ИК
const uint16_t kRecvPin = 2;  // D4, ПИН подсоединения ИК приемника
const uint16_t kIrLed = 4;    // D2, рекомендованный пин ИК передатчика
const uint16_t kCaptureBufferSize = 1024;

#if DECODE_AC
const uint8_t kTimeout = 50;
#else
const uint8_t kTimeout = 15;
#endif
const uint16_t kMinUnknownSize = 12;
////////////////////////////////////////////////////////
extern ESP8266WebServer server;
extern FileData data;
extern Data dat;
extern FastBot bot;
extern IRsend irsend;
////////////////////////////////////////////////////////
void editbt();
void handleRoot();
void readIR();
void del();
void save();
void nameread();
void sendIR();
void getName();
void gettg();
void tgdata();
void wifih();
void newMsg(FB_msg& msg);
void write_to_str(String text, int str);
void write_to_arr(uint16_t* ptr, int len, int str);
void setupWiFi();
void config();
void tick();

void newMsg(FB_msg& msg);

#endif
#define TINY_GSM_MODEM_SIM900

#include <TinyGsmClient.h>
#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>

// -------------------- RTC --------------------
RTC_DS3231 rtc;

// -------------------- GSM --------------------
SoftwareSerial modemSerial(7, 8);
TinyGsm modem(modemSerial);

// -------------------- P10 Display --------------------
SPIDMD display(1, 1);

// -------------------- Notice Message --------------------
String noticeMessage = "SMART NOTICE";

void setup() {

  // Serial Monitor
  Serial.begin(9600);

  // P10 Display
  display.begin();
  display.selectFont(SystemFont5x7);
  display.clearScreen();
  display.drawString(0, 0, "SMART");

  // GSM Serial
  modemSerial.begin(9600);
  delay(3000);

  Serial.println("Starting GSM Modem...");
  modem.restart();
  Serial.println("GSM Modem Started");

  // RTC
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  Serial.println("RTC Connected");

  // GSM SMS mode
  modem.sendAT("+CMGF=1");
  modem.waitResponse();

  modem.sendAT("+CNMI=1,2,0,0,0");
  modem.waitResponse();

  Serial.println("SMS Mode Ready");
}

void loop() {

  // -------------------- Read RTC --------------------
  DateTime now = rtc.now();

  // -------------------- Display Time --------------------
  display.clearScreen();

  String timeText = String(now.hour()) + ":" +
                    String(now.minute());

  display.drawString(0, 0, timeText);

  // -------------------- Serial Time --------------------
  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  // -------------------- Temperature --------------------
  Serial.print("Temperature: ");
  Serial.print(rtc.getTemperature());
  Serial.println(" C");

  // -------------------- Greeting --------------------
  if (now.hour() < 12) {

    Serial.println("Good Morning");

  }
  else if (now.hour() < 17) {

    Serial.println("Good Afternoon");

  }
  else {

    Serial.println("Good Evening");

  }

  // -------------------- Read SMS --------------------
  if (modemSerial.available()) {

    String sms = modemSerial.readString();

    Serial.println("SMS Received:");
    Serial.println(sms);

    // Store received SMS
    noticeMessage = sms;

    Serial.println("Notice Updated:");
    Serial.println(noticeMessage);

  }

  delay(1000);
}
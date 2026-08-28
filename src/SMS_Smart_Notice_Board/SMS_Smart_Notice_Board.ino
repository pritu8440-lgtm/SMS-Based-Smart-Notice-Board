#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

// =====================================================
// LED MATRIX - MAX7219
// =====================================================

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN 11
#define CLK_PIN 13
#define CS_PIN 10

MD_Parola display(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);


// =====================================================
// RTC DS1307
// =====================================================

RTC_DS1307 rtc;


// =====================================================
// SIM900 / GSM
//
// Arduino D7 = RX  <- GSM TX
// Arduino D8 = TX  -> GSM RX
// =====================================================

SoftwareSerial gsm(7, 8);


// =====================================================
// SETTINGS
// =====================================================

const unsigned long NORMAL_INTERVAL = 3000;


// =====================================================
// VARIABLES
// =====================================================

unsigned long lastNormalDisplay = 0;
bool rtcOK = false;


// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(9600);
  gsm.begin(9600);

  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println(" SMS BASED SMART NOTICE BOARD");
  Serial.println("================================");

  // ---------------------------------------------------
  // LED MATRIX
  // ---------------------------------------------------

  display.begin();
  display.setIntensity(3);
  display.displayClear();

  Serial.println("LED Matrix Ready");


  // ---------------------------------------------------
  // RTC
  // ---------------------------------------------------

  Wire.begin();

  if (rtc.begin())
  {
    rtcOK = true;
    Serial.println("RTC Connected");

    if (!rtc.isrunning())
    {
      Serial.println("RTC was stopped.");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
  }
  else
  {
    rtcOK = false;
    Serial.println("RTC NOT FOUND!");
  }


  // ---------------------------------------------------
  // GSM
  // ---------------------------------------------------

  Serial.println("Starting GSM Modem...");

  gsm.println("AT");
  delay(500);

  gsm.println("AT+CMGF=1");
  delay(500);

  gsm.println("AT+CNMI=1,2,0,0,0");
  delay(500);

  Serial.println("GSM Modem Started");
  Serial.println("SMS Mode Ready");


  // ---------------------------------------------------
  // STARTUP DISPLAY
  // ---------------------------------------------------

  showMessage("WELCOME TO COLLEGE");

  Serial.println();
  Serial.println("SYSTEM READY");
  Serial.println("----------------------------");
  Serial.println("WOKWI TEST:");
  Serial.println("Type any message in Serial Monitor");
  Serial.println("and press ENTER.");
  Serial.println("----------------------------");

  lastNormalDisplay = millis();
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // ---------------------------------------------------
  // 1. CHECK WOKWI SERIAL INPUT
  // ---------------------------------------------------

  if (Serial.available())
  {
    String message = Serial.readStringUntil('\n');
    message.trim();

    if (message.length() > 0)
    {
      Serial.print("TEST SMS: ");
      Serial.println(message);

      showMessage(message);

      Serial.println("Normal display resumed.");
      lastNormalDisplay = millis();
    }
  }


  // ---------------------------------------------------
  // 2. CHECK REAL GSM
  // ---------------------------------------------------

  if (gsm.available())
  {
    String gsmData = gsm.readString();

    Serial.println();
    Serial.println("GSM DATA:");
    Serial.println(gsmData);

    String message = extractSMS(gsmData);

    if (message.length() > 0)
    {
      Serial.print("SMS RECEIVED: ");
      Serial.println(message);

      showMessage(message);

      Serial.println("Normal display resumed.");
      lastNormalDisplay = millis();
    }
  }


  // ---------------------------------------------------
  // 3. NORMAL DISPLAY
  // ---------------------------------------------------

  if (millis() - lastNormalDisplay >= NORMAL_INTERVAL)
  {
    lastNormalDisplay = millis();

    showNormalDisplay();
  }
}


// =====================================================
// NORMAL DISPLAY
// =====================================================

void showNormalDisplay()
{
  if (!rtcOK)
  {
    showMessage("RTC ERROR");
    return;
  }

  DateTime now = rtc.now();


  // ---------------------------------------------------
  // GREETING
  // ---------------------------------------------------

  String greeting;

  if (now.hour() < 12)
  {
    greeting = "GOOD MORNING";
  }
  else if (now.hour() < 17)
  {
    greeting = "GOOD AFTERNOON";
  }
  else
  {
    greeting = "GOOD EVENING";
  }


  // ---------------------------------------------------
  // TIME
  // ---------------------------------------------------

  char timeText[16];

  sprintf(
    timeText,
    "TIME %02d:%02d",
    now.hour(),
    now.minute()
  );


  // ---------------------------------------------------
  // SERIAL OUTPUT
  // ---------------------------------------------------

  Serial.print("Time: ");
  Serial.println(timeText);

  Serial.println(greeting);


  // ---------------------------------------------------
  // DISPLAY
  // ---------------------------------------------------

  showMessage(greeting);
  showMessage(String(timeText));
  showMessage("WELCOME TO COLLEGE");
}


// =====================================================
// DISPLAY MESSAGE
// =====================================================

void showMessage(String message)
{
  message.trim();

  if (message.length() == 0)
  {
    return;
  }

  Serial.print("DISPLAY: ");
  Serial.println(message);

  display.displayClear();

  display.displayText(
    message.c_str(),
    PA_LEFT,
    70,
    800,
    PA_SCROLL_LEFT,
    PA_SCROLL_LEFT
  );

  while (!display.displayAnimate())
  {
    // Wait for scrolling to finish
  }

  display.displayClear();
}


// =====================================================
// EXTRACT SMS FROM SIM900 DATA
// =====================================================

String extractSMS(String data)
{
  int header = data.indexOf("+CMT:");

  if (header < 0)
  {
    return "";
  }

  // Find end of +CMT header
  int lineEnd = data.indexOf('\n', header);

  if (lineEnd < 0)
  {
    return "";
  }

  // SMS text starts after header
  String message = data.substring(lineEnd + 1);

  message.trim();

  // Remove anything after another line
  int nextLine = message.indexOf('\n');

  if (nextLine >= 0)
  {
    message = message.substring(0, nextLine);
  }

  message.trim();

  return message;
}
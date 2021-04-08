/* Blayone Rocket Project - BONESOFT
      Version 0.1
    Feburary, 20th 2021 */

/* Current version of Bonesoft does not include bluetooth
   as development with bluetooth is impossible for me. To 
   use bluetooth with React Native I would need to edit native
   libraries and for me to export would require an up to date
   Macbook to have access to IOS Development Tools */

/* Current version of Bonesoft also does not contain
   SPI Slave code as there is no need for Bone to know
   the telemetry of Arch at the moment. When BoneApp is fully 
   functional, then I will need it */

/* -------------
    Libraries
---------------- */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <Servo.h>

/* -----------------
    Definitions
------------------ */
#define VERSION 0.2
#define accessmode 1 // 1 is Acess Point, 0 is Home Network

#define dash Serial
#define debug Serial1

#if accessmode == 0
const char *ssid = "CatTower";
const char *password = "***REMOVED***";

#else
const char *ssid = "BoneNetwork";
const char *password = "brpLaunchPad";

IPAddress ip(192, 168, 0, 195);

#endif

#define LeftServoPin 4   //D2
#define RightServoPin 12 //D6
#define PullServoPin 14  //D5

Servo lS;
Servo rS;
Servo pS;

ESP8266WebServer server(80);
MDNSResponder mdns;

String webPage;
String status;
String archdata;

StaticJsonDocument<400> statusJSON;

int timeoutTime = 2;
double lastReceived = 0;
double currentTime = 0;

bool support = false;
bool launch = false;
bool man = true;
bool manToggle = false;

void handle_ArchData(), handle_Status(), handle_Launch(), handle_Support(), handle_Man();
void deploySupport(), manSupport();

/* --------------------------------
    Main Functions (SETUP, LOOP)
------------------------------------ */

void setup(void)
{
  // preparing GPIOs
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  lS.attach(LeftServoPin);
  rS.attach(RightServoPin);
  pS.attach(PullServoPin);

  lS.write(0);
  rS.write(30);
  pS.write(0);

  debug.begin(500000);
  debug.print("Starting Server$");

  dash.begin(250000);
  dash.setTimeout(30);

#if accessmode == 0
  WiFi.begin(ssid, password);
  WiFi.hostname("bone");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  IPAddress myIP = WiFi.localIP();

  debug.print("Wifi Connection Successful$");
  debug.print(myIP);
  debug.print("$");

#else
  WiFi.mode(WIFI_AP); //Only Access point
  WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
  WiFi.hostname("bone");
  WiFi.softAP(ssid, password); //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP(); //Get IP address

#endif

  statusJSON["bone"] = true;
  statusJSON["dash"] = false;
  serializeJson(statusJSON, status);

  server.on("/archdata", handle_ArchData);
  server.on("/status", handle_Status);
  server.on("/launch", handle_Launch);
  server.on("/support", handle_Support);
  server.on("/man", handle_Man);

  server.begin();
}

void loop(void)
{
  currentTime = millis() / 1000;

  if (support)
  {
    deploySupport();
    support = false;
  }

  if (manToggle)
  {
    manSupport();
    manToggle = false;
  }

  if (dash.available() > 0)
  {
    String message = dash.readStringUntil('$');
    StaticJsonDocument<2048> doc;

    DeserializationError err = deserializeJson(doc, message);
    digitalWrite(LED_BUILTIN, LOW);
    if (err == DeserializationError::Ok)
    {
      status = "";
      archdata = "";
      statusJSON["dash"] = true;
      serializeJson(doc, archdata);
      serializeJson(statusJSON, status);
      lastReceived = currentTime;
      debug.print("JSON data$");
    }
    else
    {
      debug.print("Not JSON data$");
    }
  }

  if (((currentTime - lastReceived) > timeoutTime) && (statusJSON["dash"] == true))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    status = "";
    statusJSON["dash"] = false;
    serializeJson(statusJSON, status);
  }

  server.handleClient();
}

void handle_ArchData()
{
  server.send(200, "text/plain", archdata);
}
void handle_Status()
{
  server.send(200, "text/plain", status);
}
void handle_Launch()
{
  launch = true;
  server.send(200, "text/plain", "Launch Received");
}
void handle_Support()
{
  support = true;
  server.send(200, "text/plain", "Deploy Support Received");
}
void handle_Man()
{
  manToggle = true;
  server.send(200, "text/plain", "Toggling Manual Mode");
}

void manSupport()
{
  if (man)
  {
    lS.detach();
    rS.detach();
    pS.detach();
    man = false;
  }
  else
  {
    lS.attach(LeftServoPin);
    rS.attach(RightServoPin);
    pS.attach(PullServoPin);

    lS.write(0);
    rS.write(30);
    pS.write(0);
    man = true;
  }
}

void deploySupport()
{
  lS.write(30);
  rS.write(0);
  pS.write(70);
  delay(300);
  lS.write(0);
  rS.write(30);
}
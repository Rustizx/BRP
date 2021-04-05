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

/* -----------------
    Definitions
------------------ */
#define VERSION 0.2
#define accessmode 0 // 1 is Acess Point, 0 is Home Network

#define dash Serial
#define debug Serial1

#if accessmode == 0
const char *ssid = "CatTower";
const char *password = "***REMOVED***";

#else
const char *ssid = "BoneNetwork";
const char *password = "brpLaunchPad";

IPAddress ip(192, 168, 1, 3);
IPAddress subnet(255, 255, 255, 0);

#endif

ESP8266WebServer server(80);
MDNSResponder mdns;

String example = "{'config':{'version':0.17,'orientationType':'q','servoX_offset':101,'servoY_offset':86,'recalibrateGyro':true,'gyroX_offset':0,'gyroY_offset':0,'gyroZ_offset':0,'kP':0.08,'kI':0.02,'kD':0.035,'attemptDataRecovery':false,'tvcMountTroubleshoot':false},'telemetry':{'state':1,'gyro':[-18.40791,10.55346,-4.198361],'accel':[1690,113,8417],'ore':[0,0,0],'altitude':-0.071899,'realAltitude':591.3943,'temperature':22.46,'tvcPOS':[71,56],'pyro':[0,0],'error':[0,0],'pidx':[0,0,0],'pidy':[0,0,0],'pid':[0,0],'pwm':[0,0],'time':103.216,'millis':103216}}";

String webPage;
String status;
String archdata;

StaticJsonDocument<400> statusJSON;

int timeoutTime = 2;
double lastReceived = 0;
double currentTime = 0;

/* --------------------------------
    Main Functions (SETUP, LOOP)
------------------------------------ */

void setup(void)
{
  webPage += "<h1>Bone Control Pannel</h1><p>StatusLED <a href=\"ledON\"><button>ON</button></a>&nbsp;<a href=\"ledOFF\"><button>OFF</button></a>&nbsp;<a href=\"ArchData\"><button>ArchData</button></a>&nbsp;<a href=\"ArchDataPretty\"><button>ArchDataPretty</button></a>&nbsp;<a href=\"ArchDataExample\"><button>ArchDataExample</button></a></p>";

  // preparing GPIOs
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

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
  WiFi.hostname("bone");
  WiFi.softAP(ssid, password); //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP(); //Get IP address

#endif

  statusJSON["bone"] = true;
  statusJSON["dash"] = false;
  serializeJson(statusJSON, status);

  if (mdns.begin("bone", myIP))
  {
  }

  server.on("/", []() {
    server.send(200, "text/html", webPage);
  });

  server.on("/archdata", []() {
    server.send(200, "text/plain", archdata);
  });

  server.on("/status", []() {
    server.send(200, "text/plain", status);
  });

  server.begin();
}

void loop(void)
{
  currentTime = millis() / 1000;

  debug.print("Beginning Tick - ");
  debug.print(currentTime);
  debug.print("$");

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

    //archdata = dash.readStringUntil('$');
  }

  if (((currentTime - lastReceived) > timeoutTime) && (statusJSON["dash"] == true))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    status = "";
    statusJSON["dash"] = false;
    serializeJson(statusJSON, status);
    debug.print("dash timeout$");
  }

  debug.print(archdata);
  debug.print("$");

  debug.print(status);
  debug.print("$");

  debug.print("----------------------------------------------$");

  server.handleClient();
}
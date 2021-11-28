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
#define VERSION 0.1

#define accessmode 0 // 1 is Acess Point, 0 is Home Network

#define statusLED 5

#if accessmode == 0
const char *ssid = "CatTower";
const char *password = "";

#else
const char *ssid = "BoneNetwork";
const char *password = "brpLaunchPad";

IPAddress ip(192, 168, 1, 3);
IPAddress subnet(255, 255, 255, 0);

#endif

ESP8266WebServer server(80);
MDNSResponder mdns;

String webPage = "";
String exampleRocketData = "";

String rocketData = "";
String rocketDataPretty = "";
String status = "";
String test = "";

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
    exampleRocketData += "{'config':{'version':0.17,'orientationType':'q','servoX_offset':101,'servoY_offset':86,'recalibrateGyro':true,'gyroX_offset':0,'gyroY_offset':0,'gyroZ_offset':0,'kP':0.08,'kI':0.02,'kD':0.035,'attemptDataRecovery':false,'tvcMountTroubleshoot':false},'telemetry':{'state':1,'gyro':[18.38445,0.020511,-0.039196],'accel':[-1,0,8188],'ore':[0,0,0],'altitude':-0.077332,'realAltitude':676.3664,'temperature':23.28,'tvcPOS':[71,56],'pyro':[0,0],'error':[0,0],'pidx':[0,0,0],'pidy':[0,0,0],'pid':[0,0],'pwm':[0,0],'time':67.634,'millis':67634}}";

    // preparing GPIOs
    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(statusLED, OUTPUT);
    digitalWrite(statusLED, LOW);
    digitalWrite(BUILTIN_LED, HIGH);

    delay(1000);
    Serial.begin(4800);

#if accessmode == 0
    WiFi.begin(ssid, password);
    WiFi.hostname("bone");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    IPAddress myIP = WiFi.localIP();

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

    server.on("/", []()
              { server.send(200, "text/html", webPage); });
    server.on("/ledON", []()
              {
                  server.send(200, "text/html", webPage);
                  digitalWrite(statusLED, HIGH);
                  Serial.write("1");
                  delay(1000);
              });
    server.on("/ledOFF", []()
              {
                  server.send(200, "text/html", webPage);
                  digitalWrite(statusLED, LOW);
                  Serial.write("0");
                  delay(1000);
              });

    server.on("/ArchData", []()
              { server.send(200, "text/plain", rocketData); });

    server.on("/ArchDataPretty", []()
              { server.send(200, "text/plain", rocketDataPretty); });

    server.on("/ArchDataExample", []()
              { server.send(200, "text/plain", exampleRocketData); });

    server.on("/status", []()
              { server.send(200, "text/plain", status); });

    server.on("/test", []()
              { server.send(200, "text/plain", test); });

    server.begin();
}

void loop(void)
{
    currentTime = millis() / 1000;

    if (Serial.available() > 0)
    {
        StaticJsonDocument<2048> doc;

        DeserializationError err = deserializeJson(doc, Serial);
        digitalWrite(BUILTIN_LED, LOW);
        delay(25);
        if (err == DeserializationError::Ok)
        {
            rocketData = "";
            status = "";
            serializeJson(doc, rocketData);
            statusJSON["dash"] = true;
            serializeJson(statusJSON, status);
            lastReceived = currentTime;
        }
        else
        {
            /* while (Serial.available() > 0)
      {
        test += Serial.read();
      } */
        }
    }

    if ((currentTime - lastReceived) > timeoutTime)
    {
        digitalWrite(BUILTIN_LED, HIGH);
        status = "";
        statusJSON["dash"] = false;
        serializeJson(statusJSON, status);
    }

    server.handleClient();
}
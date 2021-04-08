/* Blayone Rocket Project - ARCHSOFT
      Version 0.17
    October, 18th 2020 */

/* System States: 
 * 0 = Setup
 * 1 = Ready For Launch
 * 2 = Flight - PID Controlled Asscent
 * 3 = MECO
 * 4 = Parachute Controlled Descent
 * 5 = End of Flight
 */

/* -------------
    Libraries
---------------- */
#include <Arduino.h>
#include <ArchFunctions.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Adafruit_BMP280.h>
#include <SDConfigFile.h>
#include <math.h>
#include <Wire.h>

/* -----------------
    Definitions
------------------ */
#define VERSION 0.17
#define SERIAL_BAND 250000
#define DEBUG false

int state;
char buffer[6];

double dt, currentTime, previousTime, setupTime;

float flightStateTime;
float mecoStateTime;
float paraStateTime;

IntervalTimer logTimer;

/* --------------------
    Arch Outputs INIT
------------------------ */
AlertSystem alert(2, 0, 1, 15);  // Red Pin = 2, Green Pin = 0, Blue Pin = 1, Buzzer Pin = 15
TVCMount tvc(6, 20);             // TVCx Pin = 6, TVCx Pin = 20 / HEADER 4 (OLD : TVXy Pin = 5)
Pyro pyro(8, 7);                 // Pyro Channel 1 Pin = 8, Pyro Channel 2 Pin = 7
Debug debug(SERIAL_BAND, DEBUG); // Bone Communication uses the same UART as the USB, therefore to connect Bone you cannot use the Serial Monitor

/* --------------
    IMU INIT
---------------- */
MPU6050 mpu;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// Quaterions
Quaternion q;        // [w, x, y, z]         quaternion container
VectorInt16 aa;      // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;  // [x, y, z]            gravity-free accel sensor measurements
VectorFloat gravity; // [x, y, z]            gravity vector
float eulerRad[3];   // [psi, theta, phi]
float eularDeg[3];   // [psi, theta, phi]

// Rotation Matrices
int16_t AcX, AcY, AcZ, Tmp, GyX, GyroY, GyroZ;
double PIDX, PIDY, errorX, errorY, previous_errorX, previous_errorY, pwmX, pwmY, previouslog, OutX, OutY, OutZ, OreX, OreY, OreZ;
double PreviousGyroX, PreviousGyroY, PreviousGyroZ, IntGyroX, IntGyroY, RADGyroX, RADGyroY, RADGyroZ, RawGyZ, DifferenceGyroX, DifferenceGyroY, DifferenceGyroZ;
double matrix1, matrix2, matrix3, matrix4, matrix5, matrix6, matrix7, matrix8, matrix9;
double PrevGyX, PrevGyY, PrevGyZ, RawGyX, RawGyY;

double OrientationX = 0;
double OrientationY = 0;
double OrientationZ = 1;
double Ax;
double Ay;
double Az;

// Upright Angle of the Flight Computer
int desired_angleX = 0;
int desired_angleY = 0;

//"P" Constants
float pidX_p = 0;
float pidY_p = 0;

//"I" Constants
float pidX_i = 0;
float pidY_i = 0;

//"D" Constants
float pidX_d = 0;
float pidY_d = 0;

/* --------------
    BARO INIT
---------------- */
Adafruit_BMP280 bmp;

double realAltitude;
double calibratedAltitude;
double altitude;

double temperature;

/* --------------
    BONE CONNECT
---------------- */
#define BONESERIAL Serial1
#define BONE_BAUD 250000
#define BONETX 5  // TVC-Y Pin
#define BONERX 21 // Header 3

StaticJsonDocument<1024> dataJSON;

struct boneData
{
  bool launch;
  int countdownTime;

  int servoX_offset;
  int servoY_offset;
  bool recalibrateGyro;
};

boneData boneRec;

/* --------------------------------------
    SD Card + Configuration File INIT
----------------------------------------- */
const int SDchipSelect = 10;

File realTimeLOG;
File postFlightLOG;

const char CONFIG_FILE[] = "ARCHCO~1.CFG";

struct configData
{
  float version;
  String orientationType;

  int servoX_offset;
  int servoY_offset;
  float servoX_gear_ratio;
  float servoY_gear_ratio;

  bool recalibrateGyro;
  float gyroX_offset;
  float gyroY_offset;
  float gyroZ_offset;

  double kP;
  double kI;
  double kD;

  bool boneSPI;
  bool hardWire;
  float timeToLaunch;

  bool attemptDataRecovery;
  bool tvcMountTroubleshoot;
};
configData config;

/* -----------------
    Setup Booleans
-------------------- */
bool flashChip_init = false;
bool sdCard_init = false;
bool bmp_init = false;
bool imu_init = false;
bool config_init = false;

/* -------------
    Functions
---------------- */

// Reads configuration file from SD Card
boolean readConfiguration()
{
  const uint8_t CONFIG_LINE_LENGTH = 127;

  // The open configuration file.
  SDConfigFile cfg;

  // Open the configuration file.
  if (!cfg.begin(CONFIG_FILE, CONFIG_LINE_LENGTH))
  {
    debug.print("Failed to open configuration file: ");
    debug.println(String(CONFIG_FILE));
    return false;
  }

  // Read each setting from the file.
  while (cfg.readNextSetting())
  {
    if (cfg.nameIs("version"))
    {
      config.version = atof(cfg.getValue());
      dataJSON["config"]["version"] = config.version;
    }
    else if (cfg.nameIs("orientationType"))
    {
      config.orientationType = cfg.copyValue();
      dataJSON["config"]["orientationType"] = config.orientationType;
    }
    else if (cfg.nameIs("servoX_offset"))
    {
      config.servoX_offset = cfg.getIntValue();
      dataJSON["config"]["servoX_offset"] = config.servoX_offset;
    }
    else if (cfg.nameIs("servoY_offset"))
    {
      config.servoY_offset = cfg.getIntValue();
      dataJSON["config"]["servoY_offset"] = config.servoY_offset;
    }
    else if (cfg.nameIs("gyroX_offset"))
    {
      config.gyroX_offset = atof(cfg.getValue());
      dataJSON["config"]["gyroX_offset"] = config.gyroX_offset;
    }
    else if (cfg.nameIs("gyroY_offset"))
    {
      config.gyroY_offset = atof(cfg.getValue());
      dataJSON["config"]["gyroY_offset"] = config.gyroY_offset;
    }
    else if (cfg.nameIs("gyroZ_offset"))
    {
      config.gyroZ_offset = atof(cfg.getValue());
      dataJSON["config"]["gyroZ_offset"] = config.gyroZ_offset;
    }
    else if (cfg.nameIs("recalibrateGyro"))
    {
      config.recalibrateGyro = cfg.getBooleanValue();
      dataJSON["config"]["recalibrateGyro"] = config.recalibrateGyro;
    }
    else if (cfg.nameIs("kP"))
    {
      config.kP = atof(cfg.getValue());
      dataJSON["config"]["kP"] = config.kP;
    }
    else if (cfg.nameIs("kI"))
    {
      config.kI = atof(cfg.getValue());
      dataJSON["config"]["kI"] = config.kI;
    }
    else if (cfg.nameIs("kD"))
    {
      config.kD = atof(cfg.getValue());
      dataJSON["config"]["kD"] = config.kD;
    }
    else if (cfg.nameIs("attemptDataRecovery"))
    {
      config.attemptDataRecovery = cfg.getBooleanValue();
      dataJSON["config"]["attemptDataRecovery"] = config.attemptDataRecovery;
    }
    else if (cfg.nameIs("tvcMountTroubleshoot"))
    {
      config.tvcMountTroubleshoot = cfg.getBooleanValue();
      dataJSON["config"]["tvcMountTroubleshoot"] = config.tvcMountTroubleshoot;
    }
  }

  // Clean up
  cfg.end();

  return true;
}

// Used to move tvc mount servos to specific values, to find center values
void tvcMountTroubleshoot()
{
  debug.println("-- TVC Mount TroubleShoot Mode --");
  debug.println("1 = xAxis, 2 = yAxis");
  int index;
  bool Xaxis = true;
  while (true)
  {
    if (DEBUG)
    {
      index = Serial.readBytesUntil('\n', buffer, 5); //newline or max of 5 chars
      buffer[index] = '\0';
    }

    int n = atoi(buffer); //convert readString into a number
    debug.println(n);     //so you can see the integer

    if (n > 10)
    {
      if (Xaxis)
      {
        tvc.setX(n);
      }
      else
      {
        tvc.setY(n);
      }
    }
    else if (n == 1)
    {
      debug.println("Set to Xaxis");
      Xaxis = true;
    }
    else if (n == 2)
    {
      debug.println("Set to Yaxis");
      Xaxis = false;
    }
    buffer[0] = '\0';
  }
}

// Updates Gyro and Accel Data
void mpuUpdate()
{
  mpu.resetFIFO();
  fifoCount = mpu.getFIFOCount();

  while (fifoCount < packetSize)
    fifoCount = mpu.getFIFOCount();

  mpu.getFIFOBytes(fifoBuffer, packetSize);

  if (fifoCount >= 1024)
  {
    mpu.resetFIFO();
    debug.println(F("FIFO overflow!"));
  }
  else
  {
    mpu.dmpGetQuaternion(&q, fifoBuffer);

    mpu.dmpGetEuler(eulerRad, &q);
    eularDeg[0] = eulerRad[0] * 180 / M_PI;
    eularDeg[1] = eulerRad[1] * 180 / M_PI;
    eularDeg[2] = eulerRad[2] * 180 / M_PI;

    mpu.dmpGetAccel(&aa, fifoBuffer);
  }
}

// Updates Sensors on board Arch
void sensorUpdate()
{
  mpuUpdate();

  temperature = bmp.readTemperature();

  realAltitude = bmp.readAltitude();
  altitude = realAltitude - calibratedAltitude;
}

// Calculates Angle needed to stabilize rocket
void pidCompute()
{
  previous_errorX = errorX;
  previous_errorY = errorY;

  // Calculate Error
  errorX = Ax - desired_angleX;
  errorY = Ay - desired_angleY;

  // Defining "P"
  pidX_p = config.kP * errorX;
  pidY_p = config.kP * errorY;

  // Defining "D"
  pidX_d = config.kD * ((errorX - previous_errorX) / dt);
  pidY_d = config.kD * ((errorY - previous_errorY) / dt);

  // Defining "I"
  pidX_i = config.kI * (pidX_i + errorX * dt);
  pidY_i = config.kI * (pidY_i + errorY * dt);

  // Adding it all up
  PIDX = pidX_p + pidX_i + pidX_d;
  PIDY = pidY_p + pidY_i + pidY_d;

  pwmY = ((PIDY * config.servoY_gear_ratio) + config.servoX_offset);
  pwmX = ((PIDX * config.servoX_gear_ratio) + config.servoY_offset);
}

// Track Rocket Orientation in 3D Space using Quaterion, no gimbal lock (yay!)
void Quaterion()
{
  // MPU6050 already stores data in quaterions, extract eular angles from them
  Ax = (eularDeg[2]);
  Ay = (eularDeg[0]);
}

// Track Rocket Orientation in 3D Space using Rotation Matrices, no gimbal lock (again yay!)
void RotationMatrices()
{
  PreviousGyroX = RADGyroX;
  PreviousGyroY = RADGyroY;
  PreviousGyroZ = RADGyroZ;

  /*
  RADGyroX = (mpu.getGyroAngleX()) * (PI / 180);
  RADGyroY = (mpu.getGyroAngleY()) * (PI / 180);
  RADGyroZ = (mpu.getGyroAngleZ()) * (PI / 180);
  */

  //debug.print(mpu.getGyroAngleX()); debug.print("/");debug.print(mpu.getGyroAngleY()); debug.print("/");debug.println(mpu.getGyroAngleZ());
  //debug.print(RADGyroX);debug.print("/");debug.print(RADGyroY);debug.print("/");debug.print(RADGyroZ);debug.println();

  DifferenceGyroX = (RADGyroX - PreviousGyroX);
  DifferenceGyroY = (RADGyroY - PreviousGyroY);
  DifferenceGyroZ = (RADGyroZ - PreviousGyroZ);

  //debug.print(DifferenceGyroX);debug.print("/");debug.print(DifferenceGyroY);debug.print("/");debug.print(DifferenceGyroZ);debug.println();

  OreX = OrientationX;
  OreY = OrientationY;
  OreZ = OrientationZ;

  //X Matrices
  matrix1 = (cos(DifferenceGyroZ) * cos(DifferenceGyroY));
  matrix2 = (((sin(DifferenceGyroZ) * -1) * cos(DifferenceGyroX) + (cos(DifferenceGyroZ)) * sin(DifferenceGyroY) * sin(DifferenceGyroX)));
  matrix3 = ((sin(DifferenceGyroZ) * sin(DifferenceGyroX) + (cos(DifferenceGyroZ)) * sin(DifferenceGyroY) * cos(DifferenceGyroX)));

  //Y Matrices
  matrix4 = sin(DifferenceGyroZ) * cos(DifferenceGyroY);
  matrix5 = ((cos(DifferenceGyroZ) * cos(DifferenceGyroX) + (sin(DifferenceGyroZ)) * sin(DifferenceGyroY) * sin(DifferenceGyroX)));
  matrix6 = (((cos(DifferenceGyroZ) * -1) * sin(DifferenceGyroX) + (sin(DifferenceGyroZ)) * sin(DifferenceGyroY) * cos(DifferenceGyroX)));

  //Z Matrices
  matrix7 = (sin(DifferenceGyroY)) * -1;
  matrix8 = cos(DifferenceGyroY) * sin(DifferenceGyroX);
  matrix9 = cos(DifferenceGyroY) * cos(DifferenceGyroX);

  OrientationX = ((OreX * matrix1)) + ((OreY * matrix2)) + ((OreZ * matrix3));
  OrientationY = ((OreX * matrix4)) + ((OreY * matrix5)) + ((OreZ * matrix6));
  OrientationZ = ((OreX * matrix7)) + ((OreY * matrix8)) + ((OreZ * matrix9));

  //debug.print(OrientationX);debug.print("/");debug.print(OrientationY);debug.print("/");debug.print(OrientationZ);debug.println();

  OutX = OrientationX * 60;
  OutY = OrientationY * -60;
  //https://www.gregslabaugh.net/publications/euler.pdf

  Ax = asin(OrientationX) * (180 / PI);
  Ay = asin(OrientationY) * (180 / PI);
  Az = asin(OrientationZ) * (180 / PI);
}

// Update Tracking of the Rocket using configured method
void updatePos()
{
  if (config.orientationType == 'q')
  {
    Quaterion();
  }
  else if (config.orientationType == 'r')
  {
    RotationMatrices();
  }
}

// Update JSON Data Document
void jsonupdate(char type = 'a')
{
  // Update Configuration Settings
  if ((type == 'c') || (type == 'a'))
  {
    dataJSON["config"]["version"] = config.version;
    dataJSON["config"]["orientationType"] = config.orientationType;
    dataJSON["config"]["servoX_offset"] = config.servoX_offset;
    dataJSON["config"]["servoY_offset"] = config.servoY_offset;
    dataJSON["config"]["gyroX_offset"] = config.gyroX_offset;
    dataJSON["config"]["gyroY_offset"] = config.gyroY_offset;
    dataJSON["config"]["gyroZ_offset"] = config.gyroZ_offset;
    dataJSON["config"]["recalibrateGyro"] = config.recalibrateGyro;
    dataJSON["config"]["kP"] = config.kP;
    dataJSON["config"]["kI"] = config.kI;
    dataJSON["config"]["kD"] = config.kD;
    dataJSON["config"]["attemptDataRecovery"] = config.attemptDataRecovery;
    dataJSON["config"]["tvcMountTroubleshoot"] = config.tvcMountTroubleshoot;
  }
  // Update Telemetry
  if ((type == 't') || (type == 'a'))
  {
    dataJSON["telemetry"]["state"] = state;
    dataJSON["telemetry"]["gyro"][0] = eularDeg[0];
    dataJSON["telemetry"]["gyro"][1] = eularDeg[1];
    dataJSON["telemetry"]["gyro"][2] = eularDeg[2];
    dataJSON["telemetry"]["accel"][0] = aa.x;
    dataJSON["telemetry"]["accel"][1] = aa.y;
    dataJSON["telemetry"]["accel"][2] = aa.z;
    dataJSON["telemetry"]["ore"][0] = Ax;
    dataJSON["telemetry"]["ore"][1] = Ay;
    dataJSON["telemetry"]["ore"][2] = Az;
    dataJSON["telemetry"]["altitude"] = altitude;
    dataJSON["telemetry"]["realAltitude"] = realAltitude;
    dataJSON["telemetry"]["temperature"] = temperature;
    dataJSON["telemetry"]["tvcPOS"][0] = tvc.currentPosX;
    dataJSON["telemetry"]["tvcPOS"][1] = tvc.currentPosY;
    dataJSON["telemetry"]["pyro"][0] = pyro.pyro1;
    dataJSON["telemetry"]["pyro"][1] = pyro.pyro2;
    dataJSON["telemetry"]["error"][0] = errorX;
    dataJSON["telemetry"]["error"][1] = errorY;
    dataJSON["telemetry"]["pidx"][0] = pidX_p;
    dataJSON["telemetry"]["pidx"][1] = pidX_i;
    dataJSON["telemetry"]["pidx"][2] = pidX_d;
    dataJSON["telemetry"]["pidy"][0] = pidY_p;
    dataJSON["telemetry"]["pidy"][1] = pidY_i;
    dataJSON["telemetry"]["pidy"][2] = pidY_d;
    dataJSON["telemetry"]["pid"][0] = PIDX;
    dataJSON["telemetry"]["pid"][1] = PIDY;
    dataJSON["telemetry"]["pwm"][0] = pwmX;
    dataJSON["telemetry"]["pwm"][1] = pwmY;
    dataJSON["telemetry"]["time"] = dt;
    dataJSON["telemetry"]["millis"] = currentTime;
  }
}

// Log Data to SD Card
void logData()
{
  if (!config.attemptDataRecovery)
  {
    realTimeLOG = SD.open("data.txt", FILE_WRITE);

    String dataString = "";

    dataString += String(eularDeg[0]); // Gyro X
    dataString += ",";
    dataString += String(eularDeg[1]); // Gyro Y
    dataString += ",";
    dataString += String(eularDeg[2]); // Gyro Z
    dataString += ",";
    dataString += String(aa.x); // Accel X
    dataString += ",";
    dataString += String(aa.y); // Accel Y
    dataString += ",";
    dataString += String(aa.z); // Accel Z
    dataString += ",";
    dataString += String(Ax); // Ore X
    dataString += ",";
    dataString += String(Ay); // Ore Y
    dataString += ",";
    dataString += String(Az); // Ore Z
    dataString += ",";
    dataString += String(altitude); // Altitude
    dataString += ",";
    dataString += String(realAltitude); // Real Altitude (not calibrated to ground)
    dataString += ",";
    dataString += String(temperature); // Temperature
    dataString += ",";
    dataString += String(state); // Current State
    dataString += ",";
    dataString += String(tvc.currentPosX); // Current TVC Servo X Pos
    dataString += ",";
    dataString += String(tvc.currentPosY); // Current TVC Servo Y Pos
    dataString += ",";
    dataString += String(pyro.pyro1); // Current Pyro Channel 1 Status
    dataString += ",";
    dataString += String(pyro.pyro2); // Current Pyro Channel 2 Status
    dataString += ",";
    dataString += String(errorX); // error X
    dataString += ",";
    dataString += String(errorY); // error Y
    dataString += ",";
    dataString += String(pidX_p); // PID xP
    dataString += ",";
    dataString += String(pidX_i); // PID xI
    dataString += ",";
    dataString += String(pidX_d); // PID xD
    dataString += ",";
    dataString += String(pidY_p); // PID yP
    dataString += ",";
    dataString += String(pidY_i); // PID yI
    dataString += ",";
    dataString += String(pidY_d); // PID yD
    dataString += ",";
    dataString += String(PIDX); // PIDX
    dataString += ",";
    dataString += String(PIDY); // PIDY
    dataString += ",";
    dataString += String(pwmX); // PWMX
    dataString += ",";
    dataString += String(pwmY); // PWMY
    dataString += ",";
    dataString += String(dt);
    dataString += ",";
    dataString += String(currentTime);
    dataString += "";

    realTimeLOG.println(dataString);

    realTimeLOG.close();
  }
}

/* --------------------------------
    Main Functions (SETUP, LOOP)
------------------------------------ */

void setup()
{
  debug.println();
  debug.println();
  debug.print("Hello, I am Arch version ");
  debug.print(VERSION);
  debug.println("! Lets get started.");
  debug.println();
  debug.println();

  debug.println(" ---- Initalizing Hardware ---- ");
  debug.println();

  // Pin Setups
  debug.println("Setting Up Pins...");
  pinMode(SDchipSelect, OUTPUT);

  debug.println("Success!");
  debug.println();

  // SD Card Initialization
  debug.println("SD Card Initalizing...");
  if (!SD.begin(SDchipSelect))
  {
    debug.println("Failed!");
    debug.println();
    sdCard_init = false;
  }
  else
  {
    debug.println("Success! Reading Config File...");
    debug.println();
    sdCard_init = true;
    config_init = readConfiguration();
    if (config_init)
    {
      debug.println("Success! Configuration Complete!");
      debug.println();
      config_init = true;
      realTimeLOG = SD.open("data.txt", FILE_WRITE);
      realTimeLOG.println("-----------------------------");
      realTimeLOG.close();
    }
    else
    {
      debug.println("Failed! Could not read config file");
      debug.println();
      config_init = false;
    }
  }

  // Attach TVC Mount Servos and Set Offsets
  tvc.init(config.servoX_offset, config.servoY_offset);

  // Barometer Initialization
  debug.println("Barometer Initalizing...");
  if (!bmp.begin())
  {
    debug.println("Failed!");
    debug.println();
    bmp_init = false;
  }
  else
  {
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X4,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    debug.println("Success!");
    debug.println();
    bmp_init = true;
  }

  // IMU Initialization
  debug.println("IMU Initalizing...");
  Wire.begin();
  Wire.setClock(400000);

  mpu.initialize();

  debug.println(F("Testing device connections..."));
  debug.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
  if (!mpu.testConnection())
  {
    imu_init = false;
  }

  debug.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);

  if (devStatus == 0)
  {
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);

    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize();

    imu_init = true;
  }
  else
  {
    debug.print(F("DMP Initialization failed (code "));
    debug.print(devStatus);
    debug.println(F(")"));
    imu_init = false;
  }

  debug.println("Success!");
  debug.println();

  // Flash Clip Initialization ( ** Need To Create Custom Library ** )
  flashChip_init = true;

  // Bone SPI Connection Init Initialization
  debug.println("Bone SPI Initalizing...");

  if (!DEBUG)
  {
    BONESERIAL.begin(BONE_BAUD);
    BONESERIAL.setTimeout(50);
    BONESERIAL.setTX(BONETX);
    BONESERIAL.setRX(BONERX);
  }

  debug.println("Success!");
  debug.println();

  // Setup Check
  if (((bmp_init == 0) || (imu_init == 0) || (sdCard_init == 0) || (flashChip_init == 0)))
  {
    debug.println(" ---- Failed To Start ( Hardware Malfunction ) ---- ");
    while (1)
    {
      alert.error(HARDWARE_ERROR);
    }
  }
  else if (!(config.version == VERSION) || (config_init == 0))
  {
    debug.println(" ---- Failed To Start ( SOFTWARE ERROR ) ---- ");
    while (1)
    {
      alert.error(SOFTWARE_ERROR);
    }
  }
  else
  {
    debug.println(" ---- Hardware Startup Complete ---- ");
    debug.println();
    debug.println();
    alert.success();
  }

  // Post Setup
  realAltitude = bmp.readAltitude();
  calibratedAltitude = realAltitude;
  altitude = 0;

  tvc.setupMovement();
  state = 1;

  sensorUpdate();
  setupTime = millis();
  //logTimer.begin(logData, 25000);
}

void loop()
{
  // Keep track of the current time
  currentTime = millis() - setupTime;
  dt = (currentTime) / 1000;

  // Check config if Troubleshooting Functions are enabled
  if (config.tvcMountTroubleshoot)
  {
    tvcMountTroubleshoot();
  }

  // "Waiting for Launch" State
  if (state == 1)
  {
    sensorUpdate();
    jsonupdate();
    serializeJson(dataJSON, BONESERIAL);
    BONESERIAL.print("$");

    if (BONESERIAL.available() > 0)
    {
      String message = BONESERIAL.readStringUntil('$');
      StaticJsonDocument<200> doc;

      DeserializationError err = deserializeJson(doc, message);
      if (err == DeserializationError::Ok)
      {
        boneRec.countdownTime = doc["countdownTime"];
        boneRec.launch = doc["launch"];
        boneRec.recalibrateGyro = doc["recalibrateGyro"];
        boneRec.servoX_offset = doc["servoX_offset"];
        boneRec.servoY_offset = doc["servoY_offset"];
      }
    }

    delay(100);

    if (boneRec.launch) //|| aa.z > 1.6) // If end of countdown reached or acceleration up is greater than 1.6g, then switch to Flight State
    {
      double timeSinceLaunchRec = dt;
      while (true)
      {
        currentTime = millis() - setupTime;
        dt = (currentTime) / 1000;

        int tminus = boneRec.countdownTime - (dt - timeSinceLaunchRec);

        if (tminus < 0)
        {
          alert.colour(GREEN);
          state = 2;
          flightStateTime = dt;
          break;
        }
        else
        {
          alert.launchCountdown(tminus);
        }
      }
    }
  }

  // Flight State
  else if (state == 2)
  {
    sensorUpdate();

    updatePos();

    pidCompute();

    tvc.setX(pwmX);
    tvc.setY(pwmY);

    if (((dt - flightStateTime) > 5)) // || (aa.z <= 0.2)) // If 5 seconds has passed in Flight state or acceleration up is less then 0.2g, then switch to MECO State
    {
      state = 3;
      mecoStateTime = dt;
    }

    delay(5);
  }

  // MECO State ( Main Engine Cut Off )
  else if (state == 3)
  {
    sensorUpdate();

    if (((dt - mecoStateTime) > 1.5)) // || (aa.z < -0.2)) // If 1.5 seconds has passed in MECO state or acceleration down is 0.2g, then switch to MECO State
    {
      pyro.on(1);
      delay(750);
      pyro.off();
      state = 4;
      paraStateTime = dt;
    }

    delay(5);
  }

  // Parachute Controlled Descent
  else if (state == 4)
  {
    sensorUpdate();

    if (((dt - paraStateTime) > 15)) // || altitude < 15) // If 15 seconds has passed in Parachute Descent state or Altitude is less than 15 meters, then end Flight
    {
      state = 5;
    }

    delay(5);
  }

  // End of Flight
  else if (state == 5)
  {
    alert.completeFlight(); // Beep and Blink
  }
}
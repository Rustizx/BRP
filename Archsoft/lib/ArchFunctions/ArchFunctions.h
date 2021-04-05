/* Blayone Rocket Project - ARCHFUNCTIONS ( LIBRARY FOR ARCH )
      Version 0.15
    September, 21th 2020 */

#ifndef ARCHFUNCTIONS_H
#define ARCHFUNCTIONS_H

#include <Arduino.h>
#include <Servo.h>

#define RED 0
#define GREEN 1
#define BLUE 2

#define HARDWARE_ERROR 0
#define CONFIG_ERROR 1
#define SOFTWARE_ERROR 2

#define XAXIS 0
#define YAXIS 1

#define HEADER1 23
#define HEADER2 22
#define HEADER5 17

class AlertSystem
{
private:
    byte rLEDpin;
    byte gLEDpin;
    byte bLEDpin;
    byte buzzerpin;

public:
    AlertSystem(byte rLEDpin, byte gLEDpin, byte bLEDpin, byte buzzerpin);

    void init();

    void on();
    void off();

    void colour(uint8_t color);

    void error(uint8_t code);
    void success();

    void launchCountdown(int tminus);
    void completeFlight();
};

class TVCMount
{
private:
    Servo xServo;
    Servo yServo;
    byte TVCx;
    byte TVCy;
    int xCenter;
    int yCenter;

    int xMaxPos;
    int yMaxPos;
    int xMaxNeg;
    int yMaxNeg;

    int gearRatio = 6;
    int maxMovement = 30;
    int startMovement = 20;

public:
    TVCMount(byte TVCx, byte TVCy);

    void init(float centerX, float centerY);

    void center();
    void setupMovement();

    void moveX(int angle);
    void setX(int angle);

    void moveY(int pos);
    void setY(int pos);

    int currentPosX;
    int currentPosY;
};

class Pyro
{
private:
    byte pyroChannel1;
    byte pyroChannel2;

public:
    Pyro(byte pyroChannel1, byte pyroChannel2);

    int pyro1 = 0;
    int pyro2 = 0;

    void init();

    void on(int channel);
    void off();
};

class Debug
{
private:
    int band;
    bool debugMode;

public:
    Debug(int band, bool debugMode);

    void init();

    void print(String message = "");
    void println(String message = "");
};

#endif
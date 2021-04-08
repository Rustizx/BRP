/* Blayone Rocket Project - ARCHFUNCTIONS ( LIBRARY FOR ARCH )
      Version 0.15
    September, 21th 2020 */

#include <ArchFunctions.h>

AlertSystem::AlertSystem(byte rLEDpin, byte gLEDpin, byte bLEDpin, byte buzzerLEDpin)
{
    this->rLEDpin = rLEDpin;
    this->gLEDpin = gLEDpin;
    this->bLEDpin = bLEDpin;
    this->buzzerpin = buzzerLEDpin;
    init();
}

void AlertSystem::init()
{
    pinMode(rLEDpin, OUTPUT);
    pinMode(gLEDpin, OUTPUT);
    pinMode(bLEDpin, OUTPUT);
    off();
}

void AlertSystem::on()
{
    digitalWrite(rLEDpin, LOW);
    digitalWrite(gLEDpin, LOW);
    digitalWrite(bLEDpin, LOW);
}

void AlertSystem::off()
{
    digitalWrite(rLEDpin, HIGH);
    digitalWrite(gLEDpin, HIGH);
    digitalWrite(bLEDpin, HIGH);
}

void AlertSystem::colour(uint8_t color)
{
    if (color == RED)
    {
        digitalWrite(rLEDpin, LOW);
        digitalWrite(gLEDpin, HIGH);
        digitalWrite(bLEDpin, HIGH);
    }
    else if (color == GREEN)
    {
        digitalWrite(rLEDpin, HIGH);
        digitalWrite(gLEDpin, LOW);
        digitalWrite(bLEDpin, HIGH);
    }
    else if (color == BLUE)
    {
        digitalWrite(rLEDpin, HIGH);
        digitalWrite(gLEDpin, HIGH);
        digitalWrite(bLEDpin, LOW);
    }
}

void AlertSystem::error(uint8_t code)
{
    if (code == HARDWARE_ERROR)
    { // Sensor or Microcontroller Error, Not a good error to get.   THREE BEEPS
        colour(RED);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(100);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(100);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(1000);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(100);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(100);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(1000);
        off();
    }
    else if (code == SOFTWARE_ERROR)
    { // If the wrong version if loaded, or a configuration error.    TWO BEEPS
        colour(RED);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(1000);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(1000);
        off();
    }
    else if (code == CONFIG_ERROR)
    { // If the config file is not readable.     ONE BEEP
        colour(RED);
        tone(buzzerpin, 500);
        delay(1000);
        tone(buzzerpin, 500);
        delay(1000);
        off();
    }
}

void AlertSystem::success()
{
    colour(GREEN);
    tone(buzzerpin, 2000);
    delay(100);
    noTone(buzzerpin);
    delay(100);
    tone(buzzerpin, 2000);
    delay(100);
    noTone(buzzerpin);
    delay(1000);
    off();
}

void AlertSystem::launchCountdown(int tminus)
{
    if (tminus > 5)
    {
        this->colour(RED);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(100);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(100);
        off();
        delay(600);
    }
    else if (tminus <= 5)
    {
        this->colour(GREEN);
        tone(buzzerpin, 500);
        delay(20);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(100);
        tone(buzzerpin, 500);
        delay(20);
        tone(buzzerpin, 500);
        delay(100);
        noTone(buzzerpin);
        delay(60);
        off();
        delay(600);
    }
}

void AlertSystem::completeFlight()
{
    colour(RED);
    tone(buzzerpin, 500);
    delay(100);
    noTone(buzzerpin);
    delay(100);
    colour(GREEN);
    tone(buzzerpin, 500);
    delay(100);
    noTone(buzzerpin);
    delay(100);
    colour(RED);
    tone(buzzerpin, 500);
    delay(100);
    noTone(buzzerpin);
    delay(1000);
    colour(GREEN);
    tone(buzzerpin, 500);
    delay(100);
    noTone(buzzerpin);
    delay(100);
    colour(RED);
    tone(buzzerpin, 500);
    delay(100);
    noTone(buzzerpin);
    delay(100);
    colour(GREEN);
    tone(buzzerpin, 500);
    delay(100);
    noTone(buzzerpin);
    delay(1000);
    off();
}

TVCMount::TVCMount(byte TVCx, byte TVCy)
{
    this->TVCx = TVCx;
    this->TVCy = TVCy;
}

void TVCMount::init(float centerX, float centerY)
{
    this->xCenter = centerX;
    this->yCenter = centerY;
    xMaxPos = centerX + maxMovement;
    xMaxNeg = centerX - maxMovement;
    yMaxPos = centerY + maxMovement;
    yMaxNeg = centerY - maxMovement;
    xServo.attach(TVCx);
    yServo.attach(TVCy);
    center();
}

void TVCMount::center()
{
    xServo.write(xCenter);
    yServo.write(yCenter);
}

void TVCMount::moveX(int angle)
{
    currentPosX = xCenter + (angle * gearRatio);
    if (currentPosX > xMaxPos)
    {
        currentPosX = xMaxPos;
        xServo.write(currentPosX);
    }
    else if (currentPosX < xMaxNeg)
    {
        currentPosX = xMaxNeg;
        xServo.write(currentPosX);
    }
    else
    {
        xServo.write(currentPosX);
    }
}

void TVCMount::moveY(int angle)
{
    currentPosY = yCenter + (angle * gearRatio);
    if (currentPosY > yMaxPos)
    {
        currentPosY = yMaxPos;
        yServo.write(currentPosY);
    }
    else if (currentPosY < yMaxNeg)
    {
        currentPosY = yMaxNeg;
        yServo.write(currentPosY);
    }
    else
    {
        yServo.write(currentPosY);
    }
}

void TVCMount::setX(int pos)
{
    currentPosX = pos;
    if (currentPosX > xMaxPos)
    {
        currentPosX = xMaxPos;
        xServo.write(currentPosX);
    }
    else if (currentPosX < xMaxNeg)
    {
        currentPosX = xMaxNeg;
        xServo.write(currentPosX);
    }
    else
    {
        xServo.write(currentPosX);
    }
}

void TVCMount::setY(int pos)
{
    currentPosY = pos;
    if (currentPosY > yMaxPos)
    {
        currentPosY = yMaxPos;
        yServo.write(currentPosY);
    }
    else if (currentPosY < yMaxNeg)
    {
        currentPosY = yMaxNeg;
        yServo.write(currentPosY);
    }
    else
    {
        yServo.write(currentPosY);
    }
}

void TVCMount::setupMovement()
{
    setX(startMovement + xCenter);
    delay(200);
    setY(startMovement + yCenter);
    delay(200);
    center();
    delay(200);
    setX(startMovement - xCenter);
    delay(200);
    setY(startMovement - yCenter);
    delay(200);
    center();
}

Pyro::Pyro(byte PyroChannel1, byte PyroChannel2)
{
    this->pyroChannel1 = PyroChannel1;
    this->pyroChannel2 = PyroChannel2;
    init();
}

void Pyro::init()
{
    pinMode(pyroChannel1, OUTPUT);
    pinMode(pyroChannel2, OUTPUT);
    off();
}

void Pyro::off()
{
    digitalWrite(pyroChannel1, LOW);
    digitalWrite(pyroChannel2, LOW);
    pyro1 = 0;
    pyro2 = 0;
}

void Pyro::on(int channel)
{
    if (channel == 1)
    {
        digitalWrite(pyroChannel1, HIGH);
        pyro1 = 1;
    }
    else if (channel == 2)
    {
        digitalWrite(pyroChannel2, HIGH);
        pyro2 = 1;
    }
}

Debug::Debug(int band, bool debugMode)
{
    this->band = band;
    this->debugMode = debugMode;
    init();
}

void Debug::init()
{
    if (debugMode)
    {
        Serial.begin(band);
    }
}

void Debug::print(String message)
{
    if (debugMode)
    {
        Serial.print(message);
    }
}

void Debug::println(String message)
{
    if (debugMode)
    {
        Serial.println(message);
    }
}
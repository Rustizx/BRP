#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SPI.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

volatile boolean received;
volatile byte Slavereceived, Slavesend;
bool launch = false;
float time, timeAtLaunch, tminus;
float channel;
int x;

ISR(SPI_STC_vect)
{
    Slavereceived = SPDR;
    received = true;
}

void setup()
{
    lcd.begin(16, 2);
    lcd.print("Starting Up!");

    pinMode(MISO, OUTPUT);

    SPCR |= _BV(SPE);
    received = false;

    SPI.attachInterrupt();
}

void loop()
{
    time = (millis() / 1000);
    lcd.clear();
    if (!launch)
    {
        if (received) //Logic to SET LED ON OR OFF depending upon the value recerived from master
        {
            if (Slavereceived == 1)
            {
                lcd.print("Dash Pingged");
            }
            else if (Slavereceived == 2)
            {
                lcd.print("Dash Comfirmed");
                lcd.setCursor(0, 1);
                lcd.print("LaunchSequence");
                launch = true;
                timeAtLaunch = time + 1;
            }
            else
            {
                lcd.print("Unknown");
            }

            channel = pulseIn(6, HIGH);

            if (channel > 1800) // Launch Detected
            {
                x = 1;
            }
            else
            {
                x = 0;
            }

            Slavesend = x;
            SPDR = Slavesend;
            delay(1000);
        }
    }
    else
    {
        tminus = 15 - (time - timeAtLaunch);
        lcd.print("LaunchSequence");
        lcd.setCursor(0, 1);
        if (tminus < 0)
        {
            lcd.print(tminus);
        }
        else
        {
            lcd.print("Launched!!");
        }
        delay(1000);
    }
}
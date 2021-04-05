#ifndef ARCHSENSORS_H
#define ARCHSENSORS_H

#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <MPU6050.h>

class Baro {
    private:
        Adafruit_BMP280 bmp;

        float startAltitude;

    public:
        Baro();

        void init();

        float temp();
        float pressure();
        float realAltitude();

        float altitude();

        bool initSuccessful;
};

class MPU {
    private:
        unsigned long timer = 0;
        float timeStep = 0.01;

    public:
        MPU();

        MPU6050 sensor;

        void init();
        void orrient();

        bool initSuccessful;

        float pitch = 0;
        float roll = 0;
        float yaw = 0;

        struct mpuData {
            float AccX, AccY, AccZ;
            float GyroX, GyroY, GyroZ;
            float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
            float roll, pitch, yaw;
            float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
            float elapsedTime, currentTime, previousTime;
        };
        mpuData data;
        

};

#endif
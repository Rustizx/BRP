#include <ArchSensors.h>

Baro::Baro() {
    this->initSuccessful = false;
    init();
}

void Baro::init() {
    if (!bmp.begin()) {
        initSuccessful = false;
    } else {
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
        initSuccessful = true;
    }
    startAltitude = realAltitude();
}

float Baro::temp() {
    return bmp.readTemperature();
}

float Baro::pressure() {
    return bmp.readPressure();
}

float Baro::realAltitude() {
    return bmp.readAltitude();
}

float Baro::altitude() {
    return bmp.readAltitude() - startAltitude;
}

MPU::MPU() {
    init();
}

void MPU::init() {
    if(!sensor.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
        initSuccessful = false;
    } else {
        initSuccessful = true;
    }

    sensor.calibrateGyro();
}

void MPU::orrient() {
    timer = millis();

    // Read normalized values
    Vector norm = sensor.readNormalizeGyro();

    // Calculate Pitch, Roll and Yaw
    pitch = pitch + norm.YAxis * timeStep;
    roll = roll + norm.XAxis * timeStep;
    yaw = yaw + norm.ZAxis * timeStep;
}
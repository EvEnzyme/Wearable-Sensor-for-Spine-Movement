#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define TCAADDR 0x70
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 sensor1 = Adafruit_BNO055();
Adafruit_BNO055 sensor2 = Adafruit_BNO055();

// for calibration
uint8_t sys, gyro_cal, accel_cal, mg = 0;

#define printAxisData(sAxis) \
    Serial.print(" "); \
    Serial.print(sAxis.x()); \
    Serial.print(" "); \
    Serial.print(sAxis.y()); \
    Serial.print(" "); \
    Serial.println(sAxis.z()) \

void getData(Adafruit_BNO055 &sensor, uint8_t accel_cal, uint8_t gyro_cal, uint8_t mag_cal);

void setup() {
    while (!Serial);
    delay(1000);

    Wire.begin();
    
    Serial.begin(115200);
    Serial.println("\nTCAScanner ready!");
    
    for (uint8_t t=0; t<8; t++) {
        tcaselect(t);
        Serial.print("TCA Port #"); Serial.println(t);

        for (uint8_t addr = 0; addr<=127; addr++) {
            if (addr == TCAADDR) continue;

            Wire.beginTransmission(addr);
            if (!Wire.endTransmission()) {
                Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
            }
        }
    }
    Serial.println("\ndone");

    // initialize the 1st sensor
    tcaselect(1);
    if (!sensor1.begin()) {
        Serial.println("No sensor detected at multiplexer port1");
        while(1);
    }

    // initialize the 2nd sensor
    tcaselect(2);
    if (!sensor2.begin()) {
        Serial.println("No sensor detected at multiplexer port2");
        while(1);
    }

    int8_t temp = sensor1.getTemp();
    sensor1.setExtCrystalUse(true);
}

void loop() {
    tcaselect(1);
    Serial.println("sensor1");
    sensor1.getCalibration(&sys, &gyro_cal, &accel_cal, &mg);
    // Serial.println();
    getData(sensor1, accel_cal, gyro_cal, mg);

    Serial.println("sensor2");
    tcaselect(2);
    sensor2.getCalibration(&sys, &gyro_cal, &accel_cal, &mg);
    // Serial.println();
    getData(sensor2, accel_cal, gyro_cal, mg);

    delay(100);
}

void tcaselect(uint8_t i) {
    if (i > 7) return;
    
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

void getData(Adafruit_BNO055 &sensor, uint8_t accel_cal, uint8_t gyro_cal, uint8_t mag_cal) {
    imu::Vector<3> acc = sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = sensor.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = sensor.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    printAxisData(acc);
    printAxisData(gyro);
    printAxisData(mag);

    Serial.print(" ");
    Serial.print(accel_cal);
    Serial.print(" ");
    Serial.print(gyro_cal);
    Serial.print(" ");
    Serial.print(mag_cal);
    Serial.print(" ");
    Serial.println(sys);
}

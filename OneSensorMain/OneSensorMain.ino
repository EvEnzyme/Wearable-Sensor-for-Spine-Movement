#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>

// Constants
#define BNO055_SAMPLERATE_DELAY_MS (100)  // 100ms delay between sensor readings

// Sensor object
Adafruit_BNO055 bno055 = Adafruit_BNO055();

void setup() {
  Serial.begin(115200);   // Start serial communication at 115200 baud
  bno055.begin();         // Initialize the BNO055 sensor
  delay(1000);            // Delay for sensor startup
  bno055.setExtCrystalUse(true);  // Use external crystal for better precision
}

void loop() {
  // initiate event instances
  sensors_event_t event;
  bno055.getEvent(&event);

  // sensor's order of axes --> python code order for accurate processing
  // roll
  Serial.print((float)event.orientation.z);
  Serial.print(",");
  // pitch
  Serial.print((float)event.orientation.y);
  Serial.print(",");
  // yaw
  Serial.println((float)event.orientation.x);

  // Delay to match the sensor's sampling rate
  delay(BNO055_SAMPLERATE_DELAY_MS);
}

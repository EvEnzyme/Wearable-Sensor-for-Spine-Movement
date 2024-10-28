#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>

// Constants
#define BNO055_SAMPLERATE_DELAY_MS (100)  // 100ms delay between sensor readings


// sensor_object declaration: Adafruit_BNO055(int32_t sensorID = -1, uint8_t address = BNO055_ADDRESS_A, TwoWire *theWire = &Wire);
Adafruit_BNO055 sensor1 = Adafruit_BNO055(55, 0x28);
Adafruit_BNO055 sensor2 = Adafruit_BNO055(56, 0x29);




void setup() {
  Serial.begin(115200);   // Start serial communication at 115200 baud
  sensor1.begin();         // Initialize the BNO055 sensor
  sensor2.begin();
  delay(1000);            // Delay for sensor startup
  sensor1.setExtCrystalUse(true);  // Use external crystal for better precision
  sensor2.setExtCrystalUse(true);  // Use external crystal for better precision

}

void loop() {
  // initiate event instances
  sensors_event_t event1;
  sensors_event_t event2;

  sensor1.getEvent(&event1);
  sensor2.getEvent(&event2);

  // sensor's order of axes --> python code order for accurate processing
  // roll
  Serial.print((float)event2.orientation.z);
  Serial.print(",");
  // pitch
  Serial.print((float)event2.orientation.y);
  Serial.print(",");
  // yaw
  Serial.println((float)event2.orientation.x);

  // Delay to match the sensor's sampling rate
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
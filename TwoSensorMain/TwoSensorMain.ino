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

// initialising an array to store all angle values
float sensor_angles[2][3] = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
// initialize values to what the safe ranges are in {x, y, z}
float safe_angle_ranges[3] = {8.0, 7.5, 9.0};

// initialise functions
void updateSensorAngles(int sensorNum, const sensors_event_t& event);
void checkRange(float angles[2][3], float safe_ranges[3]);

// setup
void setup() {
  Serial.begin(115200);   // Start serial communication at 115200 baud
  sensor1.begin();         // Initialize the BNO055 sensors
  sensor2.begin();
  delay(1000);            // Delay for sensor startup
  sensor1.setExtCrystalUse(true);  // Use external crystal for better precision
  sensor2.setExtCrystalUse(true);  // Use external crystal for better precision

}

// loop until stopped
void loop() {
  // initiate event instances
  sensors_event_t event1;
  sensors_event_t event2;
  sensors_event_t events[2] = {event1, event2};
  
  // get position values per loop
  sensor1.getEvent(&event1);
  sensor2.getEvent(&event2);

  // get angle values (converted if on the verge of 0/360 for proper calculations)
  for (int i = 0; i < 2; i++) {
    updateSensorAngles(i, events[i]);
    angleConversion(sensor_angles[i]);
  }

  // displaying angle values
  printValues();

  // check if angle range is too high
  checkRange(sensor_angles, safe_angle_ranges);

  // sensor's order of axes --> order for accurate processing (FOR PYTHON VISUALIZATION)
  // roll
  // Serial.print((float)event2.orientation.z);
  // Serial.print(",");
  // // pitch
  // Serial.print((float)event2.orientation.y);
  // Serial.print(",");
  // // yaw
  // Serial.println((float)event2.orientation.x);

  // Delay to match the sensor's sampling rate
  delay(BNO055_SAMPLERATE_DELAY_MS);
}

// update the angles each loop
void updateSensorAngles(int sensorNum, const sensors_event_t& event) {
  sensor_angles[sensorNum][0] = (float)event.orientation.x;
  sensor_angles[sensorNum][1] = (float)event.orientation.y;
  sensor_angles[sensorNum][2] = (float)event.orientation.z;
}

// convert edge angles (around 0/360) to account for both directions of movement
void angleConversion(float angles[3]) {
  for (int i = 0; i < 3; i++) {
    if (angles[i] > 180) {
      angles[i] = 360 - angles[i];
    }
  }
}

// print in a standard format
void printValues() {
Serial.print("Sensor 1: ");
Serial.print(sensor_angles[0][0]);
Serial.print(", ");
Serial.print(sensor_angles[0][1]);
Serial.print(", ");
Serial.print(sensor_angles[0][2]);
Serial.print(" Sensor 2: ");
Serial.print(sensor_angles[1][0]);
Serial.print(", ");
Serial.print(sensor_angles[1][1]);
Serial.print(", ");
Serial.println(sensor_angles[1][2]);
}

// check if the angle range is safe
void checkRange(float angles[2][3], float safe_ranges[3]) {
  for (int i = 0; i < 3; i++) {
    if ((angles[0][i] - angles[1][i]) > safe_ranges[i]) {
      Serial.println("Safe angle range exceeded"); 
    }
  }
}

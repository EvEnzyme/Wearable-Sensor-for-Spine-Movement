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
float sensor_angles[2][3] = {[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]};
// change values to what the safe ranges are in {x, y, z}
float safe_angle_ranges[3] = {0.0, 0.0, 0.0}; // initialising the safe_angle ranges for a spinal surgery procedure

// initialise functions
void updateSensorAngles(int sensorNum, const sensors_event_t& event);
void checkRange(float angles[2][3], float safe_ranges[3])

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
  sensors_event_t events[2] = {event1, event2};

  sensor1.getEvent(&event1);
  sensor2.getEvent(&event2);

  // get angle values
  for (int i = 0; i < 2; i++) {
    updateSensorAngles(i, events[i]);
  }

  // check if angle range is too high
  checkRange(sensor_angles, safe_angle_ranges);


  // Check if angle is too large and send a warning (currently in the form of a serial msg)
  if (((float)event1.orientation.x - (float)event2.orientation.x) < safe_angle_range_x) {
    Serial.println("")
  }

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

// update the angles each loop
void updateSensorAngles(int sensorNum, const sensors_event_t& event) {
  sensor_angles[sensorNum][0] = (float)event.orientation.x;
  sensor_angles[sensorNum][1] = (float)event.orientation.y;
  sensor_angles[sensorNum][2] = (float)event.orientation.z;
}

// check if the angle range is safe
void checkRange(float angles[2][3], float safe_ranges[3]) {
  for (int i = 0; i < sizeof(safe_ranges); i++) {
    if ((angles[0][i] - angles[1][i]) > safe_ranges[i]) {
      Serial.println("Safe angle range exceeded");
    }
  }
}

//bluetooth data transfer function
void btDataTransfer(){
  //test code
  int I2C_SDA = 11;
  int I2C_SCL = 12;

  //setup code
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  /*
  or
  #define I2C_SDA 11
  #define I2C_SCL 12
  */

	int x = 6;
	Serial.print(x);

  //TODO: either send data variable by variable, or send data in an array.

  /*
  //data to send:
  - roll, pitch, yaw -> floats
  --> sample arguments (PUT INTO FUNCTION): float roll, float pitch, float yaw
  - whether or not the sensor exceeds the safe angles (xxx process on computer side)
  --> sample arguments: 

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
  */

}
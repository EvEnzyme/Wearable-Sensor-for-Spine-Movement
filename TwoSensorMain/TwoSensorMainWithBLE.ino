#include <Wire.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_BNO055.h> //downloaded
#include <utility/imumaths.h>
#include <math.h>
#include <ArduinoBLE.h> //import Arduino BLE library //downloaded
//all packages installed

// Constants
#define BNO055_SAMPLERATE_DELAY_MS (100)  // 100ms delay between sensor readings

// sensor_object declaration: Adafruit_BNO055(int32_t sensorID = -1, uint8_t address = BNO055_ADDRESS_A, TwoWire *theWire = &Wire);
Adafruit_BNO055 sensor1 = Adafruit_BNO055(55, 0x28);
Adafruit_BNO055 sensor2 = Adafruit_BNO055(56, 0x29);

// initialising an array to store all angle values
float sensor_angles[2][3] = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
// change values to what the safe ranges are in {x, y, z}
float safe_angle_ranges[3] = {0.0, 0.0, 0.0}; // initialising the safe_angle ranges for a spinal surgery procedure

// initialise functions
void updateSensorAngles(int sensorNum, const sensors_event_t& event);
void checkRange(float angles[2][3], float safe_ranges[3]);

//initialize bluetooth service and characteristic
BLEService newService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service
BLEFloatCharacteristic rollCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEFloatCharacteristic pitchCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEFloatCharacteristic yawCharacteristic("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
//const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup() {
  //Serial.begin(115200);   // Start serial communication at 115200 baud
  sensor1.begin();         // Initialize the BNO055 sensor
  sensor2.begin();
  BLE.begin();
  delay(1000);            // Delay for sensor startup
  sensor1.setExtCrystalUse(true);  // Use external crystal for better precision
  sensor2.setExtCrystalUse(true);  // Use external crystal for better precision
  // if (!BLE.begin()) {
  //   BLE.println("Starting BLE module failed!"); //potentially not working?
  //   while (1);
  // }
    // set advertised local name and service UUID:
    BLE.setLocalName("NANO_ESP32"); //name that will show up on pc
    BLE.setAdvertisedService(newService); //device broadcasts info to be discoverable by central
  
    // add the characteristic to the service
    newService.addCharacteristic(rollCharacteristic);
    newService.addCharacteristic(pitchCharacteristic);
    newService.addCharacteristic(yawCharacteristic);
  
    // add service
    BLE.addService(newService);
  
    // set the initial value for the characeristic:

    //sensorCharacteristic.writeValue(sensor_angles);
  
    // start advertising
    BLE.advertise();
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
  // checkRange(sensor_angles, safe_angle_ranges);


  // Check if angle is too large and send a warning (currently in the form of a serial msg)
  // if (((float)event1.orientation.x - (float)event2.orientation.x) < safe_angle_range_x) {
  //   Serial.println("")
  // }

  // sensor's order of axes --> python code order for accurate processing
  // roll
  // Serial.print((float)event2.orientation.z);
  // Serial.print(",");
  // // pitch
  // Serial.print((float)event2.orientation.y);
  // Serial.print(",");
  // // yaw
  // Serial.println((float)event2.orientation.x);

  //call bluetooth function
  btDataTransfer();

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
  // listen for BLE central to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
        rollCharacteristic.writeValue(sensor_angles[0][0]);
        pitchCharacteristic.writeValue(sensor_angles[0][1]);
        yawCharacteristic.writeValue(sensor_angles[0][2]);
    }
  }

}

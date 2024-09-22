#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>

// Constants
#define BNO055_SAMPLERATE_DELAY_MS (100)  // 100ms delay between sensor readings
#define GRAVITY (9.8)                     // Gravity constant
#define RAD_TO_DEG (180.0 / 3.141592654)  // Convert radians to degrees

// Variables for sensor readings
float pitchAccel, rollAccel;              // Angles from accelerometer
float filteredPitchAccel = 0, filteredRollAccel = 0;  // Low-pass filtered accelerometer angles
float prevFilteredPitchAccel = 0, prevFilteredRollAccel = 0;  // Previous filtered angles

// Variables for gyroscope integration
float integratedPitch = 0, integratedRoll = 0;
float prevTime;  // Stores previous time for calculating time delta

// Variables for magnetometer (yaw/heading)
float pitchRad, rollRad;  // Pitch and roll angles in radians
float magXCorrected, magYCorrected;  // Magnetometer corrected values
float heading;  // Yaw/heading angle

// Sensor object
Adafruit_BNO055 bno055 = Adafruit_BNO055();

void setup() {
  Serial.begin(115200);   // Start serial communication at 115200 baud
  bno055.begin();         // Initialize the BNO055 sensor
  delay(1000);            // Delay for sensor startup
  bno055.setExtCrystalUse(true);  // Use external crystal for better precision

  prevTime = millis();    // Initialize the previous time for the first loop iteration
}

void loop() {
  // Fetch calibration data
  uint8_t systemCalib, gyroCalib, accelCalib, magCalib;
  bno055.getCalibration(&systemCalib, &gyroCalib, &accelCalib, &magCalib);

  // Read sensor data
  imu::Vector<3> accel = bno055.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> gyro = bno055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> mag = bno055.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  // Calculate pitch and roll from accelerometer
  pitchAccel = -atan2(accel.x() / GRAVITY, accel.z() / GRAVITY) * RAD_TO_DEG;
  rollAccel  = -atan2(accel.y() / GRAVITY, accel.z() / GRAVITY) * RAD_TO_DEG;

  // Apply low-pass filtering to smooth accelerometer data (95% old data, 5% new data)
  filteredPitchAccel = 0.95 * prevFilteredPitchAccel + 0.05 * pitchAccel;
  filteredRollAccel  = 0.95 * prevFilteredRollAccel + 0.05 * rollAccel;

  // Calculate the time delta (dt) for integration from gyroscope
  float currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0;  // Time delta in seconds
  prevTime = currentTime;

  // Integrate gyroscope data for pitch and roll angles
  integratedPitch = (integratedPitch + gyro.y() * dt) * 0.95 + filteredPitchAccel * 0.05;
  integratedRoll  = (integratedRoll  - gyro.x() * dt) * 0.95 + filteredRollAccel * 0.05;

  // Calculate pitch and roll in radians for magnetometer corrections
  pitchRad = integratedPitch * 3.141592654 / 180.0;
  rollRad  = integratedRoll * 3.141592654 / 180.0;

  // Correct magnetometer readings using pitch and roll
  magXCorrected = mag.x() * cos(pitchRad) - mag.y() * sin(rollRad) * sin(pitchRad) + mag.z() * cos(rollRad) * sin(pitchRad);
  magYCorrected = mag.y() * cos(rollRad) + mag.z() * sin(rollRad);

  // Calculate yaw/heading angle from corrected magnetometer data
  heading = atan2(magYCorrected, magXCorrected) * RAD_TO_DEG;
  
  // Output the computed values to the serial monitor
  Serial.print(integratedRoll);
  Serial.print(",");
  Serial.print(integratedPitch);
  Serial.print(",");
  Serial.print(heading);
  Serial.print(",");
  Serial.println(systemCalib);

  // Store current filtered values for the next iteration
  prevFilteredRollAccel = filteredRollAccel;
  prevFilteredPitchAccel = filteredPitchAccel;

  // Delay to match the sensor's sampling rate
  delay(BNO055_SAMPLERATE_DELAY_MS);
}

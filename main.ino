#include "DFRobot_BNO055.h"
#include "Wire.h"

typedef DFRobot_BNO055_IIC    BNO;    // ******** use abbreviations instead of full names ********

BNO   bno1(&Wire, 0x28);    // input TwoWire interface and IIC address
BNO   bno2(&Wire, 0x29);

// show last sensor operate status, just some sanity checks
void printLastOperateStatus(BNO::eStatus_t eStatus)
{
  switch(eStatus) {
  case BNO::eStatusOK:   Serial.println("everything ok"); break;
  case BNO::eStatusErr:  Serial.println("unknow error"); break;
  case BNO::eStatusErrDeviceNotDetect:   Serial.println("device not detected"); break;
  case BNO::eStatusErrDeviceReadyTimeOut:    Serial.println("device ready time out"); break;
  case BNO::eStatusErrDeviceStatus:    Serial.println("device internal status error"); break;
  default: Serial.println("unknow status"); break;
  }
}

void setup()
{
  Serial.begin(115200);                 // set 115200 as your baud speed in "serial monitor"
  bno1.reset();
  bno2.reset();
  while(bno1.begin() != BNO::eStatusOK) {
    Serial.println("bno1 begin faild");
    printLastOperateStatus(bno1.lastOperateStatus);
    delay(2000);
  }
  Serial.println("bno1 begin success");

  while(bno2.begin() != BNO::eStatusOK) {
    Serial.println("bno2 begin faild");
    printLastOperateStatus(bno2.lastOperateStatus);
    delay(2000);
  }
  Serial.println("bno2 begin success");

}

#define printAxisData(sAxis) \
  Serial.print(" x: "); \
  Serial.print(sAxis.x); \
  Serial.print(" y: "); \
  Serial.print(sAxis.y); \
  Serial.print(" z: "); \
  Serial.println(sAxis.z)

void loop()
{
  BNO::sAxisAnalog_t   sAccAnalog1 = bno1.getAxis(BNO::eAxisAcc);    // read acceleration
  BNO::sAxisAnalog_t   sAccAnalog2 = bno2.getAxis(BNO::eAxisAcc);    // read acceleration
  BNO::sAxisAnalog_t   sMagAnalog1 = bno1.getAxis(BNO::eAxisMag); // read geomagnetic
  BNO::sAxisAnalog_t   sMagAnalog2 = bno2.getAxis(BNO::eAxisMag); // read geomagnetic
  BNO::sAxisAnalog_t   sGyrAnalog1 = bno1.getAxis(BNO::eAxisGyr); // read gyroscope
  BNO::sAxisAnalog_t   sGyrAnalog2 = bno2.getAxis(BNO::eAxisGyr); // read gyroscope
  BNO::sAxisAnalog_t   sLiaAnalog1 = bno1.getAxis(BNO::eAxisLia); // read linear acceleration
  BNO::sAxisAnalog_t   sLiaAnalog2 = bno2.getAxis(BNO::eAxisLia); // read linear acceleration
  BNO::sAxisAnalog_t   sGrvAnalog1 = bno1.getAxis(BNO::eAxisGrv); // read gravity vector
  BNO::sAxisAnalog_t   sGrvAnalog2 = bno2.getAxis(BNO::eAxisGrv); // read gravity vector
  BNO::sAxisAnalog_t   sEulAnalog1 = bno1.getEul();                  // read euler angle
  BNO::sAxisAnalog_t   sEulAnalog2 = bno2.getEul();                  // read euler angle
  BNO::sAxisAnalog_t   sQuaAnalog1 = bno1.getQua();                  // read quaternion
  BNO::sAxisAnalog_t   sQuaAnalog2 = bno2.getQua();                  // read quaternion


  
 
  Serial.println();
  Serial.println("======== analog data print start ========");
  Serial.print("sensor 1 acc analog: (unit mg)       "); printAxisData(sAccAnalog1);
  Serial.print("sensor 2 acc analog: (unit mg)       "); printAxisData(sAccAnalog2);
  Serial.print("sensor 1 mag analog: (unit ut)       "); printAxisData(sMagAnalog1);
  Serial.print("sensor 2 mag analog: (unit ut)       "); printAxisData(sMagAnalog2);
  Serial.print("sensor 1 gyr analog: (unit dps)      "); printAxisData(sGyrAnalog1);
  Serial.print("sensor 2 gyr analog: (unit dps)      "); printAxisData(sGyrAnalog2);
  Serial.print("sensor 1 lia analog: (unit mg)       "); printAxisData(sLiaAnalog1);
  Serial.print("sensor 2 lia analog: (unit mg)       "); printAxisData(sLiaAnalog2);
  Serial.print("sensor 1 grv analog: (unit mg)       "); printAxisData(sGrvAnalog1);
  Serial.print("sensor 2 grv analog: (unit mg)       "); printAxisData(sGrvAnalog2);
  Serial.print("sensor 1 eul analog: (unit degree)   "); Serial.print(" head: "); Serial.print(sEulAnalog1.head); Serial.print(" roll: "); Serial.print(sEulAnalog.roll);  Serial.print(" pitch: "); Serial.println(sEulAnalog.pitch);
  Serial.print("sensor 2 eul analog: (unit degree)   "); Serial.print(" head: "); Serial.print(sEulAnalog2.head); Serial.print(" roll: "); Serial.print(sEulAnalog.roll);  Serial.print(" pitch: "); Serial.println(sEulAnalog.pitch);
  Serial.print("sensor 1 qua analog: (no unit)       "); Serial.print(" w: "); Serial.print(sQuaAnalog1.w); printAxisData(sQuaAnalog);
  Serial.print("sensor 2 qua analog: (no unit)       "); Serial.print(" w: "); Serial.print(sQuaAnalog2.w); printAxisData(sQuaAnalog);

  Serial.println("========  analog data print end  ========");

  delay(40);
}

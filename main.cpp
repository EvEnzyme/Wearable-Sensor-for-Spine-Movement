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
  
 
  Serial.println();
  Serial.println("======== analog data print start ========");
  Serial.print("sensor 1 acc analog: (unit mg)       "); printAxisData(sAccAnalog1);
  Serial.print("sensor 2 acc analog: (unit mg)       "); printAxisData(sAccAnalog2);

  Serial.println("========  analog data print end  ========");

  delay(40);
}

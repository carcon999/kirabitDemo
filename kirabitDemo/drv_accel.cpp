
#include "arduino.h"
#include "drv_accel.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "MMA8653.h"
#include <Wire.h>

using namespace kirabit;

#define THRESHOLD_MMA8653   (7)
#define THRESHOLD_LSM303    (1)

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified lsm303 = Adafruit_LSM303_Accel_Unified(54321);
MMA8653 mma8653;

// global instance
kirabit::Accel ACCEL;

static sensors_event_t event;

// 初期化処理を行う関数
// コンストラクタは　クラス名::クラス名と同じ名前で構成します
Accel::Accel()
{
  sensor_type = eUnknown;
}

void Accel::setup(void)
{
  /* Initialise the sensor */
  if(!lsm303.begin())
  {
    mma8653.begin(false, 2, 3);

    Serial.println("MMA8653 used.");
    sensor_type = eMMA8653;
    threshold = THRESHOLD_MMA8653;
  }else{
    Serial.println("LSM303 used.");
    sensor_type = eLSM303;
    threshold = THRESHOLD_LSM303;
  }
}

int Accel::get_dir(void)
{
  int ret = 0;
  int x = 0;
  int sub = 0;
  int power = 0;

  if(sensor_type == eLSM303){
    lsm303.getEvent(&event);
    x = event.acceleration.x;
    sub = x - before_value;
    power = abs(sub);
  
  }else if(sensor_type == eMMA8653){
    mma8653.update();
    x = mma8653.getX();
    sub = before_value - x;
    power = abs(sub);
  }
    
  // ある程度の力がある場合のみ有効
  if(power > threshold){
    // 前回は、正方向だったが今回負方向に変化した場合
    if(before_sub >= 0 && sub < 0){
      before_sub = sub;
      ret = 1;
    // 前回は、負方向だったが今回正方向に変化した場合
    }else if(before_sub < 0 && sub >= 0){
      before_sub = sub;
      ret = -1;
    }
  }

  before_value = x;

  return ret;
}

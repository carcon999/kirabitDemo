#include "kirabit.h"
#include "arduino.h"
#include "MMA8653.h"
#include "drv_ledbar.h"
#include "drv_beep.h"
#include "drv_display.h"
#include "drv_key.h"
#include "mode_afterimg.h"
#include "test_image.h"

using namespace kirabit;

// global instance
kirabit::ModeAfterimg MODE_AFTERIMG;

MMA8653 accel;


ModeAfterimg::ModeAfterimg()
{
}

void ModeAfterimg::setup(void)
{
  Wire.begin();
  accel.begin(false, 2, 3);

  LEDBAR.setup(DEF_LED_NUM, DEF_LED_PIN);

  nextimage();
}

void ModeAfterimg::start(void)
{
  ModeBase::start();
}

void ModeAfterimg::stop(void)
{
}

void ModeAfterimg::imglock(bool value)
{
  DSP5x5.begin();
  DSP5x5.lock(value);
  lock = value;
  BEEP.tone( 3000,  500 );
  BEEP.notone();
  DSP5x5.end();
}

void ModeAfterimg::main_accel(void)
{
  accel.update();
  int x = accel.getX();

  // 前回値との差分を見てベクトル方向が反転したらトリガーとする。
  int sub = last_x - x;
  int power = abs(sub);

  // ある程度の力がある場合のみ有効
  if(power > 7){
    int dir = 0;
    // 前回は、正方向だったが今回負方向に変化した場合
    if(last_dir >= 0 && sub < 0){
      dir = 1;
    // 前回は、負方向だったが今回正方向に変化した場合
    }else if(last_dir < 0 && sub >= 0){
      dir = -1;
    }

    if(dir != 0)
    {
      Serial.println(dir);
      LEDBAR.draw(dir);
      BEEP.tone(1047, 25);
      last_dir = sub;

      counter++;
      if(counter >= 10){
        if(!lock){
          nextimage();
        }
        counter = 0;
      }
    }
  }

  last_x = x;  
}

void ModeAfterimg::main(void)
{
  main_accel();
}

void ModeAfterimg::nextimage(void)
{
  LEDBAR.setImage(img_table[img_index].tcolor, 16, img_table[img_index].timg, img_table[img_index].width, img_table[img_index].height);
  
  img_index++;
  if(img_index >= IMAGE_NUM){
    img_index = 0;
  }
}


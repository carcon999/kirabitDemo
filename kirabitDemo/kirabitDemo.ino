#include <Adafruit_Microbit.h>
#include "kirabit.h"
#include "drv_beep.h"
#include "drv_ledbar.h"
#include "drv_display.h"
#include "drv_key.h"
#include "mode_bright.h"
#include "mode_afterimg.h"

#define MODE_IMG (0)  // 残像表示モード
#define MODE_BR  (1)  // 明るさ調整モード

static int m_Mode = MODE_IMG;

Adafruit_Microbit microbit;

void setup() 
{
  Serial.begin(DEF_SERIAL_SPEED);
  
  Serial.println("microbit matrix demo is ready!");

  KEY.setup();

  BEEP.setup(DEF_BEEP_PIN);
  DSP5x5.setup(&microbit.matrix);

  MODE_AFTERIMG.setup();
}

static bool mbLock = false;

void loop() 
{
  // 残像表示モード
  if(m_Mode == MODE_IMG){
    MODE_AFTERIMG.main();

    if (KEY.is_long_pushed(kirabit::BTN::B))
    {
      MODE_AFTERIMG.stop();
      MODE_BRIGHT.start();
      m_Mode = MODE_BR;
    }
    else if(KEY.is_pushed(kirabit::BTN::A))
    {
      mbLock ^= true;
      MODE_AFTERIMG.imglock(mbLock);
    }
  }
  // 明るさ指定モード
  else if(m_Mode == MODE_BR){
    MODE_BRIGHT.main();

    if(MODE_BRIGHT.is_mode_change()){
      MODE_BRIGHT.stop();
      MODE_AFTERIMG.start();
      m_Mode = MODE_IMG;
    }
  }

  // キースキャン処理
  KEY.scan();
}




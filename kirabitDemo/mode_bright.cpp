
#include "arduino.h"
#include "drv_ledbar.h"
#include "drv_beep.h"
#include "drv_display.h"
#include "drv_key.h"
#include "mode_bright.h"

using namespace kirabit;

#define LEVEL_MIN  0
#define LEVEL_MAX  4
#define BRIGHT_MODE_TIMEOUT   (2000)

// global instance
kirabit::ModeBright MODE_BRIGHT;

ModeBright::ModeBright()
{
}

void ModeBright::start(void)
{
  ModeBase::start();
  Serial.println("* start Bright mode");

  BEEP.tone( 4000,  500 );
  BEEP.notone();

  img_bright = LEDBAR.getBright();
      
  // 明るさ指定モードへ
  DSP5x5.begin();
  DSP5x5.level(img_bright);

  m_tout = millis() + BRIGHT_MODE_TIMEOUT;
}

void ModeBright::stop(void)
{
  DSP5x5.end();
}

void ModeBright::main(void)
{
  if(KEY.is_pushed(kirabit::BTN::A))
  {
    BEEP.tone(4000, 20);
          
    img_bright++;
    if(img_bright > LEVEL_MAX){
      img_bright = LEVEL_MIN;
    }
    DSP5x5.level(img_bright);

    m_tout = millis() + BRIGHT_MODE_TIMEOUT;
  }
  else if(millis() > m_tout)
  {
    LEDBAR.setBright(img_bright);
    m_change = true;
  }
}

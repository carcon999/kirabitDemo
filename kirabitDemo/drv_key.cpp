
#include "arduino.h"
#include "drv_key.h"

using namespace kirabit;

// global instance
kirabit::Key KEY;

// 初期化処理を行う関数
// コンストラクタは　クラス名::クラス名と同じ名前で構成します
Key::Key()
{
  _tmout = 0;
  _status[A] = 0xFFFFFFFF;
  _status[B] = 0xFFFFFFFF;
}

void Key::setup(void)
{
  // Configure on-board buttons
  pinMode(PIN_BUTTON_A, INPUT_PULLUP);
  pinMode(PIN_BUTTON_B, INPUT_PULLUP);

  _sts[A] = KEY_IDLE;
  _sts[B] = KEY_IDLE;
}

uint8_t Key::scan(void)
{
  uint8_t ret = false;
  unsigned long ms = millis();

  // タイムアウト発生か？
  if(ms > _tmout)
  {
    _tmout = ms + 20;

    _status[A] <<= 1;
    _status[B] <<= 1;

    _status[A] |= digitalRead(PIN_BUTTON_A);
    _status[B] |= digitalRead(PIN_BUTTON_B);

    for(int i = 0 ; i < NUM ; i++)
    {
      if(_sts[i] == KEY_IDLE){
        if(_status[i] == 0){
            Serial.print(i);
            Serial.println(" KEY LONG");
          _sts[i] = KEY_LONG;
          _tmout = 0;
        }else if((_status[i] & 0x000000FF) == 0x0F){
            Serial.print(i);
            Serial.println(" KEY PUSHED");
          _sts[i] = KEY_PUSHED;
          _tmout = 0;
        }
      }else if(_sts[i] == KEY_UPWAIT){
        if(_status[i] == 0xFFFFFFFF){
            Serial.print(i);
            Serial.println(" KEY IDLE");
          _sts[i] = KEY_IDLE;
          _tmout = 0;
        }
      }else if(_sts[i] == KEY_PUSHED){
        _sts[i] = KEY_IDLE;
      }else if(_sts[i] == KEY_LONG){
        _sts[i] = KEY_UPWAIT;    
      }
    }

    ret = true;
  }
  return ret;
}

uint8_t Key::is_pushed(BTN btn)
{
  return (_sts[btn] == KEY_PUSHED);
}

uint8_t Key::is_long_pushed(BTN btn)
{
  return (_sts[btn] == KEY_LONG);
}


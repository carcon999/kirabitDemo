/*
 * drv_display.h
 * 5x5表示クラス
 * 
 * 
 */
#ifndef drv_display_h
#define drv_display_h

#include "Adafruit_Microbit.h"

namespace kirabit {

// クラスの定義
// クラス名・コンストラクタ名・関数名や使用する変数名を定義します。
class Display
{
  public:
    Display() ;
    void setup(Adafruit_Microbit_Matrix* mtx);
    void begin(void);
    void end(void);
    void print(const char*);
    void level(uint8_t level);
    void lock(bool);

  private:
    Adafruit_Microbit_Matrix* _mtx;
};

} // namespace kirabit

extern kirabit::Display DSP5x5;

#endif  // drv_display_h

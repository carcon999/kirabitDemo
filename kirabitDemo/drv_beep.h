/*
 * drv_beep.h
 * BEEP音クラス
 * 
 * http://nuneno.cocolog-nifty.com/blog/microbit/index.html
 * 
 */
#ifndef drv_beep_h
#define drv_beep_h

namespace kirabit {

// クラスの定義
// クラス名・コンストラクタ名・関数名や使用する変数名を定義します。
class Beep
{
  public:
    Beep() ;
    void setup(uint8_t pin);
    void tone(uint16_t freq, uint16_t duration);
    void notone(void);

  private:
    uint8_t _pin;
};

} // namespace kirabit

extern kirabit::Beep BEEP;

#endif  // drv_beep_h

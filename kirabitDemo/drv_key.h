/*
 * drv_key.h
 * キークラス
 * 
 */
#ifndef drv_key_h
#define drv_key_h

namespace kirabit {

typedef enum Button : uint8_t
{
  A = 0,
  B = 1,
  NUM = 2
}BTN;

typedef enum KeyStatus : uint8_t
{
  KEY_IDLE = 0,
  KEY_PUSHED = 1,
  KEY_LONG = 2,
  KEY_UPWAIT = 3
}KSTS;

// クラスの定義
// クラス名・コンストラクタ名・関数名や使用する変数名を定義します。
class Key
{
  public:
    Key() ;
    void setup(void);
    uint8_t scan(void);
    uint8_t is_pushed(BTN btn);
    uint8_t is_long_pushed(BTN btn);

  private:
    KSTS _sts[2];
    unsigned long _status[2];
    unsigned long _tmout;
};

} // namespace kirabit

extern kirabit::Key KEY;

#endif  // drv_key_h

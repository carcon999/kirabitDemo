/*
 * drv_accel.h
 * 加速度センサークラス
 * 
 * 
 */
#ifndef drv_accel_h
#define drv_accel_h

namespace kirabit {

typedef enum {
  eUnknown,
  eLSM303,
  eMMA8653,  
} SENSOR_TYPE;

// クラスの定義
// クラス名・コンストラクタ名・関数名や使用する変数名を定義します。
class Accel
{
  public:
    Accel() ;
    void setup(void);
    int get_dir(void);

  private:
    SENSOR_TYPE sensor_type;
    int before_value;
    int before_sub;
    int threshold;
};

} // namespace kirabit

extern kirabit::Accel ACCEL;

#endif  // drv_beep_h

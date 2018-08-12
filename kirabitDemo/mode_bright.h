/*
 * mode_bright.h
 * 明るさ設定モードクラス
 * 
 */
#ifndef mode_bright_h
#define mode_bright_h

#include "modebase.h"

namespace kirabit {

// クラスの定義
// クラス名・コンストラクタ名・関数名や使用する変数名を定義します。
class ModeBright : public ModeBase
{
  public:
    ModeBright();

    void start(void);
    void stop(void);
    void main(void);

  private:
    uint8_t img_bright;
};

} // namespace kirabit

extern kirabit::ModeBright MODE_BRIGHT;

#endif  // mode_bright_h

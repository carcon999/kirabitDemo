/*
 * modebase.h
 * モード基底クラス
 * 
 */
#ifndef modebase_h
#define modebase_h

namespace kirabit {

// クラスの定義
// クラス名・コンストラクタ名・関数名や使用する変数名を定義します。
class ModeBase
{
  public:
    ModeBase(){m_change = false;}

    virtual void setup(void){};
    virtual void start(void){m_change = false;};
    virtual void stop(void) = 0;
    virtual void main(void) = 0;
    virtual bool is_mode_change(void){return m_change;}

  protected:
    unsigned long m_tout;
    bool m_change;
    
};

} // namespace kirabit

#endif  // modebase_h

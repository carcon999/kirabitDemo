/*
 * drv_ledbar.h
 * LEDバークラス
 * 
 * 
 * 
 */
#ifndef drv_ledbar_h
#define drv_ledbar_h

#include <Adafruit_NeoPixel.h>



namespace kirabit {

// クラスの定義
// クラス名・コンストラクタ名・関数名や使用する変数名を定義します。
class Ledbar
{
  public:
    Ledbar();
    void setup(uint8_t numPixels, uint8_t pin, neoPixelType t=NEO_GRB + NEO_KHZ800);
    void draw(int dir);
    bool setImage(const uint32_t* pColor, uint8_t colorSize, const uint8_t* pImage, uint8_t width, uint8_t height);
    void setColorTable(int index, const uint8_t* pRGB);
    void setBright(uint8_t level);
    uint8_t getBright(void){ return _br_level;};
    void clear(void);

    void drawBuffer(void);

  private:
    void rgb_to_hsv(uint32_t color, float* H, float* S, float* V);
    void hsv_to_rgb(float H, float S, float V, uint32_t* color);

  private:
    bool _is_ble;
    uint8_t _br_level;              // 明るさレベル
    uint8_t _colorSize;
    uint8_t _width;
    uint8_t _height;
    uint16_t _bleIndex;

};

} // namespace kirabit

extern kirabit::Ledbar LEDBAR;

#endif  // drv_ledbar_h

#include "arduino.h"
#include "drv_ledbar.h"
#include "ram.h"

using namespace kirabit;

#define RGB32(r,g,b)  (((uint32_t)r << 16) | ((uint32_t)g <<  8) | b)

// global instance
kirabit::Ledbar LEDBAR;

static Adafruit_NeoPixel led = Adafruit_NeoPixel(8);

// 初期化処理を行う関数
// コンストラクタは　クラス名::クラス名と同じ名前で構成します
Ledbar::Ledbar()
{
}

void Ledbar::setColorTable(int index, const uint8_t* pRGB)
{
  _colororg[index] = RGB32(*pRGB, *(pRGB+1), *(pRGB+2));
  _color[index] = RGB32(*pRGB, *(pRGB+1), *(pRGB+2));
}

void Ledbar::clear(void)
{
  led.clear();
  led.show();
//  delay(1);
}

void Ledbar::setup(uint8_t numPixels, uint8_t pin, neoPixelType t)
{
  led.setPin(pin);
  led.updateLength(numPixels);
  led.updateType(t);
  led.begin();

  _colorSize = 0;
  _width = 0;
  _height = 0;
  _is_ble = false;
}

void Ledbar::draw(int dir)
{
  if(dir > 0)
  {
    uint8_t len = _height >> 1;

    for (int j = 0; j < _width ; j++){
      for (int i = 0; i < len; i++) {
        uint8_t val = _share._image[j][i];
        led.setPixelColor(_height - 1 - ((i << 1) + 0), _color[val >> 4]);
        led.setPixelColor(_height - 1 - ((i << 1) + 1), _color[val & 0x0F]);
      }
      led.show();
      delay(1);
    }
  }
  else
  {
    uint8_t len = _height >> 1;

    for (int j = _width - 1; j >= 0 ; j--){
      for (int i = 0; i < len; i++) {
        uint8_t val = _share._image[j][i];
        led.setPixelColor(_height - 1 - ((i << 1) + 0), _color[val >> 4]);
        led.setPixelColor(_height - 1 - ((i << 1) + 1), _color[val & 0x0F]);
      }
      led.show();
      delay(1);
    }    
  }

  clear();
}

bool Ledbar::setImage(const uint32_t* pColor, uint8_t colorSize, const uint8_t* pImage, uint8_t width, uint8_t height)
{
  bool ret = false;

  if(colorSize > COLOR_MAX){
    goto EXIT;
  }

  if(width > IMG_WIDTH || height > IMG_HEIGHT){
    goto EXIT;
  }

  memcpy(_colororg, pColor, sizeof(uint32_t)* colorSize);
  memcpy(_color, pColor, sizeof(uint32_t)* colorSize);
  
  _width = width;
  _height = height;

  // 縦横変換＋16色の4bit形式のデータ構造に変換
  for(int j = 0 ; j < _width ; j++)
  {
    for(int i = 0 ; i < _height ; i+=2)
    {
      const uint8_t* p1 = pImage + ((i + 0) * width) + j;   // 上位４ビット
      const uint8_t* p2 = pImage + ((i + 1) * width) + j;   // 下位４ビット
      uint8_t val = (*p1 << 4) | *p2 ; 
      _share._image[j][i>>1] = val;
    }
  }

  // 明るさも再設定
  setBright(_br_level);

  _is_ble = false;

  ret = true;

EXIT:;
  return ret;
}

// 明るさの変更
// 色テーブルを変更する。
void Ledbar::setBright(uint8_t level)
{
  _br_level = level;

  // オリジナル色を丸コピー
  memcpy(_color, _colororg, sizeof(uint32_t)*COLOR_MAX);

  float offset[] = {0.9f, 0.7f, 0.5f, 0.3f, 0.1f};

  for(int i = 0 ; i < COLOR_MAX ; i++)
  {
      // 色付のみ処理する。
      if(_color[i] != 0)
      {
        float H, S, V;
        rgb_to_hsv(_color[i], &H, &S, &V);
      
        // HSVのBrightnessを変更
        V -= offset[level];
        if(V < 0){
          // 0だと黒になるので多少の色がでるようにする。
          V = 0.01;
        }
    
        // HSVをRGB変換
        hsv_to_rgb(H, S, V, &_color[i]);
      }
  }
}

void Ledbar::rgb_to_hsv(uint32_t color, float* H, float* S, float* V)
{
  float max;
  float min;
  float val;
  float R, G, B;

  // 0.0 - 1.0（255分率）
  R = (float)((color >> 16) & 0x000000FF) / 255;
  G = (float)((color >> 8) & 0x000000FF) / 255;
  B = (float)(color & 0x000000FF) / 255;

  // 最大値・最小値
  if (R >= G && R >= B) {
    max = R;
    min = (G < B) ? G : B;
  } else if (G >= R && G >= B) {
    max = G;
    min = (R < B) ? R : B;
  } else {
    max = B;
    min = (R < G) ? R : G;
  }

  val = max - min;
  
  if(val == 0){
    *H = 0;
  }else{
    // Hue（色相）
    if (max == R) {
      *H = 60 * (G - B) / val;
    } else if (max == G){
      *H = 60 * (B - R) / val + 120;
    } else {
      *H = 60 * (R - G) / val + 240;
    }
    if (*H < 0.0) {
      *H += 360.0;
    }
  }

  // Saturation（彩度）
  if(max != 0){
    *S = val / max;
  }else{
    *S = 0;
  }
  // Value（明度）
  *V = max;
}

void Ledbar::hsv_to_rgb(float H, float S, float V, uint32_t* color)
{
  int Hi;
  float f, p, q, t;

  Hi = ((int)(H / 60)) % 6;
  f = H / 60 - Hi;
  p = V * (1 - S);
  q = V * (1 - f * S);
  t = V * (1 - (1 - f) * S);

  // 256階調に戻す
  V *= 255;
  p *= 255;
  q *= 255;
  t *= 255;

  uint8_t r,g,b;
  switch (Hi) {
    case 0: r = (uint8_t)V; g = (uint8_t)t; b = (uint8_t)p; break;
    case 1: r = (uint8_t)q; g = (uint8_t)V; b = (uint8_t)p; break;
    case 2: r = (uint8_t)p; g = (uint8_t)V; b = (uint8_t)t; break;
    case 3: r = (uint8_t)p; g = (uint8_t)q; b = (uint8_t)V; break;
    case 4: r = (uint8_t)t; g = (uint8_t)p; b = (uint8_t)V; break;
    case 5: r = (uint8_t)V; g = (uint8_t)p; b = (uint8_t)q; break;
  }

  *color = RGB32(r,g,b);
}




#include "arduino.h"
#include "drv_display.h"
#include "nrf.h"

using namespace kirabit;

// global instance
kirabit::Display DSP5x5;

#define LEVEL_MAX      5
const uint8_t level_bmp[LEVEL_MAX][5] ={
  {B00000,B00000,B00000,B00000,B11111}, // 0
  {B00000,B00000,B00000,B11111,B11111}, // 1
  {B00000,B00000,B11111,B11111,B11111}, // 2
  {B00000,B11111,B11111,B11111,B11111}, // 3
  {B11111,B11111,B11111,B11111,B11111}  // 4
};

#define LOCK_MAX      2
const uint8_t lock_bmp[LOCK_MAX][5] ={
  {B01110,B01000,B01000,B11111,B11111}, // 0
  {B01110,B01010,B01010,B11111,B11111}, // 1
};

// 初期化処理を行う関数
// コンストラクタは　クラス名::クラス名と同じ名前で構成します
Display::Display()
{
}

void Display::setup(Adafruit_Microbit_Matrix* mtx)
{
  _mtx = mtx;
  _mtx->setRotation(2);   // 上下反転
}

void Display::begin(void)
{
  _mtx->begin();
}

void Display::end(void)
{
  _mtx->print((char*)"");
  delay(200);

//  _mtx->end();  
}

void Display::print(const char* str)
{
  _mtx->print((char*)str);
}

void Display::level(uint8_t level)
{
  _mtx->show(&level_bmp[level][0]);
}

void Display::lock(bool value)
{
  _mtx->show(&lock_bmp[value][0]);
}


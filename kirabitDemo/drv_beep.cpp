
#include "arduino.h"
#include "drv_beep.h"
#include "nrf.h"

#define TIMERFREQ 1000000L    // タイマー基本周期
#define TONE_TIMER    NRF_TIMER1    // 利用タイマー資源
#define TONE_IRQ      TIMER1_IRQn   // 割り込み

using namespace kirabit;

// global instance
kirabit::Beep BEEP;

// 初期化処理を行う関数
// コンストラクタは　クラス名::クラス名と同じ名前で構成します
Beep::Beep()
{
}

void Beep::setup(uint8_t pin)
{
  _pin = pin;
}

//
// 音出し
// 引数
//  pin     : PWM出力ピン 
//  freq    : 出力周波数 (Hz) 15～ 50000
//  duration: 出力時間(msec)
// 
void Beep::tone(uint16_t freq, uint16_t duration)
{
  uint32_t f =TIMERFREQ/freq;
  uint32_t ulPin;
  
  // GPIOTEの設定：LEDピン・トグルタスクを定義する
  ulPin = g_ADigitalPinMap[_pin];  // TonePinの実ピン番号の取得 
  NRF_GPIOTE->CONFIG[0] =          // チャネル0に機能設定
    (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos) |            // タスクモード
    (ulPin << GPIOTE_CONFIG_PSEL_Pos) |                              // ピン番号設定
    (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos) |  // 動作指定：トグル
    (GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos);        // ピン出力初期値
  NRF_GPIOTE->POWER = 1;                                             // GPIOTE有効
    
  //タイマ設定
  TONE_TIMER->TASKS_STOP = 1;                          // タイマストップ
  TONE_TIMER->TASKS_CLEAR = 1;                         // カウンタクリア
  TONE_TIMER->MODE = TIMER_MODE_MODE_Timer;            // モード設定：タイマモード
  TONE_TIMER->PRESCALER   = 4;                         // プリスケーラ設定：16分周(1MHz)
  TONE_TIMER->BITMODE = TIMER_BITMODE_BITMODE_16Bit;   // カウンタ長設定：16ビット長指定
  TONE_TIMER->CC[0] = f/2;                             // コンパレータ0の設定(出力周波数設定)

  TONE_TIMER->SHORTS =                                 // ショートカット設定：クリアタスク指定
      (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);

  // PPIの設定(チャネル0を利用)
  //   TIMER0 コンパレータ0一致イベント と GPIOTE(ch0)LEDピン・トグルタスク を結び付ける
  NRF_PPI->CH[0].TEP  = (uint32_t)&NRF_GPIOTE->TASKS_OUT[0];       // PPI.ch0 にLEDピン・トグルタスク設定
  NRF_PPI->CH[0].EEP  = (uint32_t)&TONE_TIMER->EVENTS_COMPARE[0];  // PPI ch0 にコンパレータ0一致イベント設定  
  NRF_PPI->CHENSET   |= PPI_CHENSET_CH0_Enabled;                   // PPI ch0 有効
  
  // enable IRQ
  NVIC_EnableIRQ(TONE_IRQ);

  TONE_TIMER->TASKS_START = 1;   // タイマスタート

  if (duration) {
    delay(duration);
    TONE_TIMER->TASKS_STOP = 1;  // タイマストップ
  }
}

//
// 音の停止
// 引数
//
void Beep::notone(void)
{
  TONE_TIMER->TASKS_STOP = 1;  // タイマストップ

  // enable IRQ
  NVIC_DisableIRQ(TONE_IRQ);
}


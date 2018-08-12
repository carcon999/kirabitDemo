/*
 * kirabit.h
 * kirabit共通マクロ定義
 * 
 */
#ifndef kirabit_h
#define kirabit_h

#include "arduino.h"

namespace kirabit {

#define DEF_BEEP_PIN  (2)     // 音声出力PIN番号
#define DEF_LED_PIN   (1)     // LED出力PIN番号

#define DEF_LED_NUM   (40)    // LED個数
#define LED_RGB       (3)     // RGB数

#define COLOR_MAX     (16)    // 最大色数
#define IMG_WIDTH     (80)    // 最大幅
#define IMG_HEIGHT    (40)    // 最大高さ

#define DEF_SERIAL_SPEED 115200

} // namespace kirabit

#endif  // kirabit_h


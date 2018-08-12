/*
 * ram.h
 * メモリ領域定義
 * 
 */
#ifndef ram_h
#define ram_h

#include "kirabit.h"

namespace kirabit {

typedef union {
  uint8_t _image[IMG_WIDTH][IMG_HEIGHT>>1];   // 残像表示用の画像データ
//  uint8_t _buff[DEF_LED_NUM][LED_RGB];   // 演出用バッファ
}SHARE;

extern uint32_t _colororg[COLOR_MAX];   // オリジナル色
extern uint32_t _color[COLOR_MAX];      // 現在の色テーブル
extern SHARE _share;                    // 共有データ領域

} // namespace kirabit

#endif  // ram_h

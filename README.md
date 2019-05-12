# 概要
このソフトウェアは、micro:bitを振ると残像表示が行えるサンプルプログラムです。
プログラムを実行するいは、別途[kirakira:bit](https://www.switch-science.com/catalog/3923/)が必要です。
プログラムは、Arduino IDEを利用して作られています。

![Image](https://raw.githubusercontent.com/carcon999/kirabitDemo/master/img/cherry2.jpg)

# ビルド方法

### 1. micro:bitをArduino IDEでビルドできるようにします。
下記のサイトで説明されているような手順で、利用できるようにします。
https://qiita.com/ht_deko/items/96c20b76b101c4065187

### 2. Arduino IDEの設定は下記のように設定します。
![Image](https://raw.githubusercontent.com/carcon999/kirabitDemo/master/img/arduino.png)

### 3. Arduino IDEでライブラリを追加します。
下記のライブラリを利用しています。すべて追加してください。
* Adafruit microbit Library by Adafruit V1.1.0
* Adafruit GFX Library by Adafruit V1.2.8
* BLEPeripheral by Sandeep Mistry V0.4.0
* Adafruit NeoPixel by Adafruit V1.1.6

追加の方法は、「スケッチ」→「ライブラリをインクルード」→「ライブラリを管理」を選択し、上記を追加します。

### 4. 外部のライブラリをダウンロードします。
加速度センサー（MMA8653）のライブラリを利用します。

下記からMMA8653.hとMMA8653.cppをダウンロードし、手動でインストールします。

https://github.com/hidnseek/hidnseek/tree/master/arduino/libraries/MMA8653

インストールの方法が分からない場合は、本ソースコードと同じ階層に２ファイルをコピーします。

### 5. 外部のライブラリを一部修正します。
書き換える目的は、加速度センサーの感度を良く（変更できるように）します。
※デフォルトでは、感度が悪い（サンプリングレートが低い）ので設定で変更できるように修正します。

MMA8653.hファイル
![Image](https://raw.githubusercontent.com/carcon999/kirabitDemo/master/img/mma8653.png)

```c++:MMA8653.h
// void begin(bool highres = true, uint8_t scale = 2);
// ↓変更する。
   void begin(bool highres = true, uint8_t scale = 2, uint8_t sample = 6);

// 追加
uint8_t _sample;
```

MMA8653.cppファイル

![Image](https://raw.githubusercontent.com/carcon999/kirabitDemo/master/img/write.png)

```c++:MMA8653.cpp
void MMA8653::active()
{
// 省略
//Wire.write(reg1 | MMA_8653_CTRL_REG1_VALUE_ACTIVE | (_highres ? 0 : MMA_8653_CTRL_REG1_VALUE_F_READ) | MMA_8653_ODR_6_25);
// ↓変更する。
  Wire.write(reg1 | MMA_8653_CTRL_REG1_VALUE_ACTIVE | (_highres ? 0 : MMA_8653_CTRL_REG1_VALUE_F_READ) | _sample);
```

![Image](https://raw.githubusercontent.com/carcon999/kirabitDemo/master/img/begin.png)

```c++:MMA8653.cpp
// メソッドに引数を追加
//void MMA8653::begin(bool highres, uint8_t scale)
void MMA8653::begin(bool highres, uint8_t scale, uint8_t sample)
{
  // 下記の２行を追加
  Wire.begin();
  _sample = sample << 3;
```

### 6. ビルドし転送します。

転送し、kirakira:bitの電源をONして、左右に振ると残像で絵を見ることができます。

### 7. 残像の変更方法
ソースコードの「test_image.h」を変更することで好きな画像に変更することができます。
画像データを作成するツール（Webサイト）を作成しました。このサイトでソースコードが自動生成されます。
参考にしてください。画像は、16色に減色されます。
https://s3-ap-northeast-1.amazonaws.com/microbitble/imagecreater.html

# kirakira:bitとは？

[kirakira:bit](https://www.switch-science.com/catalog/3923/)は、micro:bitで利用できるLEDバーです。
小さなフルカラーLED（SK6805）を片面に40個搭載しています。micro:bitとの接続は、ネジで接続しますのではんだ付けを必要としません。
スイッチサイエンス様で購入することも可能です。

![Image](https://raw.githubusercontent.com/carcon999/kirabitDemo/master/img/DSC05002.JPG)

![Image](https://raw.githubusercontent.com/carcon999/kirabitDemo/master/img/DSC05014.JPG)


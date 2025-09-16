# NHK_2023_B_Manual

## Overview (概要)
- 2023Bチームの手動のプログラム群です。

## Directory (ディレクトリ構成)
```
.
├── 2023_HONROBO_Harvest(os 6.13.0, board F303K8)
├── 2023_HONROBO_Wheels (os 5.15.7, board F446RE)
├── 2023_HONROBO_main   (os 5.15.7, board F446RE)
└── README.md
```

## Environments (開発環境)
|          |     |
|   :-:    | --- |
| OS       | Windows 10 Pro, mbed-os 5.15.7, 6.13.0 |
| IDE      | Arm Keil Studio |
| HARD     | NUCLEO-F446RE, NUCLEO-F303K8 |
| LANG     | C++ |

---
<br>

## 概要
- 全てのマイコンは1本のCANバスを介して通信ができるようになっています。
- 2023_HONROBO_Harvest->収穫機構用のプログラムです。
- 2023_HONROBO_Wheels->足回り制御用のプログラムです。なぜか走行してると若干右回転するので、右側のオムニに補正をかけてあります。
- 2023_HONROBO_main->メインマイコン用のプログラムです。コントローラから受信した電文をここから他のマイコンへ送信します。


<br>

## 使用技術についてあれこれ
CANついて
- mbed-os5でCANの受信割り込みを使いたい場合は、InterfaceCAN.hとRawCAN.hを使う必要があります。ルートディレクトリに追加して使って下さい。(使い方は2023_HONROBO_Wheels/main.cpp参照)
- mbed-os6でCANの受信割り込みを使いたい場合は、特に何も追加する必要はありません。
- mbed2ではそもそもCANが使えませんでした。(リファンレスに使えるって載ってるのに...!)

超音波センサーについて
- 超音波センサーはURM37 v4.0というやつ(精度がいい)を使ってるので、その仕様に合わせて[チームメンバーが作ってくれたライブラリ](https://github.com/NITOkC-Robocon/ultrasonic-sensor-lib)を少し改変したものを使っています。

ジャイロについて
- 加速度のarc tanをとって機体の角度を割り出すと、値がぶっ飛びます。

<br>

## Author (リポジトリ作成者)
- [Kanato Nino](https://github.com/NKanato)

---


adddummy.exe

DMUソフト(*.mot)のプログラム使用領域を、0x2000バイト単位になるように
未使用部分を0xFFで埋めて mot ファイルを変換するプログラムである。

・当64bit版では、元の32bit版が抱えていた0x200区切りの手前3バイトまで
プログラムデータがあると、そのデータを上書きしてしまう
（＝ブートローダーでサムチェックエラーになる）問題を修正している。

・元の32bit版では0x200バイト区切りになるように、プログラム領域を調整していたが、
実際にブートローダー(t0026.mot/M5001.mot)が区切りとして計算しているのは、0x2000間隔であった。
この後の処理でmkinf2がPROGRAM.INFのプログラムサイズを
計算する際には0x2000の単位で行うので、実害は無い。

64bit版へexeを随時置き換えることになるので、
32bit版は上記2点の課題は修正せずそのままにしておく。


2019.02.01 S.Hatta

----------------------------------------------------------------------


CCW-Mのコンパイル用

motcnv.exe
motswap.exe


2018.06.14 M.Yonetsu

----------------------------------------------------------------------

MOTAPPND.EXE

イアン氏の motappnd は、Windows 10 64 bit で動作しなかったので
新たにコンパイルした。
Visualstudio 2015 でコンパイルした　64bit/32bit のOSで動作する。
プロジェクトは、motappnd　のフォルダー


mkinf2.exe

Visualstudio 2015 でコンパイルした　64bit/32bit のOSで動作する。
プロジェクトは、mkinf2　のフォルダー
オリジナルのソース(mkinf2.c)では、動作しなかったので修正している。



2017/08/10　M.Yonetsu

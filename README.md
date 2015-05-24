Ogg Vorbis Decoder DLL for .NET
===============================

.NETからOggVorbisデコードするDLL  
    by つーさ <http://tu3.jp/>  

## なにこれ？ / Abstract
C#などの.NET言語からOggVorbisをデコードするライブラリです。  
ストリームを渡せるので、暗号化ファイルやメモリ上のファイルなどに対応可能。  

This is an all-in-one library decodes vorbis streams  
using libogg and libvorbis for .NET languages such as C#, VB.net.  
You can retrieve PCM waveform from vorbis streams *on memory* or *packed files*.  

ゲームのBGM用にOggのデコーダが欲しいじゃない？  
libogg.dllとかlibvorbis.dllとかvorbisfile.dllとかやだし、**DLL 1個**がいいなー。  
それ以上でも以下でもない、シンプルなライブラリ。  
参照設定に追加して、お好みでWaveデコードしてください。もちろん再配布可(ライセンス見てね)    

## 使い方 / How to use?

こんな感じ。  
Like this...  

```sample.cs
var buffer = new buffer[65536];
using (var stream = new Tsukikage.Audio.OggDecodeStream(File.OpenRead("hoge.ogg")))
    while (true) {
        int read = stream.Read(buffer, 0, buffer.Length);
        if (read == 0) break;
        WaveOut.Write(buffer, 0, read);
    }
```
サンプルアプリケーションを見てください。  
Please see the SampleApplication.  

## ライセンス / Licence 

BSD like Licence. 

詳細は [COPYING.txt](https://github.com/ttsuki/OggVorbis.NET/blob/master/COPYING.txt) を見てください。  
See [COPYING.txt](https://github.com/ttsuki/OggVorbis.NET/blob/master/COPYING.txt).  

COPYINGには大まかに次のようなことが書いてありますので参考にしてください。  
To put it simply, COPYING.txt says... (in Japanese)  

- 本ソフトウェアは次の条件に沿う限り、自由に再配布を行うことができます。
- ソースでも実行形式でも、再配布の際はこの文章を配布物の中に含めてください。
- あなたの製品を普及させる目的で、勝手に私たちの名前を使わないでください。
- 本ソフトウェアは「現状のまま」で提供されます。品質の保証は一切行われません。
- いかなる場合でも、本ソフトウェアのによって発生したすべての損害について、  
本ソフトウェアの作者は一切の賠償責任を負わないものとします。



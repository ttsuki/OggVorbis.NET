#pragma once

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

typedef unsigned char byte;
#using <System.dll>
#include <errno.h>

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;

namespace Tsukikage{
namespace Audio {

///<summary>OggVorbisをデコードするStream</summary>
public ref class OggDecodeStream : Stream {

	public:
        ///<summary>OggVorbisデコーダを初期化します。</summary>
        ///<param name="oggVorbisStream">デコード対象のOggVorbis形式のデータが入ったストリーム。OggDecodeStreamが閉じられるとこのストリームも閉じられます。</param>
        OggDecodeStream(Stream^ oggVorbisStream);

		///<summary>自動ループ機能付きでOggVorbisデコーダを初期化します。</summary>
		///<param name="oggVorbisStream">デコード対象のOggVorbis形式のデータが入ったストリーム。OggDecodeStreamが閉じられるとこのストリームも閉じられます。</param>
		///<param name="loopStartSampleIndex">ループ開始位置を先頭からの"サンプル数"で指定する。この引数を付けてコンストラクタを呼び出すと、Read()メソッドが自動的にループするようになります。BGMの再生なんかには便利です。</param>
        OggDecodeStream(Stream^ oggVorbisStream, int loopStartSampleIndex);

		///<summary>自動ループ機能付きでOggVorbisデコーダを初期化します。</summary>
		///<param name="oggVorbisStream">デコード対象のOggVorbis形式のデータが入ったストリーム。OggDecodeStreamが閉じられるとこのストリームも閉じられます。</param>
		///<param name="loopStartSampleIndex">ループ開始位置を先頭からの"サンプル数"で指定する。この引数を付けてコンストラクタを呼び出すと、Read()メソッドが自動的にループするようになります。BGMの再生なんかには便利です。</param>
		///<param name="loopEndSampleIndex">ループ終了位置を先頭からの"サンプル数"で指定する。この引数を付けてコンストラクタを呼び出すと、Read()メソッドが自動的にループするようになります。BGMの再生なんかには便利です。</param>
		OggDecodeStream(Stream^ oggVorbisStream, int loopStartSampleIndex, int loopEndSampleIndex);

        ///<summary>終了処理を行い、使用されているすべてのリソースを解放します</summary>
        ~OggDecodeStream();

		///<summary>読めるはずです。trueが返ります。</summary>
        virtual property bool CanRead {
            bool get () override;
        }

		///<summary>書き込めません。falseが返ります。</summary>
        virtual property bool CanWrite {
            bool get () override;
        }

		///<summary>シークもできると思います。trueが返ります。</summary>
        virtual property bool CanSeek {
            bool get () override;
        }

        ///<summary>PCM全体の長さを"バイト単位"で取得します。</summary>
        virtual property long long Length {
            long long get () override;
        }

        ///<summary>ストリーム位置をPCMの"バイト単位"で取得・設定します。</summary>
        virtual property long long Position {
            long long get () override;
			void set (long long value) override;
        }

        ///<summary>ストリーム位置をPCMの"バイト単位"で設定します。</summary>
        virtual long long Seek (long long offset, SeekOrigin origin) override;

		///<summary>指定したバイト数分のPCMをデコードします。</summary>
        ///<param name="buffer">デコード先のバッファ</param>
        ///<param name="offset">PCMの格納開始位置を示すbuffer内のバイトオフセット</param>
        ///<param name="count">デコードする最大バイト数</param>
        ///<returns>デコードされたPCMのバイト数</returns>
        virtual int Read (array<byte>^ buffer, int offset, int count) override;

        ///<summary>対応していません</summary>
        virtual void Write (array<byte>^ buffer, int offset, int count) override;

        ///<summary>対応していません</summary>
        virtual void Flush () override;

        ///<summary>対応していません</summary>
        virtual void SetLength (long long length) override;

        ///<summary>終了処理を行い、使用されているすべてのリソースを解放します</summary>
        virtual void Close() override;
        
        ///<summary>デコードされるPCMの再生周波数</summary>
        property int SamplesPerSecond {
            int get();
        }

        ///<summary>デコードされるPCMのチャンネル数</summary>
        property int Channels {
            int get();
        }

        ///<summary>デコードされるPCMの量子化ビット数</summary>
        property int BitsPerSample {
            int get();
        }

	private:
        Stream^ baseStream; ///< Ogg Vorbis File Stream
        OggVorbis_File* pvf; ///< OggVorbis_File
        vorbis_info* v_info; ///< vorbis_info
        long long pcmStreamLength; ///< PCM換算時の長さ(バイト単位)
        bool disposed; ///< 破棄済み？

        bool autoRepeat; ///< 自動リピート
        int loopToSamples; ///< リピート開始地点
		int loopFromBytes; ///< リピート終了地点

        void init(Stream^ oggStream); ///< 初期化
        void CheckAlive(); ///< Disposedチェック

		int internalRead(byte* buffer, int offset, int count); ///< 読み込みヘルパ関数

    };
}
}
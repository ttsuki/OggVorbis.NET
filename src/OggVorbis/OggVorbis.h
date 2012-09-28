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

///<summary>OggVorbis���f�R�[�h����Stream</summary>
public ref class OggDecodeStream : Stream {

	public:
        ///<summary>OggVorbis�f�R�[�_�����������܂��B</summary>
        ///<param name="oggVorbisStream">�f�R�[�h�Ώۂ�OggVorbis�`���̃f�[�^���������X�g���[���BOggDecodeStream��������Ƃ��̃X�g���[���������܂��B</param>
        OggDecodeStream(Stream^ oggVorbisStream);

		///<summary>�������[�v�@�\�t����OggVorbis�f�R�[�_�����������܂��B</summary>
		///<param name="oggVorbisStream">�f�R�[�h�Ώۂ�OggVorbis�`���̃f�[�^���������X�g���[���BOggDecodeStream��������Ƃ��̃X�g���[���������܂��B</param>
		///<param name="loopStartSampleIndex">���[�v�J�n�ʒu��擪�����"�T���v����"�Ŏw�肷��B���̈�����t���ăR���X�g���N�^���Ăяo���ƁARead()���\�b�h�������I�Ƀ��[�v����悤�ɂȂ�܂��BBGM�̍Đ��Ȃ񂩂ɂ͕֗��ł��B</param>
        OggDecodeStream(Stream^ oggVorbisStream, int loopStartSampleIndex);

		///<summary>�������[�v�@�\�t����OggVorbis�f�R�[�_�����������܂��B</summary>
		///<param name="oggVorbisStream">�f�R�[�h�Ώۂ�OggVorbis�`���̃f�[�^���������X�g���[���BOggDecodeStream��������Ƃ��̃X�g���[���������܂��B</param>
		///<param name="loopStartSampleIndex">���[�v�J�n�ʒu��擪�����"�T���v����"�Ŏw�肷��B���̈�����t���ăR���X�g���N�^���Ăяo���ƁARead()���\�b�h�������I�Ƀ��[�v����悤�ɂȂ�܂��BBGM�̍Đ��Ȃ񂩂ɂ͕֗��ł��B</param>
		///<param name="loopEndSampleIndex">���[�v�I���ʒu��擪�����"�T���v����"�Ŏw�肷��B���̈�����t���ăR���X�g���N�^���Ăяo���ƁARead()���\�b�h�������I�Ƀ��[�v����悤�ɂȂ�܂��BBGM�̍Đ��Ȃ񂩂ɂ͕֗��ł��B</param>
		OggDecodeStream(Stream^ oggVorbisStream, int loopStartSampleIndex, int loopEndSampleIndex);

        ///<summary>�I���������s���A�g�p����Ă��邷�ׂẴ��\�[�X��������܂�</summary>
        ~OggDecodeStream();

		///<summary>�ǂ߂�͂��ł��Btrue���Ԃ�܂��B</summary>
        virtual property bool CanRead {
            bool get () override;
        }

		///<summary>�������߂܂���Bfalse���Ԃ�܂��B</summary>
        virtual property bool CanWrite {
            bool get () override;
        }

		///<summary>�V�[�N���ł���Ǝv���܂��Btrue���Ԃ�܂��B</summary>
        virtual property bool CanSeek {
            bool get () override;
        }

        ///<summary>PCM�S�̂̒�����"�o�C�g�P��"�Ŏ擾���܂��B</summary>
        virtual property long long Length {
            long long get () override;
        }

        ///<summary>�X�g���[���ʒu��PCM��"�o�C�g�P��"�Ŏ擾�E�ݒ肵�܂��B</summary>
        virtual property long long Position {
            long long get () override;
			void set (long long value) override;
        }

        ///<summary>�X�g���[���ʒu��PCM��"�o�C�g�P��"�Őݒ肵�܂��B</summary>
        virtual long long Seek (long long offset, SeekOrigin origin) override;

		///<summary>�w�肵���o�C�g������PCM���f�R�[�h���܂��B</summary>
        ///<param name="buffer">�f�R�[�h��̃o�b�t�@</param>
        ///<param name="offset">PCM�̊i�[�J�n�ʒu������buffer���̃o�C�g�I�t�Z�b�g</param>
        ///<param name="count">�f�R�[�h����ő�o�C�g��</param>
        ///<returns>�f�R�[�h���ꂽPCM�̃o�C�g��</returns>
        virtual int Read (array<byte>^ buffer, int offset, int count) override;

        ///<summary>�Ή����Ă��܂���</summary>
        virtual void Write (array<byte>^ buffer, int offset, int count) override;

        ///<summary>�Ή����Ă��܂���</summary>
        virtual void Flush () override;

        ///<summary>�Ή����Ă��܂���</summary>
        virtual void SetLength (long long length) override;

        ///<summary>�I���������s���A�g�p����Ă��邷�ׂẴ��\�[�X��������܂�</summary>
        virtual void Close() override;
        
        ///<summary>�f�R�[�h�����PCM�̍Đ����g��</summary>
        property int SamplesPerSecond {
            int get();
        }

        ///<summary>�f�R�[�h�����PCM�̃`�����l����</summary>
        property int Channels {
            int get();
        }

        ///<summary>�f�R�[�h�����PCM�̗ʎq���r�b�g��</summary>
        property int BitsPerSample {
            int get();
        }

	private:
        Stream^ baseStream; ///< Ogg Vorbis File Stream
        OggVorbis_File* pvf; ///< OggVorbis_File
        vorbis_info* v_info; ///< vorbis_info
        long long pcmStreamLength; ///< PCM���Z���̒���(�o�C�g�P��)
        bool disposed; ///< �j���ς݁H

        bool autoRepeat; ///< �������s�[�g
        int loopToSamples; ///< ���s�[�g�J�n�n�_
		int loopFromBytes; ///< ���s�[�g�I���n�_

        void init(Stream^ oggStream); ///< ������
        void CheckAlive(); ///< Disposed�`�F�b�N

		int internalRead(byte* buffer, int offset, int count); ///< �ǂݍ��݃w���p�֐�

    };
}
}
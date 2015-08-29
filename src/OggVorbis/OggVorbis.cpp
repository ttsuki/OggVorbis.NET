// これは メイン DLL ファイルです。

#include "OggVorbis.h"

namespace Tsukikage{
namespace Audio {

template<class T>
inline T min(T a, T b) { return a < b ? a : b; }

// oggvorbisストリームを読む。libvorbisfileから呼ばれるはず
static size_t read(void *ptr, size_t size, size_t nmemb, void *datasource) {
	Stream^ stream = *(static_cast<Stream^ *>(datasource));
	try {
		int forread = size * nmemb;
		array<byte> ^buffer = gcnew array<byte>(forread);
		int sz = stream->Read(buffer, 0, forread);
		Marshal::Copy(buffer, 0, (IntPtr)ptr, sz);
		return sz;
	}
	catch(...) {
		errno = -1;
	}
	return 0;
}

// oggvorbisストリームをシークする。libvorbisfileから呼ばれるはず
static int seek(void *datasource, ogg_int64_t offset, int whence) {
	Stream^ stream = *(static_cast<Stream^ *>(datasource));
	try {
		switch (whence) {
			case SEEK_SET: stream->Seek(offset, System::IO::SeekOrigin::Begin); break;
			case SEEK_END: stream->Seek(offset, System::IO::SeekOrigin::End); break;
			case SEEK_CUR: stream->Seek(offset, System::IO::SeekOrigin::Current); break;
			default: return -1;
		}
	} catch (...) {
		return -1;
	}
	return 0;
}

// oggvorbisストリームを閉じる。libvorbisfileから呼ばれるはず
static int close(void *datasource){
	Stream^ stream = *(static_cast<Stream^ *>(datasource));
	try {
		if (stream != nullptr) {
			stream->Close();
		}
		return 0;
	} catch (...) {
		return -1;
	}
}

// 現在位置を返す。libvorbisfileから呼ばれるはず
static long tell(void *datasource) {
	Stream^ stream = *(static_cast<Stream^ *>(datasource));
	return static_cast<long>(stream->Position);
}

OggDecodeStream::OggDecodeStream(Stream^ oggVorbisStream) {
	init(oggVorbisStream);
	this->autoRepeat = false;
	this->loopToSamples = 0;
	this->loopFromBytes = static_cast<int>(pcmStreamLength);
}

OggDecodeStream::OggDecodeStream(Stream^ oggVorbisStream, int loopStartSampleIndex) {
	init(oggVorbisStream);
	this->autoRepeat = true;
	this->loopToSamples = loopStartSampleIndex;
	this->loopFromBytes = static_cast<int>(pcmStreamLength);
}

OggDecodeStream::OggDecodeStream(Stream^ oggVorbisStream, int loopStartSampleIndex, int loopEndSampleIndex) {
	this->init(oggVorbisStream);
	this->autoRepeat = true;
	this->loopToSamples = loopStartSampleIndex;
	this->loopFromBytes = loopEndSampleIndex * 2 * Channels;
}

OggDecodeStream::~OggDecodeStream() {
	Close();
}

bool OggDecodeStream::CanRead::get() {
    return !disposed && true;
}

bool OggDecodeStream::CanWrite::get() {
    return false;
}

bool OggDecodeStream::CanSeek::get() {
    return !disposed && true;
}


long long OggDecodeStream::Seek (long long offset, System::IO::SeekOrigin origin) {
	CheckAlive();
	pin_ptr<Stream^> stream = &baseStream;
	pvf->datasource = stream;
	int curPos = static_cast<int>(ov_pcm_tell(pvf) * 2 * Channels);

	switch(origin) {
		case System::IO::SeekOrigin::Begin: break;
		case System::IO::SeekOrigin::Current: offset += curPos; break;
		case System::IO::SeekOrigin::End: offset = Length + offset; break;
	}

	// Byte -> Sample
	offset /= 2*Channels;

	if (ov_pcm_seek(pvf, offset))
		throw gcnew IOException("ov_pcm_seek fail.");

	pvf->datasource = 0;
	return Position;
}

int OggDecodeStream::internalRead(byte* buffer, int offset, int count) {
	int total = 0;
	while(total < count) {
		int sz = ov_read(pvf, reinterpret_cast<char*>(buffer) + offset + total, count - total, 0, 2, 1, NULL);
		if (sz == 0) break;
		if (sz < 0) throw gcnew IOException("ov_read fail ["+sz+"]");
		total += sz;
	}
	return total;
}

int OggDecodeStream::Read (array<byte>^ buffer, int offset, int count) {
	CheckAlive();

	pin_ptr<Stream^> stream = &baseStream;
	pvf->datasource = stream;

	pin_ptr<unsigned char> bufferPinned = &buffer[0];
	unsigned char *bufferDest = bufferPinned;
	
	int curPos = static_cast<int>(ov_pcm_tell(pvf) * 2 * Channels);
	if (!autoRepeat || curPos >= loopFromBytes)
		return internalRead(bufferPinned, offset, count);

	int total = 0;

	while (true) {
		int curPos = static_cast<int>(ov_pcm_tell(pvf) * 2 * Channels);
		int nextRead = min(count - total, loopFromBytes - curPos);
		int sz = internalRead(bufferPinned, offset + total, nextRead);
		total += sz;
		if (curPos + sz == loopFromBytes) ov_pcm_seek(pvf, loopToSamples);
		if (sz == 0 || total == count) break;
	}
	pvf->datasource = 0;
	return total;
}

void OggDecodeStream::Write (array<byte>^ buffer, int offset, int count) {
	throw gcnew NotSupportedException;
}

void OggDecodeStream::Flush () {
	throw gcnew NotSupportedException;
}

void OggDecodeStream::SetLength (long long length) {
	throw gcnew NotSupportedException;
}

void OggDecodeStream::Close() {
	if (disposed) return;
	disposed = true;

	if (pvf) {
		pin_ptr<Stream^> stream = &baseStream;
		pvf->datasource = stream;
		ov_clear(pvf);

		baseStream->Close();
		delete pvf;
		pvf = 0;
	}
}

long long OggDecodeStream::Length::get() {
	CheckAlive();
	return pcmStreamLength;
}

long long OggDecodeStream::Position::get() {
	CheckAlive();
	pin_ptr<Stream^> stream = &baseStream;
	pvf->datasource = stream;
	long long pos = ov_pcm_tell(pvf) * 2 * Channels;
	pvf->datasource = 0;
	return pos;

}
void OggDecodeStream::Position::set (long long value) {
	Seek(value, System::IO::SeekOrigin::Begin);
}

int OggDecodeStream::SamplesPerSecond::get() {
	CheckAlive();
	return samplePerSec;
}

int OggDecodeStream::Channels::get() {
	CheckAlive();
	return channels;
}

int OggDecodeStream::BitsPerSample::get() {
	CheckAlive();
	return 16;
}

void OggDecodeStream::init(Stream^ oggStream)
{
	disposed = false;
	baseStream = oggStream;
	pin_ptr<Stream^> ptr = &baseStream;
	pvf = new OggVorbis_File;

	ov_callbacks callbacks;
	callbacks.read_func = read;
	callbacks.seek_func = seek;
	callbacks.close_func = close;
	callbacks.tell_func = tell;

	if (ov_open_callbacks(ptr, pvf, NULL, 0, callbacks) != 0)
		throw gcnew ArgumentException("ov_open_callbacks failed. Is oggStream truly vorbis stream? - OggVorbisデコーダの初期化に失敗。入力ソースおかしいかも。");
	
	if (ov_seekable(pvf) == 0)
		throw gcnew ArgumentException("oggStream must be seekable. - シーク可能なストリームに対してのみ対応。");

	vorbis_info *v_info = ov_info(pvf, -1);
	samplePerSec = v_info->rate;
	channels = v_info->channels;
	pcmStreamLength = ov_pcm_total(pvf, -1) * 2 * channels;
	pvf->datasource = 0;
}

void OggDecodeStream::CheckAlive()
{
	if (disposed)
		throw gcnew ObjectDisposedException("Stream has been disposed. - ストリームはすでに閉じられています。");
}
}
}


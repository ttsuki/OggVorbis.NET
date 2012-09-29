Ogg Vorbis Decoder DLL for .NET
===============================

.NET����OggVorbis�f�R�[�h����DLL
    by �[�� <http://tu3.jp/>

## �Ȃɂ���H / Abstract
C#�Ȃǂ�.NET���ꂩ��OggVorbis���f�R�[�h���郉�C�u�����ł��B
�X�g���[����n����̂ŁA�Í����t�@�C���⃁������̃t�@�C���ȂǂɑΉ��\�B

This is an all-in-one library decodes vorbis streams
using libogg and libvorbis for .NET languages such as C#, VB.net.
You can retrieve PCM waveform from vorbis streams *on memory* or *packed files*.

## �g���� / How to use?

����Ȋ����B
Like this?  

var s = new Tsukikage.Audio.OggDecodeStream(File.OpenRead("hoge.ogg")); 
while (true) {  
    byte[] buffer = new buffer[65536];  
    int read = s.Read(buffer, 0, buffer.Length);  
    if (read == 0) break;
    WaveOut.Write(buffer, 0, read);  
}  


�T���v���A�v���P�[�V�������ĂˁI  
See also SampleApplication!  

## ���C�Z���X / Licence 

BSD like Licence. 

�ڍׂ� COPYING.txt �����Ă��������B
See COPYING text.  

COPYING�ɂ͑�܂��Ɏ��̂悤�Ȃ��Ƃ������Ă���܂��̂ŎQ�l�ɂ��Ă��������B  
To put it simply, COPYING text says... (in Japanese)  

- �{�\�t�g�E�F�A�͎��̏����ɉ�������A���R�ɍĔz�z���s�����Ƃ��ł��܂��B
- �\�[�X�ł����s�`���ł��A�Ĕz�z�̍ۂ͂��̕��͂�z�z���̒��Ɋ܂߂Ă��������B
- ���Ȃ��̐��i�𕁋y������ړI�ŁA����Ɏ������̖��O���g��Ȃ��ł��������B
- �{�\�t�g�E�F�A�́u����̂܂܁v�Œ񋟂���܂��B�i���̕ۏ؂͈�؍s���܂���B
- �����Ȃ�ꍇ�ł��A�{�\�t�g�E�F�A�̂ɂ���Ĕ����������ׂĂ̑��Q�ɂ��āA  
�{�\�t�g�E�F�A�̍�҂͈�؂̔����ӔC�𕉂�Ȃ����̂Ƃ��܂��B



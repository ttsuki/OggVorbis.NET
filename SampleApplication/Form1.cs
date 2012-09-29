using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Tsukikage.WinMM.WaveIO;
using Tsukikage.Audio;

namespace WaveOutSample
{
    /// <summary>
    /// WaveOutを使ったアプリケーションのサンプル
    /// </summary>
    /// <remarks>
    /// あなたのアプリケーションでDLLを使う場合、プラットフォームターゲットをx86に変更するのを忘れないでください。
    /// (C#側のプロジェクトのプロパティ→ビルド→プラットフォームターゲット)
    /// C++/CLIで作られたDLLはネイティブコードを含むため、プラットフォーム間でDLLを共有できません。
    /// Any CPUのまま.NETアプリをビルドしていると、x64として動作するEXEがx86 DLLにリンクしようとしてコケます。
    /// x64環境でもx86版アプリケーションは動作するので、EXE自体もx86としてビルドのをおすすめします。
    ///
    /// Debug ビルドのターゲットプラットフォームを変更したけど、
    /// Release ビルドのターゲットプラットフォームを変更してなかった！　ってこと、
    /// よくあるので気をつけてください(笑)
    /// </remarks>
    public partial class Form1 : Form
    {
        /// <summary>
        /// OggStream
        /// </summary>
        OggDecodeStream waveStream = null;

        /// <summary>
        /// waveOut
        /// </summary>
        WaveOut waveOut;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // WaveOut を初期化する。
            waveOut = new WaveOut(WaveOut.WaveMapper, 44100, 16, 2);

            // WaveOutの状態を気にしつつ、データを出力するためのタイマー
            timer1.Interval = 10;
            timer1.Tick += new EventHandler(delegate(object sender_, EventArgs e_) { Feed(); });

            // 自動ループ機能を使ってOggVorbisデコーダを初期化する。引数の単位は頭からのサンプル数。
            // Oggファイルをそのまま聴くとフェードアウトになってるんですが、ゲーム中は切れ目なくループ再生したいわけで。
            // ループの先頭と末尾の位置は、Wave編集ソフトとかで開いて波形を見て決める。
            // ループしたい場所のなるべく近くで、左右チャンネルとも音量がほぼ0になっているタイミングを見つけて指定。
            // ループ位置を繋いだときに波形がジャンプしているとクリックノイズがのってしまう。
            // 100サンプル程度前後しても、その程度の音飛び・リズム狂いに気づける人はほとんどいないはずなので、
            // つなぎ目の音量優先で位置を決めましょう。
            waveStream = new OggDecodeStream(File.OpenRead("SampleBGM.ogg"), 251000, 815518);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 窓を閉じる準備。止めて、閉じて、解放する。
            Stop();
            waveOut.Close();
            waveStream.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // 再生状態をトグル。
            if (IsPlaying)
            {
                Stop();
                button1.Text = "STOPPED.";
            }
            else
            {
                Play();
                button1.Text = "PLAYING...";
            }
        }

        /// <summary>
        /// 再生中か？
        /// </summary>
        private bool IsPlaying { get { return timer1.Enabled; } }

        /// <summary>
        /// 再生する。
        /// </summary>
        private void Play()
        {
            timer1.Start();
        }

        /// <summary>
        /// 停止して頭出し。
        /// </summary>
        private void Stop()
        {
            timer1.Stop();
            waveOut.Stop();
            waveStream.Seek(0, SeekOrigin.Begin);
        }

        /// <summary>
        /// WaveOutに出力済みのデータが一定以下になったらデコードして書き込む。
        /// </summary>
        private void Feed()
        {
            // 再生位置を監視するにはタイマーやスレッドを使う。
            // 44100Hz 2ch 16bps => 176400Bytes/s なので、
            // 60fps ゲーム の場合、11025Bytes/fである。
            // それよりは大きくしないと、音が途切れる。
            
            const int BLOCK_SIZE = 16384;
            const int BUFFER_SIZE = BLOCK_SIZE * 4;
            while (waveOut.EnqueuedBufferSize < BUFFER_SIZE)
            {
                byte[] buffer = new byte[BLOCK_SIZE];
                waveStream.Read(buffer, 0, BLOCK_SIZE);
                waveOut.Write(buffer);
            }
        }

    }
}

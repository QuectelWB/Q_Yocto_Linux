#音频


https://www.jianshu.com/p/1b79537da86d

https://www.cnblogs.com/jikexianfeng/p/7447570.html


## info

v@v-pc:~$ cat /dev/snd/
	
	by-path/   controlC1  hwC0D0     pcmC0D10p  pcmC0D7p   pcmC0D9p   pcmC1D0p   timer
	controlC0  controlC2  hwC1D0     pcmC0D3p   pcmC0D8p   pcmC1D0c   seq

sudo apt-get install alsa-base -y


alsamixer



/etc/asound.conf


## Audio Player


mpg123是播放mp3文件的命令行实用程序。您可以使用以下方法在Ubuntu中安装它：
	
	sudo apt-get install mpg123
	
	
	 v@v-pc:~$ mpg123 menglishuixing.mp3
	High Performance MPEG 1.0/2.0/2.5 Audio Player for Layers 1, 2 and 3
		version 1.22.4; written and copyright by Michael Hipp and others
		free software (LGPL) without any warranty but with best wishes

	Playing MPEG stream 1 of 1: menglishuixing.mp3 ...

	MPEG 1.0 layer III, 320 kbit/s, 44100 Hz joint-stereo
	Title:   08 ******** ****                Artist: ************
	Comment: **QQ:648488989 ************     Album:  ************
	Year:    2012                            Genre:  Unknown

---------------------------




	 play win-cc.ogg

	win-cc.ogg:

	 File Size: 119k      Bit Rate: 87.6k
	  Encoding: Vorbis
	  Channels: 2 @ 16-bit
	Samplerate: 44100Hz      Album: 仙剑奇侠传四
	Replaygain: off
	  Duration: 00:00:10.85  Title: 昆仑道

	In:82.1% 00:00:08.92 [00:00:01.94] Out:393k  [  -===|===-  ] Hd:1.0 Clip:0
	
	
------------------


	v@v-pc:~$ play shangbeizi.wav

	shangbeizi.wav:

	 File Size: 43.2M     Bit Rate: 1.41M
	  Encoding: Signed PCM
	  Channels: 2 @ 16-bit
	Samplerate: 44100Hz
	Replaygain: off
	  Duration: 00:04:04.88

	In:7.97% 00:00:19.50 [00:03:45.37] Out:860k  [  -===|===-  ]        Clip:0


## moc

	sudo apt-get install moc

命令行形式的 mocp

## vlc



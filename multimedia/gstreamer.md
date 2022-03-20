##一、gst-inspect-1.0 命令

查看支持的格式

##二、gst-play-1.0

1、播放视频
	
	gst-play-1.0 --videosink "fbdevsink device=/dev/fb0" /data/test.mp4 (默认使用autoaudiosink，有声音)

2、解码视频
	
	gst-play-1.0 --videosink "filesink location=/data/1.yuv" /data/3.mp4

##三、gst-launch-1.0 命令

###1、音视频播放
1）播放音频
	
	gst-launch-1.0 filesrc location=/data/test_2.mp4 ! decodebin ! audioconvert ! audioresample ! autoaudiosink
	
	gst-launch-1.0 playbin uri=file:///data/g.mp3  //使用playbin播放

设置耳机输出：
	
	gst-launch-1.0 filesrc location=/data/Hassium.ogg ! oggdemux ! vorbisdec ! audioconvert ! qahwsink output-device=wired-headphone volume=2


2）播放视频
	gst-launch-1.0 filesrc location=/data/test.mp4 ! decodebin ! videoconvert ! fbdevsink device=/dev/fb0
3）适配屏幕大小的播放
	gst-launch-1.0 filesrc location=/data/video.webm ! decodebin name=dec ! queue ! videoconvert ! videoscale ! video/x-raw,width=720,height=1280 ! fbdevsink device=/dev/fb0 dec. ! queue ! audioconvert ! audioresample ! autoaudiosink
4） 播放音视频
	gst-launch-1.0 playbin uri=file:///data/test_1.mp4 video-sink="fbdevsink device=/dev/fb0"
	gst-launch-1.0 filesrc location=/data/test.mp4 ! qtdemux name=demux demux.audio_0 ! queue ! decodebin ! audioconvert ! audioresample ! autoaudiosink demux.video_0 ! queue ! decodebin ! videoconvert ! fbdevsink device=/dev/fb0

###2、预览usb camera
	gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=640,height=480' ! videoconvert ! fbdevsink device=/dev/fb0

###3、usb camera录像
	gst-launch-1.0 v4l2src device=/dev/video3 ! 'video/x-raw,width=1280,height=720' ! videoconvert ! omxh264enc ! h264parse ! matroskamux ! filesink location=/data/mpeg.mkv

###4、mipi camera预览
1）预览
	gst-launch-1.0 quectelmipisrc device=/dev/video0 ! 'video/x-raw, width=640, height=480,format=NV12' ! videoconvert! fbdevsink device=/dev/fb0
2)旋转
	gst-launch-1.0 quectelmipisrc device=/dev/video0 ! 'video/x-raw, width=640,
height=480,format=NV12' ! videoflip method=clockwise ! videoconvert! fbdevsink device=/dev/fb0 //可以通过gst-inspect-1.0 videoflip查看该元件

###5、mipi camera录像
1）单编码：
	
	gst-launch-1.0 quectelmipisrc device=/dev/video1 ! 'video/x-raw, width=1280, height=720,format=NV21' ! videoflip method=clockwise ! videoconvert! omxh264enc target-bitrate = 512000 ! filesink location=/data/mpeg.h264

2）存储YUV
	
	gst-launch-1.0 quectelmipisrc device=/dev/video1 ! 'video/x-raw, width=1280, height=720,format=NV21' ! videoconvert! filesink location=/data/mpeg.yuv

###6、录制视频文件
1）录制MKV
	
	gst-launch-1.0 quectelmipisrc device=/dev/video1 ! 'video/x-raw, width=1280, height=720,format=NV21' ! videoflip method=clockwise ! videoconvert! omxh264enc target-bitrate = 1024000 ! h264parse ! matroskamux  ! filesink location=/data/mpeg.mkv

2）录制mp4
	
	gst-launch-1.0 -e quectelmipisrc device=/dev/video1 ! 'video/x-raw, width=1280, height=720,format=NV21' ! videoconvert! omxh264enc target-bitrate = 1024000 ! h264parse ! mp4mux track-rotate=90 ! filesink location=/data/mpeg.mp4
//注意gst-launch-1.0添加参数 -e，结束的时候发送EOS，gstqtmux中检测到eos写入moov信息，否则不能播放

###7、mipi camera转码
	
	gst-launch-1.0 quectelmipisrc device=/dev/video1 num-buffers=1800 ! 'video/x-raw,format=YV12,width=1280,height=720,framerate=30/1' ! filesink location=/data/raw.yv12


###8、录制音频
1）存储PCM

	gst-launch-1.0 qahwsrc ! audioconvert ! filesink location=audio.pcm

2）封装成WAV
	
	gst-launch-1.0 qahwsrc ! audioconvert ! wavenc ! filesink location=/data/audio.wav

###10、录制带音频的视频
	
	gst-launch-1.0 quectelmipisrc device=/dev/video0 ! 'video/x-raw, width=1280, height=720,format=NV21' ! videoconvert! omxh264enc control-rate=3 target-bitrate = 4096000 ! h264parse ! queue ! mux.video_0 qahwsrc ! audioconvert ! avenc_aac ! queue ! mux.audio_0 matroskamux name=mux ! filesink location=/data/mpeg.mkv

注：!用来分割命令，每个!里面可以代表一个管道，数据流会一个管道一个管道传输

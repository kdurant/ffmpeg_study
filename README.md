# 学习资源
- [office document](https://ffmpeg.org/documentation.html)
- [office repo](https://github.com/FFmpeg/FFmpeg)
- [ffmpeg-libav-tutorial](https://github.com/leandromoreira/ffmpeg-libav-tutorial)
- [simplest_ffmpeg_device](https://github.com/leixiaohua1020/simplest_ffmpeg_device)
- [ffmpeg_tutorial](https://github.com/loupus/ffmpeg_tutorial)
- [wlxklyh/FFMpegStudy](https://github.com/wlxklyh/FFMpegStudy)


- [FFmpeg源码整体结构体](https://zhuanlan.zhihu.com/p/143195044)
# 术语
- codec(coder-decoder), 编解码器，是编码器encoder与解码器decoder的混合体
- demuxer, 解封装, 解复用
    在一个多媒体文件中，既包括音频也包括视频，而且音频和视频都是分开进行压缩的，因为音频和视频的压缩算法不一样，既然压缩算法不一样，那么肯定解码也不一样，所以需要对音频和视频分别进行解码。虽然音频和视频是分开进行压缩的，但是为了传输过程的方便，将压缩过的音频和视频捆绑在一起进行传输。所以我们解码的第一步就是将这些绑在一起的音频和视频流分开来，也就是传说中的解复用，所以一句话，解复用这一步就是将文件中捆绑在一起的音频流和视频流分开来以方便后面分别对它们进行解码
- decode
- encode
- muxer, 封装, 复用


# 模块功能
- libavformat，format，格式封装
- libavcodec，codec，编码、解码
- libavutil，util，通用音视频工具，像素、IO、时间等工具
- libavfilter，filter，过滤器，可以用作音视频特效处理
- libavdevice，device，设备（摄像头、拾音器）
- libswscale，scale，视频图像缩放，像素格式互换
- libavresample，resample，重采样
- libswresample，也是重采样，类似图像缩放
- libpostproc，后处理，？？
![pic](https://pic4.zhimg.com/80/v2-0587c276a49b615f8845a4a8b564849b_720w.jpg)

# 常用数据类型
![](https://upload-images.jianshu.io/upload_images/2048812-961d95f534381a19.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

## AVFormatContext
描述了一个媒体文件或者媒体流的构成和基本信息

## AVIOContext
输入输出对应的结构体，用于输入输出（读写文件，RTMP协议等）

## AVStream, AVCodecParameter
音视频流对应的结构体，用于音视频编解码

## AVFrame
AVFrame中存储的是经过解码后的原始数据(RGB/YUV/PCM采样数据)。在解码中，AVFrame是解码器的输出；在编码中，AVFrame是编码器的输入

## AVPacket
AVPacket中存储的是经过编码的压缩数据(H264/HEVC/ACC等码流数据)。在解码中，AVPacket由解复用器输出到解码器；在编码中，AVPacket由编码器输出到复用器



# 视频解码流程
![flow](https://raw.githubusercontent.com/leandromoreira/ffmpeg-libav-tutorial/master/img/decoding.png)

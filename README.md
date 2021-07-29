# 学习资源
- [office document](https://ffmpeg.org/documentation.html)
- [office repo](https://github.com/FFmpeg/FFmpeg)
- [ffmpeg-libav-tutorial](https://github.com/leandromoreira/ffmpeg-libav-tutorial)
- [simplest_ffmpeg_device](https://github.com/leixiaohua1020/simplest_ffmpeg_device)

# 术语
- codec, 编解码器，是编码器encoder与解码器decoder的混合体
- demuxer, 解封装, 解复用
    在一个多媒体文件中，既包括音频也包括视频，而且音频和视频都是分开进行压缩的，因为音频和视频的压缩算法不一样，既然压缩算法不一样，那么肯定解码也不一样，所以需要对音频和视频分别进行解码。虽然音频和视频是分开进行压缩的，但是为了传输过程的方便，将压缩过的音频和视频捆绑在一起进行传输。所以我们解码的第一步就是将这些绑在一起的音频和视频流分开来，也就是传说中的解复用，所以一句话，解复用这一步就是将文件中捆绑在一起的音频流和视频流分开来以方便后面分别对它们进行解码
- decode
- encode
- muxer, 封装, 复用

# ffmpeg常用数据类型

## AVFrame
AVFrame中存储的是经过解码后的原始数据。在解码中，AVFrame是解码器的输出；在编码中，AVFrame是编码器的输入

## AVPacket
AVPacket中存储的是经过编码的压缩数据。在解码中，AVPacket由解复用器输出到解码器；在编码中，AVPacket由编码器输出到复用器



// 输出视频流信息
#include <libavcodec/codec.h>
#include <libavutil/avutil.h>
#include <libavutil/log.h>
#include <stdio.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <stdbool.h>

int main(void)
{
    AVFormatContext *fmt_ctx   = NULL;
    AVCodecContext * codec_ctx = NULL;
    AVCodec *        codec     = NULL;

    int ret = 0;
    //    char *filename        = "../small_bunny_1080p_60fps.mp4";
    char *filename           = "../Wildlife.wmv";
    int   video_stream_index = -1;

    if((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "cannot open input file\n");
        goto end;
    }

    if((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "cannot get  stream information\n");
        goto end;
    }

    for(int i = 0; i < fmt_ctx->nb_streams; i++)
    {
        if(fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
            break;
        }
    }

    if(video_stream_index < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "No video stream\n");
        goto end;
    }

    av_dump_format(fmt_ctx, video_stream_index, filename, false);

    codec_ctx = avcodec_alloc_context3(NULL);

    if((ret = avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[video_stream_index]->codecpar)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Cannot get codec parameters\n");
        goto end;
    }

    codec = avcodec_find_decoder(codec_ctx->codec_id);
    if(codec == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "No decoder found\n");
        goto end;
    }

    if((ret = avcodec_open2(codec_ctx, codec, NULL)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
        goto end;
    }

    printf("\nDecoding codec is : %s\n", codec->name);

end:
    if(codec_ctx)
        avcodec_close(codec_ctx);
    if(fmt_ctx)
        avformat_close_input(&fmt_ctx);

    return 0;
}

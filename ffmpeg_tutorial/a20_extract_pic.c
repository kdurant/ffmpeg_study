#include <libavcodec/codec.h>
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <libavutil/pixfmt.h>
#include <libavutil/imgutils.h>
#include <libavutil/log.h>
#include <libswscale/swscale.h>

#include <stdint.h>
#include <stdio.h>

void SaveFrame(AVFrame *pFrame, int width, int height, int index)
{
    FILE *pFile;
    char  szFileName[32];
    int   y;

    sprintf(szFileName, "frame%d.ppm", index);
    pFile = fopen(szFileName, "wb");
    if(pFile == NULL)
        return;

    fprintf(pFile, "P6%d %d 255", width, height);

    for(y = 0; y < height; y++)
    {
        fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);
    }
    fclose(pFile);
}

int main(void)
{
    char *           in_filename = "../small_bunny_1080p_60fps.mp4";
    AVFormatContext *pFormatCtx;
    AVCodecContext * pCodecCtx;
    AVCodec *        pCodec;
    AVFrame *        pFrame;
    AVFrame *        pFrameRGB;
    AVPacket *       packet;
    uint8_t *        out_buffer;

    static struct SwsContext *img_convert_ctx;

    int videoStream, i, numBytes;
    int ret, got_picture;

    pFormatCtx = avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx, in_filename, NULL, NULL) != 0)
    {
        av_log(NULL, AV_LOG_ERROR, "can't open the file.");
        return -1;
    }

    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Can't find stream information.");
        return -1;
    }

    videoStream = -1;

    for(i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            videoStream = i;
        break;
    }
    if(videoStream == -1)
    {
        av_log(NULL, AV_LOG_ERROR, "Can't find a video stream.");
        return -1;
    }

    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    pCodec    = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "Can't find codec");
        return -1;
    }

    if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Can't open codec.");
        return -1;
    }

    pFrame    = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

    // old api: avpicture_get_size()
    numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height, 1);

    out_buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

    return 0;
}

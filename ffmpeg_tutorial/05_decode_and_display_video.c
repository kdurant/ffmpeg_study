#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <libavcodec/codec.h>
#include <libavcodec/packet.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavutil/log.h>
#include <stdio.h>
#include <stdbool.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <SDL.h>
#undef main

SDL_Renderer *renderer;
SDL_Texture * texture;
SDL_Rect      r;

void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize, FILE *f)
{
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

    for(int i = 0; i < ysize; i++)
    {
        fwrite(buf + i * wrap, 1, xsize, f);
    }
}

void displayFrame(AVFrame *frame, AVCodecContext *dec_ctx)
{
    SDL_UpdateYUVTexture(texture, &r, frame->data[0], frame->linesize[0], frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt, FILE *f)
{
    int ret;
    ret = avcodec_send_packet(dec_ctx, pkt);
    if(ret < 0)
    {
        fprintf(stderr, "Error sending a packet for decoding\n");
        return;
    }

    while(ret >= 0)
    {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return;
        }
        else if(ret < 0)
        {
            fprintf(stderr, "Error during decoding\n");
            return;
        }

        printf("saving frame %3d\n", dec_ctx->frame_number);
        fflush(stdout);
        pgm_save(frame->data[0], frame->linesize[0], frame->width, frame->height, f);

        displayFrame(frame, dec_ctx);
    }
}

int initSDL(AVCodecContext *codec_ctx)
{
    SDL_Window *window = NULL;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "could not init sdl %s\n", SDL_GetError());
        return -1;
    }
    window = SDL_CreateWindow("Preview", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, codec_ctx->width, codec_ctx->coded_height, 0);
    if(!window)
    {
        fprintf(stderr, "could not create sdl window \n");
        return -1;
    }

    r.x = 0;
    r.y = 0;
    r.w = codec_ctx->width;
    r.h = codec_ctx->height;

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
    {
        fprintf(stderr, "could not create sdl renderer \n");
        return -1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, codec_ctx->width, codec_ctx->height);
    if(!texture)
    {
        fprintf(stderr, "could not create sdl texture \n");
        return -1;
    }

    return 0;
}

int main(void)
{
    AVFormatContext *fmt_ctx   = NULL;
    AVCodecContext * codec_ctx = NULL;
    AVCodec *        codec     = NULL;

    int   ret;
    char *in_filename        = "../Wildlife.wmv";
    char *out_filename       = "../output.yuv";
    int   video_stream_index = -1;

    FILE *fin  = NULL;
    FILE *fout = NULL;

    AVFrame * frame = NULL;
    AVPacket *pkt   = NULL;

    if((ret = avformat_open_input(&fmt_ctx, in_filename, NULL, NULL)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "cannot open input file\n");
        goto end;
    }

    if((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "cannot get stream info\n");
        goto end;
    }

    // get video stream index
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

    av_dump_format(fmt_ctx, video_stream_index, in_filename, false);

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

    pkt = av_packet_alloc();
    // av_init_packet(pkt);
    if(!pkt)
    {
        av_log(NULL, AV_LOG_ERROR, "Cannot init packet\n");
        goto end;
    }

    frame = av_frame_alloc();
    if(!frame)
    {
        av_log(NULL, AV_LOG_ERROR, "Cannot init frame\n");
        goto end;
    }

    fin = fopen(in_filename, "rb");
    if(!fin)
    {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        goto end;
    }

    // open output file
    fout = fopen(out_filename, "w");
    if(!fout)
    {
        av_log(NULL, AV_LOG_ERROR, "Cannot open output file\n");
        goto end;
    }

    if(initSDL(codec_ctx) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "init sdl failed\n");
        goto end;
    }

    while(1)
    {
        if((ret = av_read_frame(fmt_ctx, pkt)) < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "cannot read frame");
            break;
        }

        if(pkt->stream_index == video_stream_index)
        {
            decode(codec_ctx, frame, pkt, fout);
        }
        av_packet_unref(pkt);
    }

    decode(codec_ctx, frame, NULL, fout);
end:
    if(fin)
        fclose(fin);
    if(fout)
        fclose(fout);
    if(codec_ctx)
        avcodec_close(codec_ctx);
    if(fmt_ctx)
        avformat_close_input(&fmt_ctx);
    if(frame)
        av_frame_free(&frame);
    if(pkt)
        av_packet_free(&pkt);

    return 0;
}

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"

static void logging(const char *fmt, ...);
static int  decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame);
static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);

int main(int argc, const char *argv[])
{
    if(argc < 2)
    {
        printf("You need to sepcify a media file.\n");
        return -1;
    }

    logging("initializing all th containers, codecs and protocols.");
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if(!pFormatContext)
    {
        logging("ERROR could not allocate memory for Format Context");
        return -1;
    }
    logging("opening the input file (%s) and loading format (container) header", argv[1]);
    if(avformat_open_input(&pFormatContext, argv[1], NULL, NULL) != 0)
    {
        logging("ERROR could not open the file");
        return -1;
    }
    return 0;
}


#include <libavformat/avformat.h>
#include <libavutil/log.h>
#include <stdbool.h>

int main(void)
{
    AVFormatContext *fmt_ctx = NULL;
    int              ret;
    char *           filename = "../small_bunny_1080p_60fps.mp4";

    if((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)))
        return ret;

    if((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Can not find stream information\n");
        return ret;
    }

    for(int i = 0; i < fmt_ctx->nb_streams; i++)
    {
        av_dump_format(fmt_ctx, i, filename, false);
        printf("--------------------------\n\n");
    }
    avformat_close_input(&fmt_ctx);

    return 0;
}

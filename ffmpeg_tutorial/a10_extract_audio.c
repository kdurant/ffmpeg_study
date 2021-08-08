#include <libavcodec/packet.h>
#include <libavutil/avutil.h>
#include <stdio.h>
#include <stdbool.h>
#include <libavformat/avformat.h>
#include <libavutil/log.h>
#include <libavformat/avio.h>

void adts_header(char *header, int dataLen)
{
    // aac级别
    int aac_type = 2 - 1;
    // 采样率下标
    /** 0: 96000 Hz
     1: 88200 Hz
     2: 64000 Hz
     3: 48000 Hz
     4: 44100 Hz
     5: 32000 Hz
     6: 24000 Hz
     7: 22050 Hz
     8: 16000 Hz
     9: 12000 Hz
     10: 11025 Hz
     11: 8000 Hz
     12: 7350 Hz */
    // 采样率下标：LC格式的为正常索引，HE格式的索引为除2后对应的采样索引
    // 因为：HE使用了SBR技术，即 Spectral Band Replication(频段复制)，所以存储同样的音频内容，HE文件较小。使用时采样率为LC的一半。
    int sampling_frequency_index = 4;
    // 声道数
    int channel_config = 2;

    // ADTS帧长度,包括ADTS长度和AAC声音数据长度的和。
    int adtsLen = dataLen + 7;

    // syncword,标识一个帧的开始，固定为0xFFF,占12bit(byte0占8位,byte1占前4位)
    header[0] = 0xff;
    header[1] = 0xf0;

    // ID,MPEG 标示符。0表示MPEG-4，1表示MPEG-2。占1bit(byte1第5位)
    header[1] |= (0 << 3);

    // layer,固定为0，占2bit(byte1第6、7位)
    header[1] |= (0 << 1);

    // protection_absent，标识是否进行误码校验。0表示有CRC校验，1表示没有CRC校验。占1bit(byte1第8位)
    header[1] |= 1;

    // profile,标识使用哪个级别的AAC。1: AAC Main 2:AAC LC 3:AAC SSR 4:AAC LTP。占2bit(byte2第1、2位)
    header[2] = aac_type << 6;

    // sampling_frequency_index,采样率的下标。占4bit(byte2第3、4、5、6位)
    header[2] |= (sampling_frequency_index & 0x0f) << 2;

    // private_bit,私有位，编码时设置为0，解码时忽略。占1bit(byte2第7位)
    header[2] |= (0 << 1);

    // channel_configuration,声道数。占3bit(byte2第8位和byte3第1、2位)
    header[2] |= (channel_config & 0x04) >> 2;
    header[3] = (channel_config & 0x03) << 6;

    // original_copy,编码时设置为0，解码时忽略。占1bit(byte3第3位)
    header[3] |= (0 << 5);

    // home,编码时设置为0，解码时忽略。占1bit(byte3第4位)
    header[3] |= (0 << 4);

    // copyrighted_id_bit,编码时设置为0，解码时忽略。占1bit(byte3第5位)
    header[3] |= (0 << 3);

    // copyrighted_id_start,编码时设置为0，解码时忽略。占1bit(byte3第6位)
    header[3] |= (0 << 2);

    // aac_frame_length,ADTS帧长度,包括ADTS长度和AAC声音数据长度的和。占13bit(byte3第7、8位，byte4全部，byte5第1-3位)
    header[3] |= ((adtsLen & 0x1800) >> 11);
    header[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);
    header[5] = (uint8_t)((adtsLen & 0x7) << 5);

    // adts_buffer_fullness，固定为0x7FF。表示是码率可变的码流 。占11bit(byte5后5位，byte6前6位)
    header[5] |= 0x1f;
    header[6] = 0xfc;

    // number_of_raw_data_blocks_in_frame,值为a的话表示ADST帧中有a+1个原始帧，(一个AAC原始帧包含一段时间内1024个采样及相关数据)。占2bit（byte6第7、8位）。
    header[6] |= 0;
}

int main(void)
{
    AVFormatContext *fmt_ctx = NULL;
    AVPacket         pkt;

    int   ret;
    char *in_filename = "../Wildlife.wmv";

    if((ret = avformat_open_input(&fmt_ctx, in_filename, NULL, NULL)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Can not open file %s\n", in_filename);
        return ret;
    }

    av_dump_format(fmt_ctx, 0, in_filename, false);

    FILE *dst = fopen("output.acc", "wb");
    if(dst == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "Can't open file!\n");
        avformat_close_input(&fmt_ctx);
        return -1;
    }

    int audio_index = 0;
    if((audio_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Can't find the best stream!\n");
        avformat_close_input(&fmt_ctx);
        fclose(dst);
        return -1;
    }

    while(av_read_frame(fmt_ctx, &pkt) >= 0)
    {
        if(pkt.stream_index == audio_index)
        {
            char adts_header_buf[7];
            adts_header(adts_header_buf, pkt.size);
            fwrite(adts_header_buf, 1, 7, dst);

            if(fwrite(pkt.data, 1, pkt.size, dst) != pkt.size)
            {
                av_log(NULL, AV_LOG_WARNING, "warning: length of data is not equal size of pkt!\n");
            }
        }
        av_packet_unref(&pkt);
    }

    avformat_close_input(&fmt_ctx);
    fclose(dst);

    return 0;
}

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/error.h>
}
#include <iostream>

const char *ErrorStr(int err_num)
{
    static char err[256];
    av_strerror(err_num, err, 255);
    return err;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: ./hls input_filename output_filename" << std::endl;
        return -1;
    }
    // 创建输入上下文对象
    AVFormatContext *inputContext = nullptr, *outputContext = nullptr;
    int ret = avformat_open_input(&inputContext, argv[1], nullptr, nullptr);
    if (ret < 0)
    {
        std::cout << "Open input file failed: " << ErrorStr(ret) << std::endl;
        return -1;
    }
    // 查找输入流信息
    ret = avformat_find_stream_info(inputContext, nullptr);
    if (ret < 0)
    {
        std::cout << "Find stream info failed: " << ErrorStr(ret) << std::endl;
        return -1;
    }
    // 创建输出上下文对象
    ret = avformat_alloc_output_context2(&outputContext, nullptr, "hls", argv[2]);
    if (ret < 0)
    {
        std::cout << "Alloc output context failed: " << ErrorStr(ret) << std::endl;
        return -1;
    }

    // 为输出上下文创建流, 并添加参数到输出上下文
    for (int i = 0; i < inputContext->nb_streams; i++)
    {
        AVStream *inputStream = inputContext->streams[i];
        AVStream *outputStream = avformat_new_stream(outputContext, nullptr);
        avcodec_parameters_copy(outputStream->codecpar, inputStream->codecpar);
        outputStream->avg_frame_rate = inputStream->avg_frame_rate;
        outputStream->r_frame_rate = inputStream->r_frame_rate;
    }
    // 设置输出上下文参数
    AVDictionary *dict = nullptr;
    av_dict_set(&dict, "hls_time", "10", 0);
    av_dict_set(&dict, "hls_base_url", "http://localhost:9000/video/", 0);
    av_dict_set(&dict, "hls_playlist_type", "vod", 0);

    // 写入输出上下文头
    ret = avformat_write_header(outputContext, &dict);
    if (ret < 0)
    {
        std::cout << "Write header failed: " << ErrorStr(ret) << std::endl;
        return -1;
    }

    // 读取输入上下文的包 
    AVPacket pkt;
    while (av_read_frame(inputContext, &pkt) >= 0)
    {
        AVStream *inputStream = inputContext->streams[pkt.stream_index];
        AVStream *outputStream = outputContext->streams[pkt.stream_index];
        if (pkt.pts == AV_NOPTS_VALUE)
        {
            // 若输入包的输出时间戳无效, 则设置为0, 并将dts设置为pts
            pkt.pts = av_rescale_q(0, AV_TIME_BASE_Q, inputStream->time_base);
            pkt.dts = pkt.pts;
        }
        av_packet_rescale_ts(&pkt, inputStream->time_base, outputStream->time_base);

        ret = av_interleaved_write_frame(outputContext, &pkt);
        if (ret < 0)
        {
            std::cout << "Write frame to output context failed: " << ErrorStr(ret) << std::endl;
            return -1;
        }
        // 释放输入上下文的包
        av_packet_unref(&pkt);
    }
    // 写入输出上下文尾
    ret = av_write_trailer(outputContext);
    if (ret < 0)
    {
        std::cout << "Write trailer failed: " << ErrorStr(ret) << std::endl;
        return -1;
    }

    // 释放
    av_dict_free(&dict);
    avformat_free_context(outputContext);
    avformat_close_input(&inputContext); 
    return 0;
}
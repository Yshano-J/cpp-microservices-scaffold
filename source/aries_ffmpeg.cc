
#include "aries_ffmpeg.h"

namespace aries_ffmpeg {

    M3U8Info::M3U8Info(const std::string &filename): _filename(filename){}

    bool M3U8Info::parse() {
        std::string content;
        aries_util::FILE_UTIL::read(_filename, content);
        if (content.empty())
        {
            return false;
        }
        std::vector<std::string> parts;
        int count = aries_util::STRING::split(content, "\n", parts);
        if(count == 0)
            return false;

        for(int i = 0; i < count; i++) {
            if (parts[i] == M3U8_ENDLIST) {
                break;
            }
            std::string line = parts[i];
            int pos = line.find(":");
            line = line.substr(0, pos);
            if(line == M3U8_EXTINF) {
                _slices.push_back({parts[i], parts[i + 1]});
                i++;
                continue;
            }
            _headers.push_back(parts[i]);
        }
        return true;
    }

    bool M3U8Info::write() {
        std::stringstream ss;
        // 写入头信息
        for(auto &header: _headers) {
            ss << header << std::endl;
        }
        // 写入切片信息
        for(auto &slice: _slices) {
            ss << slice.first << std::endl;
            ss << slice.second << std::endl;
        }
        ss << M3U8_ENDLIST; // 结束列表
        return aries_util::FILE_UTIL::write(_filename, ss.str());
    }

    // 获取头信息
    std::vector<std::string> &M3U8Info::headers() {
        return _headers;
    }
    // 获取切片信息
    std::vector<std::pair<std::string, std::string>> &M3U8Info::slices() {
        return _slices;
    }

    const char *ErrorStr(int err_num)
    {
        static char err[256];
        av_strerror(err_num, err, 255);
        return err;
    }

    HLS::HLS(const aries_ffmpeg::HLS_Setting &setting): _setting(setting){}
    bool HLS::decode(std::string& input, std::string& output) {
        // 创建输入上下文对象
        AVFormatContext *inputContext = nullptr, *outputContext = nullptr;
        int ret = avformat_open_input(&inputContext, input.c_str(), nullptr, nullptr);
        if (ret < 0)
        {
            std::cout << "Open input file failed: " << ErrorStr(ret) << std::endl;
            return false;
        }
        // 查找输入流信息
        ret = avformat_find_stream_info(inputContext, nullptr);
        if (ret < 0)
        {
            std::cout << "Find stream info failed: " << ErrorStr(ret) << std::endl;
            avformat_close_input(&inputContext);
            return false;
        }
        // 创建输出上下文对象
        ret = avformat_alloc_output_context2(&outputContext, nullptr, "hls", output.c_str());
        if (ret < 0)
        {
            std::cout << "Alloc output context failed: " << ErrorStr(ret) << std::endl;

            avformat_free_context(outputContext);
            avformat_close_input(&inputContext);
            return false;
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
        av_dict_set(&dict, "hls_time", std::to_string(_setting._hls_time).c_str(), 0);
        av_dict_set(&dict, "hls_base_url", _setting._base_url.c_str(), 0);
        av_dict_set(&dict, "hls_playlist_type", _setting._playlist_type.c_str(), 0);

        // 写入输出上下文头
        ret = avformat_write_header(outputContext, &dict);
        if (ret < 0)
        {
            std::cout << "Write header failed: " << ErrorStr(ret) << std::endl;

            av_dict_free(&dict);
            avformat_free_context(outputContext);
            avformat_close_input(&inputContext);
            return false;
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
            // 输入数据时间戳转换为输出数据时间戳
            av_packet_rescale_ts(&pkt, inputStream->time_base, outputStream->time_base);

            ret = av_interleaved_write_frame(outputContext, &pkt);
            if (ret < 0)
            {
                std::cout << "Write frame to output context failed: " << ErrorStr(ret) << std::endl;

                av_dict_free(&dict);
                avformat_free_context(outputContext);
                avformat_close_input(&inputContext);
                return false;
            }
            // 释放输入上下文的包
            av_packet_unref(&pkt);
        }
        // 写入输出上下文尾
        ret = av_write_trailer(outputContext);
        if (ret < 0)
        {
            std::cout << "Write trailer failed: " << ErrorStr(ret) << std::endl;

            av_dict_free(&dict);
            avformat_free_context(outputContext);
            avformat_close_input(&inputContext);
            return false;
        }

        // 释放资源
        av_dict_free(&dict);
        avformat_free_context(outputContext);
        avformat_close_input(&inputContext);
        return true;
    }
}
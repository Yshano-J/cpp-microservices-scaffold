#include "../../source/aries_ffmpeg.h"

void hls_test()
{
    aries_ffmpeg::HLS_Setting hls_settings = {
        ._hls_time = 10,
        ._base_url = "http://localhost:9000/video/",
        ._playlist_type = "vod"
    };
    aries_ffmpeg::HLS hls(hls_settings);
    std::string input = "./test.mp4"; 
    std::string output = "./resource/test.m3u8";
    hls.decode(input, output);
}

void m3u8_test()
{
    std::string input = "./resource/test.m3u8";
    aries_ffmpeg::M3U8Info m3u8_info(input);
    m3u8_info.parse();

    for (auto &header : m3u8_info.headers())
    {
        std::cout << header << std::endl;
    }
    std::cout << "--------- header end ---------" << std::endl;
    for (auto& slice : m3u8_info.slices())
    {
        std::cout << slice.first << std::endl;
        std::cout << slice.second << std::endl;
    }
    // m3u8_info.write();
}

int main()
{
    hls_test();
    m3u8_test();
    return 0;
}

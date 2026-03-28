
#pragma once

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/error.h>
}
#include <iostream>
#include <vector>
#include <sstream>
#include "util.h"
#include "aries_log.h"
/*
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-TARGETDURATION:10
#EXT-X-MEDIA-SEQUENCE:0
#EXT-X-PLAYLIST-TYPE:VOD
*/
namespace aries_ffmpeg {

    const std::string M3U8_HEADER = "#EXTM3U";
    const std::string M3U8_VERSION = "#EXT-X-VERSION";
    const std::string M3U8_TARGET_DURATION = "#EXT-X-TARGETDURATION";
    const std::string M3U8_MEDIA_SEQUENCE = "#EXT-X-MEDIA-SEQUENCE";
    const std::string M3U8_PLAYLIST_TYPE = "#EXT-X-PLAYLIST-TYPE";
    const std::string M3U8_INDEPENDENT_SEGMENTS = "#EXT-X-INDEPENDENT-SEGMENTS";
    const std::string M3U8_ENDLIST = "#EXT-X-ENDLIST";
    const std::string M3U8_EXTINF = "#EXTINF";

    class M3U8Info {
        public:
            M3U8Info(const std::string& filename);
            bool parse();
            bool write();
            std::vector<std::string>& headers();
            std::vector<std::pair<std::string, std::string>>& slices();
        private:
            std::string _filename;
            std::vector<std::string> _headers; 
            std::vector<std::pair<std::string, std::string>> _slices;
    };

    struct hls_setting {
        size_t _hls_time;
        std::string _base_url;
        std::string _playlist_type;
    };
    using HLS_Setting = hls_setting;

    class HLS {
        public:
            HLS(const HLS_Setting& setting);
            bool decode(std::string& input, std::string& output);
        private:
            HLS_Setting _setting;
    };
}

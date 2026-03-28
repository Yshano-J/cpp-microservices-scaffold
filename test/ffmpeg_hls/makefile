
hls_test: hls_test.cc ../../source/aries_ffmpeg.cc ../../source/aries_log.cc ../../source/util.cc
	g++ $^ -o $@ -lavcodec -lavformat -lavutil -lpthread -lgflags -lfmt -lspdlog -ljsoncpp

clean:
	rm -rf hls_test
	rm -rf ./resource/test*
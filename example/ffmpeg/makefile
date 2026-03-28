all: hls server

hls: hls.cc
	g++ hls.cc -o hls -lavformat -lavcodec -lavutil
server: server.cc
	g++ server.cc -o server -lpthread
clean:
	rm -f hls
	rm -f output*
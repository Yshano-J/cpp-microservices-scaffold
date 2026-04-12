CFLAGS=-I../../source -std=c++17
util:util.cc ../../source/util.cc ../../source/aries_log.cc
	g++ $(CFLAGS) $^ -o $@ -lpthread -lgflags -lfmt -ljsoncpp
	
.PHONY:clean
clean:
	rm -rf util
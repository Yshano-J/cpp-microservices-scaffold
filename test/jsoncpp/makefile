CFLAGS=-I../../source -std=c++17
json:json_test.cc ../../source/util.cc ../../source/aries_log.cc
	g++ $(CFLAGS) $^ -o $@ -lpthread -lgflags -lfmt -ljsoncpp
	
.PHONY:clean
clean:
	rm -rf json
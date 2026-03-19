CFLAGS=-I../../source -std=c++17
log:log_test.cc ../../source/aries_log.cc
	g++ $(CFLAGS) $^ -o $@ -lpthread -lgflags -lfmt
	
.PHONY:clean
clean:
	rm -rf log
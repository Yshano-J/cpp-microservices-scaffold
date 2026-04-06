test: test.cc ../../source/aries_redis.cc
	g++ -std=c++17 $^ -o $@ -lhiredis -lredis++ -lpthread

clean:
	rm -f test
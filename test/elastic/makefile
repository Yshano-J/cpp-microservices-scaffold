all: base es
base: base.cc ../../source/aries_elastic.cc ../../source/util.cc ../../source/aries_log.cc
	g++ -std=c++17 $^ -o $@ -lspdlog -lfmt -ljsoncpp -lcpr  -lelasticlient
es: es.cc ../../source/aries_elastic.cc ../../source/util.cc ../../source/aries_log.cc
	g++ -std=c++17 $^ -o $@ -lspdlog -lfmt -ljsoncpp -lcpr  -lelasticlient

clean:
	rm -rf base es

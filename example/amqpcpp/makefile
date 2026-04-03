all:simple_producer simple_consumer dlx_producer dlx_consumer

simple_producer:simple_producer.cc ../../source/aries_log.cc
	g++ -std=c++17 $^ -o $@ -lamqpcpp -lpthread -lspdlog -lfmt -lev
simple_consumer:simple_consumer.cc ../../source/aries_log.cc
	g++ -std=c++17 $^ -o $@ -lamqpcpp -lpthread -lspdlog -lfmt -lev
dlx_producer:dlx_producer.cc ../../source/aries_log.cc
	g++ -std=c++17 $^ -o $@ -lamqpcpp -lpthread -lspdlog -lfmt -lev
dlx_consumer:dlx_consumer.cc ../../source/aries_log.cc
	g++ -std=c++17 $^ -o $@ -lamqpcpp -lpthread -lspdlog -lfmt -lev

clean:
	rm -f simple_producer simple_consumer dlx_producer dlx_consumer
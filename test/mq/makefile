all: delayed_producer delayed_consumer

simple_producer:simple_producer.cc ../../source/aries_log.cc ../../source/aries_amqpcpp.cc
	g++ -std=c++17 $^ -o $@ -lamqpcpp -lpthread -lspdlog -lfmt -lev
simple_consumer:simple_consumer.cc ../../source/aries_log.cc ../../source/aries_amqpcpp.cc
	g++ -std=c++17 $^ -o $@ -lamqpcpp -lpthread -lspdlog -lfmt -lev
delayed_producer:delayed_producer.cc ../../source/aries_log.cc ../../source/aries_amqpcpp.cc
	g++ -std=c++17 $^ -o $@ -lamqpcpp -lpthread -lspdlog -lfmt -lev
delayed_consumer:delayed_consumer.cc ../../source/aries_log.cc ../../source/aries_amqpcpp.cc
	g++ -std=c++17 $^ -o $@ -lamqpcpp -lpthread -lspdlog -lfmt -lev

clean:
	rm -f simple_producer simple_consumer delayed_producer delayed_consumer
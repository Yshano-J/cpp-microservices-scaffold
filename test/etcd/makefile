all : discovery register

discovery: discovery.cc ../../source/util.cc ../../source/aries_log.cc ../../source/aries_etcd_service_register.cc
	g++ -std=c++17 $^ -o $@ -ljsoncpp -lpthread -lfmt -lspdlog -lcpprest -letcd-cpp-api
register: register.cc ../../source/util.cc ../../source/aries_log.cc ../../source/aries_etcd_service_register.cc
	g++ -std=c++17 $^ -o $@ -ljsoncpp -lpthread -lfmt -lspdlog -lcpprest -letcd-cpp-api

clean:
	rm -f discovery register
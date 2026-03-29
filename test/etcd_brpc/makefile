all: server client

server:rpc_server.cc cal.pb.cc ../../source/aries_brpc.cc ../../source/aries_log.cc ../../source/aries_etcd_service_register.cc ../../source/util.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lgflags -lssl -lcrypto -ljsoncpp -lpthread -lfmt -lspdlog -lcpprest -letcd-cpp-api
client:rpc_client.cc cal.pb.cc ../../source/aries_brpc.cc ../../source/aries_log.cc ../../source/aries_etcd_service_register.cc ../../source/util.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto -lfmt -lspdlog -lcpprest -letcd-cpp-api -ljsoncpp

%.pb.cc: %.proto
	protoc --cpp_out=./ $^

clean:
	rm -rf server client
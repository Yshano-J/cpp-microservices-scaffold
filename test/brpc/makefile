all: server client

server: rpc_server.cc cal.pb.cc ../../source/aries_brpc.cc ../../source/aries_log.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto -lfmt
client: rpc_client.cc cal.pb.cc ../../source/aries_brpc.cc ../../source/aries_log.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto -lfmt

%.pb.cc: %.proto
	protoc --cpp_out=./ $^

clean:
	rm -rf server client
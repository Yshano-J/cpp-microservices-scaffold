all: server client async_client async_server http_server http_client

server: server.cc cal.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto
client: client.cc cal.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto
async_client: async_client.cc cal.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto
async_server: async_server.cc cal.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto
http_server: http_server.cc cal.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto
http_client: http_client.cc cal.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto

%.pb.cc: %.proto
	protoc --cpp_out=./ $^

clean:
	rm -rf server client async async_server http_server http_client 
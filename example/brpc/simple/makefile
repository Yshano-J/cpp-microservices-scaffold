all: server client
server: server.cc cal.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto
client: client.cc cal.pb.cc
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -ldl -lprotobuf -lpthread -lgflags -lssl -lcrypto

clean:
	rm -rf server client
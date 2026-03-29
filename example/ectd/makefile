all: get put dirget dirput

get: get.cc
	g++ -std=c++17 $^ -o $@ -letcd-cpp-api -lcpprest
put: put.cc
	g++ -std=c++17 $^ -o $@ -letcd-cpp-api -lcpprest
dirget: dirget.cc
	g++ -std=c++17 $^ -o $@ -letcd-cpp-api -lcpprest
dirput: dirput.cc
	g++ -std=c++17 $^ -o $@ -letcd-cpp-api -lcpprest
	
clean:
	rm -rf get put dirget dirput

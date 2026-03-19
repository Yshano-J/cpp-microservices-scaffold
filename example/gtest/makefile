all : simple global local
simple:simple.cc
	g++ $^ -o $@ -lgtest
global:global.cc
	g++ $^ -o $@ -lgtest
local:local.cc
	g++ $^ -o $@ -lgtest
.PHONY:clean
clean:
	rm -rf simple global local
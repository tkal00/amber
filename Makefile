SRC := $(shell find -type f -iregex ".*\.cpp")

server:
	g++ $(SRC) -o server.run -std=c++23 -I./ -g -O0 -Wall

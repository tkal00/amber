SRC = $(shell find -type f -iregex ".*\.cpp")
OBJ = $(shell find -type f -iregex ".*/.o")
CFLAGS = -Wall
CXXFLAGS = -g
OUT = server.run
OBJOUT = bin/obj/

server:
	g++ -g -std=c++23 -O0 -Wall -o -I./ $(OUT) $(OBJ)

obj: $(SRC)/%.cpp
	echo $(shell find -type f -iregex ".*\.cpp")
	g++ -g -std=c++23 -O0 -Wall -I./ -c $(SRC)

$(SRC)/%.cpp:
	g++ -g -std=c++23 -O0 -Wall -I./ -c $@

clean:
	rm $(shell find -type f -iregex ".*\.o")
	rm $(OUT)

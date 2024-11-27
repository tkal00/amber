SRC = $(shell find src/ -type f -iregex ".*\.cpp")
OBJ = $(patsubst %.cpp,%.o,$(SRC))
CFLAGS = -Wall
CXXFLAGS = -g
OUT = server.run
OBJOUT = bin/obj/
SRCDIR=src/

server:
	g++ -g -std=c++23 -O0 -Wall -o -I$(SRCDIR) -o $(OUT) $(OBJ)

%.o: $(SRC)/%.cpp
	echo $(shell find src/ -type f -iregex ".*\.cpp")
	g++ -g -std=c++23 -O0 -Wall -I$(SRCDIR) -c $(SRC)

$(SRC)/%.cpp:
	g++ -g -std=c++23 -O0 -Wall -I$(SRCDIR) -c $@

clean:
	rm $(shell find -type f -iregex ".*\.o")
	rm $(OUT)

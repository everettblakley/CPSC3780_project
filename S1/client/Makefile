# Makefile for the s1 client program
#
CXX = g++
CXXFLAGS = -std=c++11 -g
SRC_DIR = ../../src
SRC_INCLUDE = ../../include
INCLUDE = -I ${SRC_INCLUDE}

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all : client

client: $(SRC_DIR)
	$(clean)
	$(CXX) $(CXXFLAGS) -o client $(INCLUDE) $(SRC_DIR)/*.cpp client.cpp
	./client

clean:
	rm -f *.o client

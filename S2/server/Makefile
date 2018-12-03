# Makefile for the S2 server program
#
CXX = g++
CXXFLAGS = -std=c++11 -g -lpthread
SRC_DIR = ../../src
SRC_INCLUDE = ../../include
INCLUDE = -I ${SRC_INCLUDE}

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all : server

server: $(SRC_DIR)
	$(CXX) $(CXXFLAGS) -o server $(INCLUDE) $(SRC_DIR)/*.cpp server.cpp
	serverS

clean:
	rm -f *.o server

# Makefile for the socket programming tutorial
#
CXX = g++
CXXFLAGS = -std=c++0x -g -lpthread
SRC_DIR = ../src
SRC_INCLUDE = ../include
INCLUDE = -I ${SRC_INCLUDE}


#server = ServerSocket.o Socket.o server.o ServerSocket.h Socket.h SocketException.h

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all : server

server: $(SRC_DIR)
	$(CXX) $(CXXFLAGS) -o server $(INCLUDE) $(SRC_DIR)/*.cpp server.cpp
	server

#Socket: Socket.cpp
#ServerSocket: ServerSocket.cpp
#ClientSocket: ClientSocket.cpp
#server: server.cpp

clean:
	rm -f *.o server

# Makefile for the socket programming tutorial
#
CXX = g++
CXXFLAGS = -std=c++0x -g

server = ServerSocket.o Socket.o server.o ServerSocket.h Socket.h SocketException.h

all : server

server: $(server)
	$(CXX) $(CXXFLAGS) -o server $(server)
	server

Socket: Socket.cpp
ServerSocket: ServerSocket.cpp
ClientSocket: ClientSocket.cpp
server: server.cpp

clean:
	rm -f *.o server

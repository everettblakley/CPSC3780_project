# Makefile for the socket programming tutorial
#
CXX = g++
CXXFLAGS = -std=c++0x -g

client = ClientSocket.o Socket.o client.o ServerSocket.h Socket.h SocketException.h

all : client

client: $(client)
	$(CXX) $(CXXFLAGS) -o client $(client)
	client

Socket: Socket.cpp
ServerSocket: ServerSocket.cpp
ClientSocket: ClientSocket.cpp
client: client.cpp

clean:
	rm -f *.o client

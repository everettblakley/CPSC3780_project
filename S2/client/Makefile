# Makefile for the socket programming tutorial
#
CXX = g++
CXXFLAGS = -std=c++0x -g
SRC_DIR = ../src
SRC_INCLUDE = ../include
INCLUDE = -I ${SRC_INCLUDE}

# client = ClientSocket.o Socket.o client.o ServerSocket.h Socket.h SocketException.h

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all : client

client: $(SRC_DIR)
	$(clean)
	$(CXX) $(CXXFLAGS) -o client $(INCLUDE) $(SRC_DIR)/*.cpp client.cpp
	client

# Socket: Socket.cpp
# ServerSocket: ServerSocket.cpp
# ClientSocket: ClientSocket.cpp
# client: client.cpp

clean:
	rm -f *.o client

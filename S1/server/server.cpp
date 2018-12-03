#include "Protocol.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <string>

int main() {
  std::string port = "";
  std::cout << "Please enter the port you would like to start the server on: ";
  std::cin >> port;
  int dataPort = std::stoi(port);

   try{
      // Create the socket
      ServerSocket serverDat(dataPort);
      ServerSocket serverAck(dataPort + 1);

      std::cout << "Running server of ports " << dataPort << " and " << dataPort + 1 << "...\n\n";
      while (true){
      	 ServerSocket new_sockDat;
         ServerSocket new_sockAck;

      	 serverDat.accept(new_sockDat);
         new_sockDat << "30001";
         serverAck.accept(new_sockAck);
      	 try{
           protocol_server(new_sockDat, new_sockAck);
      	 }
      	 catch(SocketException& e){
           std::cout << "Exception caught: " << e.description() << std::endl;
      	 }
      }
   }
   catch (SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
  return 0;
}

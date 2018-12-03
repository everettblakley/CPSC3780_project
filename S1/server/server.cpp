#include "Protocol.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <string>

int main() {
   std::cout << "running....\n\n";
   try{
      // Create the socket
      ServerSocket server(30000);

      while (true){
      	 ServerSocket new_sock;
      	 server.accept(new_sock);
      	 try{
           protocol_server(new_sock);
           std::cout << "File Transfered" << std::endl;
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

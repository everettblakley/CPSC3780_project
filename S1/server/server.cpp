#include "Protocol.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <string>

int main(int argc, int argv[])
{
   std::cout << "running....\n";
   try{
      // Create the socket
      ServerSocket server(30000);

      while (true){
    	 ServerSocket new_sock;
    	 server.accept(new_sock);
    	 try{
         protocol_server(new_sock);
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

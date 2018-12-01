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
      //  int req_count = 0;
    	 try{
    	    while (true){
            protocol_server(new_sock);
    	      //  std::string req;
    	      //  new_sock >> req;
            //  std::cout << "Request #" << req_count << " from client: " << req << std::endl;
            //  std::string data;
            //  data = "Responding to req #";
            //  data += std::to_string(req_count);
    	      //  new_sock << data;
            //  req_count++;
    	    }
    	 }
    	 catch(SocketException&){
    	 }
      }
   }
   catch (SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
  return 0;
}

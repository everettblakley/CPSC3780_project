#include "ClientSocket.h"
#include "SocketException.h"
#include "Protocol.h"
#include <iostream>
#include <string>

int main(int argc, int argv[])
{
   try{
      // Replace "localhost" with the hostname
      // that you're running your server.
      ClientSocket client_socket("localhost", 30000); //IP addr for 36wolverine : 142.66.140.51
      try {
        std::string f ="tutorial.txt";
        protocol_client(client_socket,f);
      }
      catch (SocketException& e) {}
   }
   catch(SocketException& e){
      std::cout << "Exception was caught building socket: " << e.description() << "\n";
   }

   return 0;
}

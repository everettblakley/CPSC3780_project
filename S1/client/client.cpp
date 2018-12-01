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
      ClientSocket client_socket("localhost", 30000);
      // std::string reply;
      try {
        protocol_client(client_socket);
      }
      catch (SocketException&) {
        
      }
      // int req_count = 0;
      // do {
      //   std::string req;
      //   req = "This is request #";
      //   req += std::to_string(req_count);
      //   try {
      //     client_socket << req;
      //     client_socket >> reply;
      //   }
      //   catch(SocketException&){
      //   }
      //   std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;
      //   req_count++;
      // } while(req_count < 100);
   }
   catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\n";
   }

   return 0;
}

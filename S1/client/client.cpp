#include "ClientSocket.h"
#include "SocketException.h"
#include "Protocol.h"
#include <iostream>
#include <string>
#include <stdio.h>

int main() {
   try{
      // Replace "localhost" with the hostname
      // that you're running your server.
      ClientSocket client_socket("localhost", 30000); //IP addr for 36wolverine : 142.66.140.51
      try {
        std::vector<std::string> files{"sherlock_holmes.txt", "tutorial.txt", "file_1.txt" };
        char choice;
        do{
        std::cout << "The available files for transfer are:" << std::endl;
        for (int i = 0; i < files.size(); i++) {
          std::cout << "\t" << i << ". " << files[i] << std::endl;
        }
        std::cout << "Please select the file number to transfer: ";
        std::cin >> choice;
      } while ((int)choice >= 0 && (int)choice < files.size());

        protocol_client(client_socket, files[(int)choice]);
      }
      catch (SocketException& e) {}
   }
   catch(SocketException& e){
      std::cout << "Exception was caught building socket: " << e.description() << "\n";
   }

   return 0;
}

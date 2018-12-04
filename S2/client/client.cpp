#include "ClientSocket.h"
#include "SocketException.h"
#include "Protocol.h"
#include <iostream>
#include <string>
#include <stdio.h>

int main() {
  std::string acknackport; //= 30001;
   try{
      std::string IPaddr = "";
      std::cout << "Enter the IP address of the server (or localhost): ";
      std::cin >> IPaddr;

      std::string port = "30000";
      int dataPort = std::stoi(port);

      //Data port
      ClientSocket client_socketDat(IPaddr, dataPort);

      //Ack/Nak port
      client_socketDat >> acknackport;
      int new_port = std::stoi(acknackport);
      ClientSocket client_socketAck(IPaddr, new_port);

      std::cout << std::endl <<
      "Attempting to connect to " << IPaddr << "..." << std::endl << std::endl;
      try {
        std::vector<std::string> files{"five_random_lines.txt", "large_ebooks.txt", "random_characters.txt",
        "sherlock_holmes.txt", "tutorial.txt"};
        char choice;
        int intChoice;
        std::cout << "The available files for transfer are:" << std::endl;
        for (int i = 0; i < files.size(); i++) {
          std::cout << "\t" << i << ". " << files[i] << std::endl;
        }
        do{
        std::cout << "Please select the file number to transfer: ";
        std::cin >> choice;
        intChoice = (int)choice - 48;
      } while (intChoice < 0 || intChoice >= files.size());

        std::cout << std::endl << "Grabbing " << files[intChoice] << " from the server " << std::endl << std::endl;
        protocol_client(client_socketDat, client_socketAck, files[intChoice]);
      }
      catch (SocketException& e) {}
   }
   catch(SocketException& e){
      std::cout << "Exception was caught building socket: " << e.description() << "\n";
   }

   return 0;
}

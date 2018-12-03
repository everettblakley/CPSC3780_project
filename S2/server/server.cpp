#include "Protocol.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

#define NUM_THREADS 5

void *Bserver( void *);
void *serveClient(void *);

int serverPort = 30000;
ServerSocket serverDat(serverPort);
ServerSocket serverAck(serverPort+1);

int main()
{
  std::cout << "Running server on ports " << serverPort << " and " << serverPort + 1 << "..." << std::endl;
   pthread_t server_thread;
   for(int i = 0; i < 5; i++)
   {
     if (pthread_create( &server_thread, NULL, Bserver, NULL)){
       std::cout << "Could not create server thread" << std::endl;
       exit(-1);
     }
   }
   pthread_exit(NULL);
  return 0;
}

void *serveClient(void * args){
  pthread_detach(pthread_self());
  try{
    while(true)
    {
      ServerSocket new_sock_dat;
      ServerSocket new_sock_ack;

      serverDat.accept(new_sock_dat);
      new_sock_dat << "30001";
      serverAck.accept(new_sock_ack);

      try{
        protocol_server(new_sock_dat, new_sock_ack);
      }
      catch (SocketException& e){}
    }
  }
  catch (SocketException& e)
  {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }
  pthread_exit(NULL);
}

void *Bserver( void *args) {
   pthread_t worker_thread;
     try{
        if (pthread_create(&worker_thread, NULL, serveClient, NULL) != 0)
        {
          std::cout << "Could not create worker thread";
          pthread_exit(NULL);
        }
      }
     catch(SocketException& e){
        std::cout << "Exception caught: " << e.description() << std::endl;
     }
     pthread_exit(NULL);
}

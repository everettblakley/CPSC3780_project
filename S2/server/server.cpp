#include "Protocol.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

#define NUM_THREADS 2

ServerSocket new_sock;

void *Bserver( void *threadid) {
   long ThreadID;
   ThreadID = (long)threadid;
   while(true){
   try{
      protocol_server(new_sock);
      std::cout << "File Transfered" << std::endl;
   }
   catch(SocketException& e){
      std::cout << "Exception caught: " << e.description() << std::endl;
   }
   pthread_exit(threadid);
   }
}


int main()
{
   pthread_t threads[NUM_THREADS];
   int rc;
   int i=0;

   std::cout << "running....\n\n";
   /*for(i = 0; i < NUM_THREADS; i++) {
      std:: cout << "Create thread " << i << std::endl;
      rc = pthread_create( &threads[i], NULL, Bserver, (void *)i);

      if(rc) {
	 std::cout << "Didnt create thread" << std::endl;
	 exit(-1);
	 }*/

   try{
      // Create the socket
      //ServerSocket server(30000);
      
      //while (true){
	 ServerSocket server(30000);
      	 server.accept(new_sock);
	 for(i = 0; i < NUM_THREADS; i++) {
	    std:: cout << "Create thread " << i << std::endl;
	    rc = pthread_create( &threads[i], NULL, Bserver, (void *)i);
	 }
	 //std::cout<<"WHat the fuck is rc= " << rc << std::endl;
	 //rc = pthread_create( &threads[i], NULL, Bserver, (void *)i);
	 //std::cout<<"WHat the fuck is rc= " << rc << std::endl;
	 //pthread_exit(NULL);
      }
   // }
   catch (SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
      pthread_exit(NULL);
   }
  return 0;
}


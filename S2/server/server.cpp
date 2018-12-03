#include "Protocol.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

#define NUM_THREADS 5

ServerSocket new_sock;
ServerSocket server(30000);

void *Bserver( void *threadid) {
   long ThreadID;
   ThreadID = (long)threadid;

   std::cout << "Thread #: " << pthread_self() << std::endl;
   //while(true){
     try{
       server.accept(new_sock);
        protocol_server(new_sock);
        std::cout << "File Transfered" << std::endl;
     }
     catch(SocketException& e){
        std::cout << "Exception caught: " << e.description() << std::endl;
     }
     //pthread_exit(threadid);
   //}

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
	//  ServerSocket server(30000);
  //     	 server.accept(new_sock);
	 for(i = 0; i < 5; i++) {
	    std:: cout << "Create thread " << i << std::endl;
	    pthread_create( &threads[i], NULL, Bserver, (void *)i );
      //pthread_join(threads[i], NULL);
	 }

      }
   // }
   catch (SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";

   }
   pthread_exit(NULL);
  return 0;
}

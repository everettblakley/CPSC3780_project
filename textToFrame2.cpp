#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

struct frame{
   char parity;
   char * data;
};

char calcEvenParity(char *, bool);
void printFrameInfor(std::vector<frame>);
std::vector<frame> buildFrames();

int main(){

   return 0;
}

std::vector<frame>buildFrames(){
   std::ifstream asciiText("files/file_0.txt", std::ios::in|std::ios::binary);
   std::vector<frame> frames;
   std::string buffer;
   int ctr = 0;
   bool invalidFrame = false;
   int currentLineLength = 0;

   if(asciiText.is_open(){
	 while(std::getline(asciiText, buffer){
	       currentLineLength = buffer.length();
	       std::cout << buffer.c_str() << " " <<  currentLineLength << std::endl;
	 }
   }

}

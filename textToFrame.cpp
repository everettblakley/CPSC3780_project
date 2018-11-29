#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

struct frame{
   char parityBit;
   char * data;
};

char parity(char*, bool);
void printFrameInfo(std::vector<frame>);
std::vector<frame> buildFrame();

int main(){
   std::vector<frame> frames;
   std::cout << "Building Frame" << std::endl;
   frames = buildFrame();
   printFrameInfo(frames);
   return 0;
}

/// Creates a vector of frames from a given file
std::vector<frame> buildFrame(){
  std::ifstream asciiText ("files/file_1.txt", std::ios::in|std::ios::binary|std::ios::ate);
  std::vector<frame> frames;
  char * buffer;
  int ctr = 0;
  bool invalid = false;

  if(asciiText.is_open()){
   asciiText.seekg(0, asciiText.beg);
   do{
    buffer = new char[63]; // Are frames 64 bits or 64 characters (256 bits)?
    asciiText.read(buffer, 64); // Would this not read in the line number as well?
    frames.push_back(frame());
    frames[ctr].data = buffer;
    invalid = (ctr % 5 == 0) ? true:false;
    frames[ctr].parityBit = parity(buffer, invalid);
    ctr++;
    } while(!asciiText.eof());
    asciiText.close();
    std::cout << "Lines read: " << std::endl;

  }
  else std::cout << "An error has occured when opening file" << std::endl;
  return frames;
}


/// Calculates the even parity for a buffer with a bad parity bit every 5 frames
char parity(char * buffer, bool invalid){
   int sum = 0;
   for(int i = 0; i < 64; i++) sum += '0'-buffer[i];
   char pbit = (sum%2==0) ? '0' : '1';
   return (!invalid) ? pbit : (pbit == '0') ? '1' : '0';
}

/// Prints frame parity bit and data
void printFrameInfo(std::vector<frame> frames){
   for(int i = 0; i < frames.size(); i++){
        std::cout << "Frame: " << i << std::endl;
        std::cout << frames[i].parityBit << ' ';
        for(int j = 0; j <= 63; j++)
            std::cout << frames[i].data[j];
        std::cout << std::endl;
   }
}

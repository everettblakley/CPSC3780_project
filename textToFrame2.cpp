#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

struct frame{
   char parity;
   char * data;
};

char calcEvenParity(char *, bool);
void printFrameInfo(std::vector<frame>);
std::vector<frame> buildFrames();

int main(){
    std::vector<frame> frames = buildFrames();
    printFrameInfo(frames);
   return 0;
}

std::vector<frame>buildFrames(){
   std::ifstream asciiText("files/file_0.txt", std::ios::in|std::ios::binary);
   std::vector<frame> frames;
   std::string line;
   char * buffer;
   int ctr = 0;
   int bufferLength = 0;
   bool invalidFrame = false;
   int currentLineLength = 0;

   if(asciiText.is_open()){
	 do{
        std::getline(asciiText, line);
        currentLineLength = line.length();
        for(int i = currentLineLength/64; i >= 0; i--){
                if (currentLineLength <= 63){
                    buffer = new char[currentLineLength];
                    bufferLength = currentLineLength;
                }
                else{
                    buffer = new char[64];
                    bufferLength = 64;
                    currentLineLength -= 64;
                }

            frames.push_back(frame());
            for(int j = 0; j < bufferLength-1; j++)
                buffer[j] = line.at(j);
            line.erase(0, bufferLength);
            frames[ctr].data = buffer;
            invalidFrame = (ctr % 5 == 0) ? true:false;
            frames[ctr].parity = calcEvenParity(buffer, invalidFrame);
            ++ctr;

        }
	 }while(!asciiText.eof());
	 asciiText.close();
   }
   else
        std::cout << "Could not find file" << std::endl;
   return frames;

}

char calcEvenParity(char * buffer, bool invalid){
   int sum = 0;
   for(int i = 0; i < 64; i++) sum += '0'-buffer[i];
   char pbit = (sum%2==0) ? '0' : '1';
   return (!invalid) ? pbit : (pbit == '0') ? '1' : '0';
}

void printFrameInfo(std::vector<frame> frames){ for(int i = 0; i < frames.size(); i++) std::cout << frames[i].parity << " " << frames[i].data << std::endl; }

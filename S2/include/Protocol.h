#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>

/// Structure to store a frames contents
struct frame{
  std::string seq;
   std::string parity;
   std::string type;
   std::string line;
   std::string data;
};


struct workerArgs
{
  ServerSocket ss;
};

/// Converts a frame to a string for transmission
std::string frame_to_string(frame f) {
  std::string outString;
  outString = f.seq + f.parity + f.type + f.line + f.data;
  return outString;
}

/// Build a frame from a transmitted string
frame assemble_frame(std::string s) {
  frame outFrame;
  outFrame.seq = s[0];
  outFrame.parity = s[1];
  outFrame.type = s.substr(2,3);
  outFrame.line = s.substr(5, 5);
  if (s.length() > 10) {
    outFrame.data = s.substr(10);
  }
  else outFrame.data = "";
  return outFrame;
}

/// Calculate the parity of a frames data
std::string calcEvenParity(std::string buffer, bool invalid){
   int sum = 0;
   for(int i = 0; i < buffer.length()+1; i++) sum += '0'-buffer[i];
   std::string pbit = (sum%2==0) ? "0" : "1";
   pbit = (!invalid) ? pbit : (pbit == "0") ? "1" : "0";
   return pbit;
}

/// Transmit a frame
void send_data(std::string frame_no, std::string line_no, std::string buffer, ClientSocket& sock, std::string t) {
  frame s;
  s.seq = frame_no;
  s.parity = calcEvenParity(buffer, false);
  s.type = t;
  s.line = line_no;
  s.data = buffer;
  sock << frame_to_string(s);
}

/// Build a set of frame objects from a given text file
std::vector<frame> buildFrames(std::string filename){
   std::ifstream asciiText("./files/"+filename);
   std::vector<frame> frames;
   std::string line;
   std::string buffer;
   int ctr = 0;
   int bufferLength = 0;
   bool invalidFrame = false;
   int currentLineLength = 0;

   if(asciiText.is_open()){
  	 do{
      std::getline(asciiText, line);
      std::string lineNum = line.substr(0, 5);
      line.erase(0, 6);
      currentLineLength = line.length();
      for(int i = int(currentLineLength/64); i >= 0; i--){
              if (currentLineLength < 64){
                  bufferLength = currentLineLength;
              }
              else{
                  bufferLength = 64;
                  currentLineLength -= 64;
              }
          frames.push_back(frame());
          for(int j = 0; j < bufferLength; j++)
              buffer += line.at(j);
          line.erase(0, bufferLength);
          frames[ctr].data = buffer;
          invalidFrame = (ctr % 5 == 0) ? true:false;
          frames[ctr].parity = calcEvenParity(buffer, invalidFrame);
          frames[ctr].line = lineNum;
          frames[ctr].seq = std::to_string(ctr % 2);
          frames[ctr].type = "dat";
          ++ctr;
          buffer = "";
      }
	 }while(!asciiText.eof());
	 asciiText.close();
   }
   else{
        std::cout << "Could not find file" << std::endl;
      }
   return frames;
}

/// Client-side transmission protocol
void protocol_client(ClientSocket& socket, std::string filename = "") {
  std::string receivedData = "";
  int seq_exp = 0; //When starting a session, initial seq_exp is 0
  frame f; // assemble intial 'request' frame
  std::string frameType;
  std::string currentLine = "00001";
  std::ofstream outputFile ("output.txt", std::ios::out);
  f.seq = std::to_string(seq_exp);
  send_data(f.seq, currentLine, filename, socket, "nak"); // send the request to the server
  while(true) {
    frameType = "nak";
    std::string res;
    socket >> res; // get response from the server
    f = assemble_frame(res); // parse string into a frame
    if (f.type == "nat") break;
    std::string parity = calcEvenParity(f.data, false);
    if (f.seq == std::to_string(seq_exp)) {
      if (f.parity == parity) {
        if (f.line == currentLine) {
          receivedData += f.data;
        }
        else {
          if (outputFile.is_open()) {
            outputFile << currentLine << " " << receivedData << std::endl;
          }
          std::cout << currentLine << " " << receivedData << std::endl;
          currentLine = f.line;
          receivedData = f.data;
        }
      frameType = "ack";
      seq_exp = (seq_exp + 1) % 2; // if the response seq no matches, increment
      }
    }
    send_data(std::to_string(seq_exp), currentLine, "", socket, frameType); // send the ack/nak frame
  }
  std::cout << currentLine << " " << receivedData << std::endl;
  outputFile.close();
}

/// Server-side transmission protocol
void protocol_server(ServerSocket& socket) {
  int frameCtr = 0; // current frame being sent
  int seq_exp = 0; // expected ack frame< std::endl;
  std::string req;
  bool first = true;
  socket >> req;
  frame f = assemble_frame(req);
  std::vector<frame> frames = buildFrames(f.data); // all frames for the file
  socket << frame_to_string(frames[frameCtr]);
  std::cout << "Request accepted. \nTransmitting file ..." << std::endl;
  while(frameCtr < frames.size()-1){
    socket >> req;
    f =assemble_frame(req); // build a frame from the request
    if (f.type == "ack"){
      seq_exp = (seq_exp + 1) % 2;
      frameCtr+=1;
      socket << frame_to_string(frames[frameCtr]);
    }
    else{
        std::string parityCheck = calcEvenParity(frames[frameCtr].data, false);
        if(frames[frameCtr].parity != parityCheck){
          frames[frameCtr].parity = parityCheck;
        }
        if(frames[frameCtr].seq != f.seq){
          seq_exp = (seq_exp + 1) % 2;
        }
        socket << frame_to_string(frames[frameCtr]);
    }
  }
  socket << "00nat99999End of File; Close the socket";
  std::cout << "File transmitted" << std::endl;
}

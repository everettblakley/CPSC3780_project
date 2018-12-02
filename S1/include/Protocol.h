#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>

struct frame{
  std::string seq;
   std::string parity;
   std::string type;
   std::string line;
   std::string data;
};

std::string frame_to_string(frame f) {
  std::string outString;
  outString = f.seq + f.parity + f.type + f.line + f.data;
  //std::cout << "Converted frame to " << outString << std::endl;
  return outString;
}

frame assemble_frame(std::string s) {
  //std::cout << "Assembling frame from: " << s << std::endl;
  frame outFrame;
  outFrame.seq = s[0];
  outFrame.parity = s[1];
  outFrame.type = s.substr(2,3);
  outFrame.line = s.substr(5, 5);
  if (s.length() > 10) {
    outFrame.data = s.substr(10);
  } else outFrame.data = "";
  //std::cout << frame_to_string(outFrame) << std::endl;
  return outFrame;
}



std::string calcEvenParity(std::string buffer, bool invalid){
   //std::cout << "Calculating Parity Check for : '" << buffer << "'";
   int sum = 0;
   for(int i = 0; i < buffer.length()+1; i++) sum += '0'-buffer[i];
   std::string pbit = (sum%2==0) ? "0" : "1";
   //std::cout << " " << pbit << " ";
   pbit = (!invalid) ? pbit : (pbit == "0") ? "1" : "0";
   //std::cout << " " << pbit << std::endl;
   return pbit;
}

void send_data(std::string frame_no, std::string line_no, std::string buffer, ClientSocket& sock, std::string t) {
  frame s;
  s.seq = frame_no;
  s.parity = calcEvenParity(buffer, false);
  s.type = t;
  s.line = line_no;
  s.data = buffer;
  // std::cout << frame_to_string(s) << std::endl;

  sock << frame_to_string(s);
}

std::vector<frame> buildFrames(){
   std::ifstream asciiText("tutorial.txt");//, std::ios::in|std::ios::binary);
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
                  // buffer = new char[currentLineLength];
                  bufferLength = currentLineLength;
                  //std::cout<<bufferLength<<std::endl;
              }
              else{
                  // buffer = new char[64];
                  bufferLength = 64;
                  currentLineLength -= 64;
              }
          frames.push_back(frame());
          for(int j = 0; j < bufferLength; j++)
              buffer += line.at(j);
          line.erase(0, bufferLength);
          frames[ctr].data = buffer;
          invalidFrame = (ctr % 5 == 0) ? true:false;
          //std::cout <<"In buffer before parity check in build frame: " <<buffer << std::endl;

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
   else
        std::cout << "Could not find file" << std::endl;
   return frames;

}

void protocol_client(ClientSocket& socket) {
  std::string receivedData = "";
  int seq_exp = 0; //When starting a session, initial seq_exp is 0
  frame f; // assemble intial 'request' frame
  std::string frameType;
  std::string currentLine = "00001";
  std::ofstream outputFile ("output.txt", std::ios::out);
  f.seq = std::to_string(seq_exp);
  frameType = ((f.seq == std::to_string(seq_exp))&&(f.parity == calcEvenParity(f.data, false))) ? "ack" : "nak";
  //std::cout << "First frame: " << frameType << " " << frame_to_string(f) << std::endl;
  send_data(f.seq, currentLine, "", socket, frameType); // send the request to the server
  while(true) {
    std::string res;
    socket >> res; // get response from the server
    // std::cout << "Response: " << res << std::endl;
    f = assemble_frame(res); // parse string into a frame
    std::string parity = calcEvenParity(f.data, false);
    if (f.seq == std::to_string(seq_exp)) {
      if (f.parity == parity) {
                    if (f.line == currentLine) {
                      receivedData += f.data; // If line is still the current line, add
                    } else {
                      if (outputFile.is_open()) {
                        outputFile << currentLine << " " << receivedData << std::endl;
                      }
                      std::cout << currentLine << " " << receivedData << std::endl;
                      currentLine = f.line;
                      receivedData = f.data; // If line is still the current line, add
                    }
                    //receivedData = f.data; // If line is still the current line, add
        frameType = "ack";
        seq_exp = (seq_exp + 1) % 2; // if the response seq no matches, increment
      }
      else{
        frameType = "nak";
      }
    }
    else{
      frameType = "nak";
    }
    send_data(std::to_string(seq_exp), currentLine, "", socket, frameType); // send the ack/nak frame
  }
  std::cout << currentLine << " " << receivedData << std::endl;
  outputFile.close();
}

void protocol_server(ServerSocket& socket) {
  std::vector<frame> frames = buildFrames(); // all frames for the file
  int frameCtr = 0; // current frame being sent
  int seq_exp = 0; // expected ack frame< std::endl;
  std::string req;
  bool first = true;
  socket >> req;
  socket << frame_to_string(frames[frameCtr]);
  while(frameCtr < frames.size()-1){
    socket >> req;
    std::cout << "REQUEST: " << req << std::endl;
    frame f = assemble_frame(req); // build a frame from the request
    std::cout << f.type << " " << frame_to_string(f) << " " << frame_to_string(frames[frameCtr])<< std::endl;
    if (f.type == "ack"){
      seq_exp = (seq_exp + 1) % 2;
      frameCtr+=1;
      std::cout << "ack on frame: " << frame_to_string(frames[frameCtr]) << std::endl;
      socket << frame_to_string(frames[frameCtr]);
    }
    else{
        std::cout << "NAK Frames: " << frame_to_string(f) << " : " << frame_to_string(frames[frameCtr]) << std::endl;
        if(frames[frameCtr].parity != calcEvenParity(frames[frameCtr].data, false)){
          frames[frameCtr].parity = calcEvenParity(frames[frameCtr].data, false);
          std::cout << "NAK Frames (After parity change): "  << frame_to_string(frames[frameCtr]) << std::endl;
        }
        if(frames[frameCtr].seq != f.seq){
          std::cout << "NAK seq bad " << frames[frameCtr].seq << " " << std::to_string(seq_exp) << " " << f.seq << std::endl;
          seq_exp = (seq_exp + 1) % 2;
          std::cout << "NAK seq bad (after change) " << frames[frameCtr].seq << " " << std::to_string(seq_exp) << " " <<f.seq << std::endl;
        }
        socket << frame_to_string(frames[frameCtr]);
    }

  }
}

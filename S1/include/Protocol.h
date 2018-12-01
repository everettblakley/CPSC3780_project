#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

struct frame{
   std::string parity;
   std::string data;
   std::string seq;
   std::string line;
};

frame assemble_frame(std::string s) {
  std::cout << "Assembling frame from: " << s << std::endl;
  frame outFrame;
  outFrame.seq = s[0];
  outFrame.parity = s[1];
  outFrame.line = s.substr(2, 5);
  outFrame.data = s.substr(8);
  return outFrame;
}

std::string frame_to_string(frame f) {
  std::string outString;
  outString = f.seq + f.parity + f.line + f.data;
  // outString += f.parity;
  // outString += f.line;
  // outString += f.data;
  std::cout << "Converted frame to " << outString << std::endl;
  return outString;
}

std::string calcEvenParity(std::string buffer, bool invalid){
   int sum = 0;
   for(int i = 0; i < 64; i++) sum += '0'-buffer[i];
   std::string pbit = (sum%2==0) ? "0" : "1";
   return (!invalid) ? pbit : (pbit == "0") ? "1" : "0";
}

void send_data(std::string frame_no, std::string buffer, ClientSocket sock) {
  frame s;
  s.parity = calcEvenParity(buffer, false);
  s.seq = frame_no;
  s.data = buffer;
  s.line = "99999";
  sock << frame_to_string(s);
  std::cout << "Frame sent to server" << std::endl;
}

std::vector<frame> buildFrames(){
   std::ifstream asciiText("file_1.txt", std::ios::in|std::ios::binary);
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

void protocol_client(ClientSocket socket) {
  std::string receivedData;
  int seq_exp = 0; //When starting a session, initial seq_exp is 0
  frame f; // assemble intial 'request' frame
  f.seq = std::to_string(seq_exp);
  send_data(f.seq, "ack", socket); // send the request to the server
  while(true) {
    std::string res;
    // try {
      socket >> res; // get response from the server
      std::cout << "Response: " << res << std::endl;
      f = assemble_frame(res); // parse string into a frame
      if (f.seq == std::to_string(seq_exp)) {
        if (f.parity == calcEvenParity(f.data, false)) {
          receivedData += f.data; // add the res.data into the string of correct data
          seq_exp = (seq_exp + 1) % 2; // if the response seq no matches, increment
        }
      }
      send_data(std::to_string(seq_exp), "ack", socket); // send the ack/nak frame
    // }
    // catch (SocketException&) {
    //
    // }
  }
  std::cout << receivedData << std::endl; // output all the received data
}

void protocol_server(ServerSocket socket) {
  std::vector<frame> frames = buildFrames(); // all frames for the file
  int frameCtr = 0; // current frame being sent
  int seq_exp = 0; // expected ack frame

  while(true) {
    std::string req;
    socket >> req; // get request from the socket
    frame f = assemble_frame(req); // build a frame from the request
    if (f.seq == std::to_string(seq_exp)) {
      socket << frame_to_string(frames[frameCtr]);
      std::cout << "Frame sent to client: " << frameCtr << std::endl;
      seq_exp = (seq_exp + 1) % 2;
      frameCtr++;
    } else {
      f.parity = calcEvenParity(f.data, false);
      socket << frame_to_string(frames[frameCtr]);
    }
  }

}

#include "cmd_process.h"
#include "cmds.h"
#include <string>
#include <sstream>
#include <Arduino.h>

using namespace std;

class CMDSerial{
private:
  CMD cmd;
  HardwareSerial * serial;
  stringbuf tmp_cmd;
public:
  CMDSerial(HardwareSerial * s): serial(s) {}

  int commandReader();
  int processCmd(const char c);
  int lightControl(int lightId, int lightAmount);
  int parse_int(const char * number, int * int_out);
  int set_pin_mode_input(const uint8_t pin){
    pinMode(pin, INPUT);
    return 0;
  }
  int set_pin_mode_input(const uint8_t pin){
    pinMode(pin, OUTPUT);
    return 0;
  }
  int analog_write(const uint8_t pin, uint8_t pwm){
    analogWrite(pin, pwm);
    return 0;
  }
  int analog_read(const uint8_t pin){
    return analogRead(pin);
  }

}

int CMDSerial::commandReader(){
  int recvByte = 0;
  while (s->available() > 0) {
    recvByte = s->read();
    const char c = recvByte;
    this->processCmd(c);
  }
  return 0;
}

int CMDSerial::processCmd(const char c){
  if(c == CMD::ENDLINE){
    string cmd = tmp_cmd.str();
  }
  else{
    cmd.cmd_started = true;
    tmp_cmd.sputc(c);
  }
  return 0;
}


int CMDSerial::parse_int(const char * number, int * int_out){
  istringstream is;
  int n;
  is << number;
  is >> n;
  *int_out = n;
  return 0;
}

int CMDSerial::lightControl(int lightId, int lightAmount){
  return 0;
}
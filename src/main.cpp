#include <Arduino.h>
#include <ArduinoJson.h>
#include "async_comm.h"
#include "cmds.h"
#include "config.h"
#include "cmd_routing.h"

DynamicJsonDocument doc(1024);
AsyncComm asyncComm(&Serial, &doc);
Logger logger(&asyncComm);
CMDRouter cmdRouter("test_dev", &asyncComm, &asyncComm, &logger);
CMD cmd("test", &logger);

void setup() {
  
}

void loop() {
  
}
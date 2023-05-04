#include <Arduino.h>
#include <ArduinoJson.h>
#include "async_comm.h"
#include "cmds.h"
#include "config.h"
#include "cmd_routing.h"

DynamicJsonDocument doc(1024);
AsyncComm asyncComm(&Serial, &doc);
CMDRouter cmdRouter("test_dev", &asyncComm, &asyncComm);
CMD cmd("test", &asyncComm);

void setup() {
  asyncComm.setup_hook();
}

void loop() {
  
}
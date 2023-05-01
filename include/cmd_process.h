
#ifndef CMD_PROCESS_H
#define CMD_PROCESS_H


#include <Arduino.h>
#include <ArduinoJson.h>
#include "log.h"

/**
 * Define your commands
 */

enum CMDTypes {
    LOG,
    RPC,
    ACTION
};


/**
 * @brief CMDSerial class
 * 
*/
class CMDSerial {
public:
    CMDSerial(const HardwareSerial& serial, const DynamicJsonDocument& doc, const Log& log): doc(doc), serial(serial), log(log), errorProcessing(false){}
    void setup_hook();
    void loop_hook();
    void cmdProcess();
private:
    DynamicJsonDocument doc;
    HardwareSerial serial;
    Log log;
    char *errorMsg;
    bool errorProcessing;
};

void CMDSerial::setup_hook(){

}

void CMDSerial::loop_hook(){
    if (serial.available() > 0) {
        DeserializationError error = deserializeJson(doc, serial);
        if (error) {
            char buf[100];
            strcpy(buf ,"deserializeJson() failed: ");
            strncpy(&buf[28], error.c_str(), 73);
            log.printError(buf);
        }
    }
}

#endif // CMD_PROCESS_H

#ifndef CMD_PROCESS_H
#define CMD_PROCESS_H


#include <Arduino.h>
#include <ArduinoJson.h>
#include "log.h"
#include "cmds.h"

/**
 * Define your commands
 */

enum CMDTypes {
    LOG,
    RPC,
    EVENT
};


/**
 * @brief CMDSerial class
 * Compromises - 
 * 1. JSON, if you want to use just RPC without any asynchronous events, then you're better off using simpleRPC library.
 * 2. No support for multiple arguments, but you can use single argument.
 * 3. No support for multiple return values, but you can use single return value.
 * 4. No support for multiple commands, but you can use single command.
*/
class CMDSerial {
public:
    CMDSerial(Stream& serial, const DynamicJsonDocument& doc, const Log& log): doc(doc), serial(serial), log(log), errorProcessing(false){}
    void setup_hook();
    void loop_hook();
    void cmdProcess();
    void processLog();
    void processRPC();
    void processEvent();
    void searchAndCallCmd(const char * cmdName){
        CMD<void (*)(),const char *,void*, void*> * cmd_ptr = cmd;
        while(cmd_ptr != NULL){
            if(cmd_ptr->isFunction(cmdName)){
                cmd_ptr->_call();

                return;
            }
            cmd_ptr = cmd_ptr->_next();
        }
    }
    void determineType(){
        if(isLog()){
            type = LOG;
        }else if(isRPC()){
            type = RPC;
        }else{
            type = EVENT;
        }
    }
    bool isLog(){
        return strcmp(doc["type"], "log") == 0;
    }
    bool isRPC(){
        return strcmp(doc["type"], "rpc") == 0;
    }
    bool isEvent(){
        return strcmp(doc["type"], "event") == 0;
    }
    bool receivedMsg(){
        return doc.size() > 0;
    }
    bool isCmdProcessedSuccessfully(){
        return cmdProcessedSuccessfully;
    }
    void sendMsg(DynamicJsonDocument& doc){
        serializeJson(doc, serial);
    }
    DynamicJsonDocument& getDoc(){
        return doc;
    }
private:
    DynamicJsonDocument doc;
    Stream& serial;
    Log log;
    char *errorMsg;
    bool errorProcessing;
    CMD<void (*)(),const char *,void*, void*> * cmd;
    CMDTypes type;
    bool cmdProcessedSuccessfully;
    bool processingMsg;
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
            log.error(buf);
        }
    }
}

void CMDSerial::cmdProcess(){
    
}

#endif // CMD_PROCESS_H
#ifndef ASYNC_COMM_H
#define ASYNC_COMM_H


#include <Arduino.h>
#include <ArduinoJson.h>
#include "log.h"
#include "cmds.h"

/**
 * Define your commands
 */

enum CommType {
    LOG,
    RPC,
    EVENT
};


/**
 * @brief AsyncComm class
 * Compromises - 
 * 1. JSON, if you want to use just RPC without any asynchronous events, then you're better off using simpleRPC library.
 * 2. No support for multiple arguments, but you can use single argument.
 * 3. No support for multiple return values, but you can use single return value.
 * 4. No support for multiple commands, but you can use single command.
 * 5. BSON would not save much space.
 * 
 * LOG messages are asynchronous, so they could be sent at any time.
 * RPC messages are synchronous, so they are sent only when the previous command is processed.
*/
class AsyncComm {
public:
    AsyncComm(Stream* io, DynamicJsonDocument* doc, Log* log): doc(doc), io(io), log(log), errorProcessing(false){}
    void setup_hook();
    void loop_hook();
    void msgProcess();
    virtual void processLog();
    virtual void processRPC();
    virtual void processEvent();
    bool searchAndCallCmd(const char * cmdName){
        CMD<void (*)(),const char *,void*, void*> * cmd_ptr = cmd;
        while(cmd_ptr != NULL){
            if(cmd_ptr->isFunction(cmdName)){
                cmd_ptr->_call();
                return true;
            }
            cmd_ptr = cmd_ptr->_next();
        }
        return false;
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
        return strcmp(getDoc()["type"], "log") == 0;
    }
    bool isRPC(){
        return strcmp(getDoc()["type"], "rpc") == 0;
    }
    bool isEvent(){
        return strcmp(getDoc()["type"], "event") == 0;
    }
    bool receivedMsg(){
        return doc->size() > 0;
    }
    bool ismsgProcessedSuccessfully(){
        return msgProcessedSuccessfully;
    }
    void sendMsg(DynamicJsonDocument& doc){
        serializeJson(doc, io);
    }
    DynamicJsonDocument& getDoc(){
        return *doc;
    }
    void createMessage(){
        getDoc().clear();
        getDoc()["deviceID"] = deviceID;
        getDoc()["msgID"] = msgID;
        getDoc()["timestamp"] = getTimestamp();
    }
    void createRPCMessage(){
        createMessage();
        getDoc()["type"] = "rpc";
    }
    void createEventMessage(){
        createMessage();
        getDoc()["type"] = "event";
    }
    void setTimestamp(unsigned long timestamp){
        this->timestamp = timestamp-(millis()/1000);
    }
    unsigned long getTimestamp(){
        return timestamp + (millis()/1000);
    }
protected:
    DynamicJsonDocument* doc;
    Stream* io;
    Log* log;
    char *errorMsg;
    bool errorProcessing;
    CMD<void (*)(),const char *,void*, void*> * cmd;
    CommType type;
    bool msgProcessedSuccessfully;
    bool processingMsg;
    unsigned int msgID;
    unsigned int deviceID;
    unsigned long timestamp;
};

void AsyncComm::setup_hook(){

}

void AsyncComm::loop_hook(){
    if (io->available() > 0) {
        DeserializationError error = deserializeJson(getDoc(), *io);
        if (error) {
            log->error("deserializeJson() failed: %s", error.c_str());
        }
    }
}

void AsyncComm::msgProcess(){
    processingMsg = true;
    determineType();
    switch(type){
        case LOG:
            processLog();
            break;
        case RPC:
            processRPC();
            break;
        case EVENT:
            processEvent();
            break;
    }
}

void AsyncComm::processLog(){
    //log->info("Processing log message");
    //msgProcessedSuccessfully = true;
}

void AsyncComm::processRPC(){
    //log->info("Processing rpc message");
    if(searchAndCallCmd(getDoc()["cmd"])){
        msgProcessedSuccessfully = true;
    }
}

void AsyncComm::processEvent(){
    //log->info("Processing event message");
    //msgProcessedSuccessfully = true;
}

#endif // ASYNC_COMM_H
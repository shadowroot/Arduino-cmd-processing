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
        ioizeJson(doc, io);
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
    void setTimestamp(unsigned long timestamp){
        this->timestamp = timestamp-(millis()/1000);
    }
    unsigned long getTimestamp(){
        return timestamp + (millis()/1000);
    }
private:
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
        DeioizationError error = deioizeJson(getDoc(), *io);
        if (error) {
            char buf[100];
            strcpy(buf ,"deioizeJson() failed: ");
            strncpy(&buf[28], error.c_str(), 73);
            log->error(buf);
        }
    }
}

void AsyncComm::msgProcess(){
    
}

#endif // ASYNC_COMM_H
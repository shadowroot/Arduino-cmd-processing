#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "async_comm.h"

class Logger {
public:
    Logger(AsyncComm * asyncComm): asyncComm(asyncComm){}
    void setup_hook();
    void info(const char* fmt...);
    const char* formatString(const char *fmt...);
    void error(const char* fmt...);
    void debug(const char* fmt...);
    unsigned long get_timestamp(){
        return last_timestamp + (millis() - last_millis) / 1000;
    }
    DynamicJsonDocument& getDoc(){
        return asyncComm->getDoc();
    }
    void cleanDoc(){
        getDoc().clear();
    }
    void set_timestamp(unsigned long timestamp){
        last_timestamp = timestamp;
    }
    void createDataDoc(){
        getDoc().clear();
        getDoc()["type"] = "data";
        getDoc()["timestamp"] = get_timestamp();
    }
    void buildDoc(const char* attr, const char* value){
        getDoc()[attr] = value;
    }
    void sendDoc(){
        serializeJson(getDoc(), asyncComm->getIO());
    }
private:
    AsyncComm * asyncComm;
    unsigned long last_timestamp;
    unsigned long last_millis;
};

void Logger::setup_hook(){
}

const char * Logger::formatString(const char *fmt...){
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return buf;
}

void Logger::info(const char* fmt...){
    getDoc().clear();
    getDoc()["type"] = "log";
    getDoc()["level"] = "info";
    getDoc()["timestamp"] = get_timestamp();
    getDoc()["message"] = formatString(fmt);
    serializeJson(getDoc(), asyncComm->getIO());
}

void Logger::error(const char* fmt...){
    getDoc().clear();
    getDoc()["type"] = "log";
    getDoc()["level"] = "error";
    getDoc()["timestamp"] = get_timestamp();
    getDoc()["message"] = formatString(fmt);
    serializeJson(getDoc(), asyncComm->getIO());
}


void Logger::debug(const char* fmt...){
    getDoc().clear();
    getDoc()["type"] = "log";
    getDoc()["level"] = "debug";
    getDoc()["timestamp"] = get_timestamp();
    getDoc()["message"] = formatString(fmt);
    serializeJson(getDoc(), asyncComm->getIO());
}

#endif
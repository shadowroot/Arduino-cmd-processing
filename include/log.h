#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Log {
public:
    Log(AsyncComm * asyncComm): asyncComm(asyncComm){}
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

void Log::setup_hook(){
}

const char * Log::formatString(const char *fmt...){
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return buf;
}

void Log::info(const char* fmt...){
    getDoc().clear();
    getDoc()["type"] = "log";
    getDoc()["level"] = "info";
    getDoc()["timestamp"] = get_timestamp();
    getDoc()["message"] = formatString(fmt);
    serializeJson(getDoc(), asyncComm->getIO());
}

void Log::error(const char* fmt...){
    getDoc().clear();
    getDoc()["type"] = "log";
    getDoc()["level"] = "error";
    getDoc()["timestamp"] = get_timestamp();
    getDoc()["message"] = formatString(fmt);
    serializeJson(getDoc(), asyncComm->getIO());
}


void Log::debug(const char* fmt...){
    getDoc().clear();
    getDoc()["type"] = "log";
    getDoc()["level"] = "debug";
    getDoc()["timestamp"] = get_timestamp();
    getDoc()["message"] = formatString(fmt);
    serializeJson(getDoc(), asyncComm->getIO());
}

#endif
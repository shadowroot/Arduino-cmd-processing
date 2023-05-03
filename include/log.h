#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Log {
public:
    Log(unsigned int deviceID, Stream& serial, DynamicJsonDocument& doc): deviceID(deviceID), serial(serial), doc(doc){}
    void setup_hook();
    void info(const char* fmt...);
    const char* formatString(const char *fmt...);
    void error(const char* fmt...);
    void debug(const char* fmt...);
    unsigned long get_timestamp(){
        return last_timestamp + (millis() - last_millis) / 1000;
    }
    void cleanDoc(){
        doc.clear();
    }
    void set_timestamp(unsigned long timestamp){
        last_timestamp = timestamp;
    }
    void createDataDoc(){
        doc.clear();
        doc["type"] = "data";
        doc["timestamp"] = get_timestamp();
    }
    void buildDoc(const char* attr, const char* value){
        doc[attr] = value;
    }
    void sendDoc(){
        serializeJson(doc, serial);
    }
private:
    Stream& serial;
    DynamicJsonDocument& doc;
    unsigned long last_timestamp;
    unsigned long last_millis;
    unsigned int deviceID;
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
    doc.clear();
    doc["type"] = "log";
    doc["level"] = "info";
    doc["timestamp"] = get_timestamp();
    doc["message"] = formatString(fmt);
    serializeJson(doc, serial);
}

void Log::error(const char* fmt...){
    doc.clear();
    doc["type"] = "log";
    doc["level"] = "error";
    doc["timestamp"] = get_timestamp();
    doc["message"] = formatString(fmt);
    serializeJson(doc, serial);
}


void Log::debug(const char* fmt...){
    doc.clear();
    doc["type"] = "log";
    doc["level"] = "debug";
    doc["timestamp"] = get_timestamp();
    doc["message"] = formatString(fmt);
    serializeJson(doc, serial);
}

#endif
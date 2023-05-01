#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Log {
public:
    Log(const HardwareSerial& serial, const DynamicJsonDocument& doc): serial(serial), doc(doc){}
    void setup_hook();
    void info(const char* msg);
    void error(const char* error);
    void debug(const char* msg);
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
    HardwareSerial serial;
    DynamicJsonDocument doc;
    unsigned long last_timestamp;
    unsigned long last_millis;
};

void Log::setup_hook(){
}

void Log::info(const char* msg){
    doc.clear();
    doc["type"] = "log";
    doc["level"] = "info";
    doc["message"] = msg;
    doc["timestamp"] = get_timestamp();
    serializeJson(doc, serial);
}

void Log::error(const char* error){
    doc.clear();
    doc["type"] = "log";
    doc["level"] = "error";
    doc["message"] = error;
    doc["timestamp"] = get_timestamp();
    serializeJson(doc, serial);
}

void Log::debug(const char* msg){
    doc.clear();
    doc["type"] = "log";
    doc["level"] = "debug";
    doc["message"] = msg;
    doc["timestamp"] = get_timestamp();
    serializeJson(doc, serial);
}

#endif
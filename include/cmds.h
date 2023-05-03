#ifndef CMDS_H
#define CMDS_H
#include "log.h"

/**
 * Define your commands
 */

// enum ARGType {
//     INT,
//     FLOAT,
//     STRING,
//     BOOL,
//     VOID
// };

template <class T>
class CMDArg{
    public:
        CMDArg(const Log& log):log(log), size(0){}
        void _type(ARGType type){
            this->type = type;
        }
        void _set_data(T data){
            size = sizeof(data);
            this->data = data;
        }
        void * _get_data(){
            return &data;
        }
    private:
        Log log;
        ARGType type;
        T data;
        int size;
};

template <class F, class D, class A, class R>
class CMD{
    public:
        CMD(const char * name, const Log& log): name(name), log(log){}
        void _register(F f, D doc, A arg){
            char buf[100];
            strcpy(buf ,"Registering ");
            strncpy(&buf[12], name, 73);
            log.info(buf);
            this->f = f;
            this->doc = doc;
            this->arg = arg;
        }
        void _call(){
            char buf[100];
            strcpy(buf ,"Calling ");
            strncpy(&buf[8], name, 73);
            log.info(buf);
            result = f(arg);
        }
        void _next(CMD * next){
            this->next = next;
        }
        CMD * _next(){
            return next;
        }
        const char * _name(){
            return name;
        }
        bool isFunction(const char * name){
            return strcmp(this->name, name) == 0;
        }
        R get_result(){
            return result;
        }
    private:
        Log log;
        const char * name;
        F f;
        D doc;
        CMD * next;
        A arg;
        R result;
};

template <class F, class D, class R>
class CMD{
    public:
        CMD(const char * name, const Log& log): name(name), log(log){}
        void _register(F f, D doc){
            char buf[100];
            strcpy(buf ,"Registering ");
            strncpy(&buf[12], name, 73);
            log.info(buf);
            this->f = f;
            this->doc = doc;
        }
        void _call(){
            char buf[100];
            strcpy(buf ,"Calling ");
            strncpy(&buf[8], name, 73);
            log.info(buf);
            result = f();
        }
        void _next(CMD * next){
            this->next = next;
        }
        CMD * _next(){
            return next;
        }
        const char * _get_name(){
            return name;
        }
        bool isFunction(const char * name){
            return strcmp(this->name, name) == 0;
        }
    private:
        Log log;
        const char * name;
        F f;
        D doc;
        CMD * next;
        R result;
};



#endif // CMDS_H
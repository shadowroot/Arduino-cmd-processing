#ifndef CMDS_H
#define CMDS_H

#include "async_comm.h"

/**
 * Define your commands
 */

enum ARGType {
    INT,
    FLOAT,
    STRING,
    BOOL,
    VOID
};

// template <class T>
// class CMDArg{
//     public:
//         CMDArg(const Logger& logger):logger(logger), size(0){}
//         void _type(ARGType type){
//             this->type = type;
//         }
//         void _set_data(T data){
//             size = sizeof(data);
//             this->data = data;
//         }
//         void * _get_data(){
//             return &data;
//         }
//     private:
//         Logger logger;
//         ARGType type;
//         T data;
//         int size;
// };

class CMD{
    public:
        CMD(const char * name, AsyncComm* asyncComm): name(name), asyncComm(asyncComm){}
        void _register(char * (*f)(char*), const char * doc, char * arg, ARGType arg_type, ARGType result_type){
            asyncComm->logInfo("Registering %s", name);
            this->f = f;
            this->doc = doc;
            this->arg = arg;
            this->arg_type = arg_type;
            this->result_type = result_type;
        }
        void _register(char * (*f)(), const char * doc, ARGType result_type){
            asyncComm->logInfo("Registering %s", name);
            this->f = (char * (*)(char*))f;
            this->doc = doc;
            this->arg = NULL;
            this->arg_type = VOID;
            this->result_type = result_type;
        }
        void _call(){
            asyncComm->logInfo("Calling %s", name);
            if(arg_type == VOID){
                char * (*fn)() = (char * (*)())f;
                result = fn();
            }
            else{
                result = f(arg);
            }
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
        char * get_result(){
            return result;
        }
    protected:
        AsyncComm * asyncComm;
        const char * name;
        char * (*f)(char*);
        const char * doc;
        CMD * next;
        char * arg;
        ARGType arg_type;
        char * result;
        ARGType result_type;
};



#endif // CMDS_H
#ifndef CMD_ROUTING_H
#define CMD_ROUTING_H
#include "cmd_process.h"

/**
 * Define your commands
 */

class CMDRouter{
    public:
        CMDRouter(CMDSerial* cmdSerial1, CMDSerial* cmdSerial2, const Log& log): cmdSerial1(cmdSerial1), cmdSerial2(cmdSerial2), log(log){}
        void setup_hook(){
            cmdSerial1->setup_hook();
            cmdSerial2->setup_hook();
        }
        void loop_hook(){
            cmdSerial1->loop_hook();
            cmdSerial2->loop_hook();
            if(cmdSerial1->receivedMsg() && !cmdSerial1->isCmdProcessedSuccessfully()){
                log.info("Routing message from cmdSerial1 to cmdSerial2");
                cmdSerial2->sendMsg(cmdSerial1->getDoc());
            }
            else if(cmdSerial2->receivedMsg() && !cmdSerial2->isCmdProcessedSuccessfully()){
                log.info("Routing message from cmdSerial2 to cmdSerial1");
                cmdSerial1->sendMsg(cmdSerial2->getDoc());
            }
        }
    private:
        Log log;
        CMDSerial * cmdSerial1;
        CMDSerial * cmdSerial2;
};

#endif // CMD_ROUTING_H
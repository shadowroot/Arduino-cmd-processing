#include "async_comm.h"
#include "logger.h"

#ifndef CMD_ROUTING_H
#define CMD_ROUTING_H

/**
 * Define your commands
 */

class CMDRouter{
    public:
        CMDRouter(unsigned int deviceID, AsyncComm* cmdSerial1, AsyncComm* cmdSerial2, Logger* logger): deviceID(deviceID), cmdSerial1(cmdSerial1), cmdSerial2(cmdSerial2), logger(logger){}
        void setup_hook(){
            cmdSerial1->setup_hook();
            cmdSerial2->setup_hook();
        }
        void loop_hook(){
            cmdSerial1->loop_hook();
            cmdSerial2->loop_hook();
            if(cmdSerial1->receivedMsg() && !cmdSerial1->ismsgProcessedSuccessfully()){
                logger->info("Routing message from cmdSerial1 to cmdSerial2");
                cmdSerial2->sendMsg(cmdSerial1->getDoc());
            }
            else if(cmdSerial2->receivedMsg() && !cmdSerial2->ismsgProcessedSuccessfully()){
                logger->info("Routing message from cmdSerial2 to cmdSerial1");
                cmdSerial1->sendMsg(cmdSerial2->getDoc());
            }
        }
    private:
        Logger * logger;
        AsyncComm * cmdSerial1;
        AsyncComm * cmdSerial2;
        unsigned int deviceID;
};

#endif // CMD_ROUTING_H
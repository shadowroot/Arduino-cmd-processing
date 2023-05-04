#ifndef CMD_ROUTING_H
#define CMD_ROUTING_H

#include "async_comm.h"

/**
 * Define your commands
 */

class CMDRouter{
    public:
        CMDRouter(const char * deviceName, AsyncComm* cmdSerial1, AsyncComm* cmdSerial2): deviceName(deviceName), cmdSerial1(cmdSerial1), cmdSerial2(cmdSerial2){}
        void setup_hook(){
            cmdSerial1->setup_hook();
            cmdSerial2->setup_hook();
        }
        void loop_hook(){
            cmdSerial1->loop_hook();
            cmdSerial2->loop_hook();
            if(cmdSerial1->receivedMsg() && !cmdSerial1->ismsgProcessedSuccessfully()){
                cmdSerial1->logInfo("Routing message from cmdSerial1 to cmdSerial2");
                cmdSerial2->sendMsg(cmdSerial1->getDoc());
            }
            else if(cmdSerial2->receivedMsg() && !cmdSerial2->ismsgProcessedSuccessfully()){
                cmdSerial1->logInfo("Routing message from cmdSerial2 to cmdSerial1");
                cmdSerial1->sendMsg(cmdSerial2->getDoc());
            }
        }
    private:
        AsyncComm * cmdSerial1;
        AsyncComm * cmdSerial2;
        const char * deviceName;
};

#endif // CMD_ROUTING_H
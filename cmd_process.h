#include <Arduino.h>

using namespace std;

class CMD {
public:
    int cmd;
    void* data; //usually arguments struct - typecast
    /*
    Internal process states
    */
    //hopefully optimized
    //consider int and bit masks
    bool cmd_started; // cmd processing started
    bool cmd_processed; // cmd successfully parsed
    bool cmd_executed; //cmd executed successfully
    bool cmd_set; //cmd code recognized
    bool cmd_data_processed; // additional data successfully parsed

    static const char ENDLINE = '\n';
    CMD(): cmd(-1), data(NULL), cmd_started(false), cmd_processed(false) {}
    void clean(){
        cmd = -1;
        data = NULL;
        cmd_started = false;
        cmd_processed = false;
        endline_reached = false;
    }
}
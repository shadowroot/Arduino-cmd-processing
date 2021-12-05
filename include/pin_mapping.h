#include <Arduino.h>
#include <string.h>

#define BOARD_TYPE_UNO 1

typedef struct pin_map{
    const char * pin_name;
    uint8_t pin;
} pin_map;


#ifdef BOARD_TYPE_UNO
const pin_map [] pin_mapping = {
    {"A0", A0},
    {"A1", A1},
    {"A2", A2},
    {"A3", A3},
    {"A4", A4},
    {"A5", A5},
    {"D0", D0},
    {"D1", D1},
    {"D2", D2},
    {"D3", D3},
    {"D4", D4},
    {"D5", D5},
    {"D6", D6},
    {"D7", D7},
    {"D8", D8},
    {"D9", D9},
    {"D10", D10},
    {"D11", D11},
    {"D12", D12},
    {"D13", D13}
};
#endif

/*
 * 
*/
int get_pin(const char * pin_desc, uint8_t * pin_mapped){
    for(uint8_t i=0; i < sizeof(pin_mapping); i++){
        if(strncmp(pin_desc, pin_mapping[i].pin_name, strlen(pin_mapping[i].pin_name) == 0){
            *pin_mapped = pin_mapping[i].pin;
            return 0;
        }
    }
    return -1;
}


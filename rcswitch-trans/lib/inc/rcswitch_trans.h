//rcswitch_trans.h


#ifndef _rc_switch_
#define _rc_switch_

#include "stm8s.h"
#include "delay.h"

#define LOW 1
#define HIGH 0

void switchOn_2(int nGroupNumber, int nSwitchNumber);
void switchOff_2(int nGroupNumber, int nSwitchNumber);
void switchOn_0(const char* sGroup, int nSwitchNumber);
void switchOff_0(const char* sGroup, int nSwitchNumber);
void switchOn_1(char sFamily, int nGroup, int nDevice);
void switchOff_1(char sFamily, int nGroup, int nDevice);
void switchOn_4(const char* sGroup, const char* sDevice);
void switchOff_4(const char* sGroup, const char* sDevice);
void switchOn_3(char sGroup, int nDevice);
void switchOff_3(char sGroup, int nDevice);

void sendTriState(const char* sCodeWord);
void send(unsigned long code, unsigned int length);
void _send_(const char* sCodeWord);
    



void enableTransmit(int nTransmitterPin);
void disableTransmit();
void setPulseLength(int nPulseLength);
void setRepeatTransmit(int nRepeatTransmit);


struct HighLow {
     uint8_t high;
     uint8_t low;
};

struct Protocol {
    int pulseLength;
    struct HighLow syncFactor;
    struct HighLow zero;
    struct HighLow one;
        /** @brief if true inverts the high and low logic levels in the HighLow structs */
    bool invertedSignal;
};

void setProtocol(int nProtocol);

char* getCodeWordA(const char* sGroup, const char* sDevice, bool bStatus);
char* getCodeWordB(int nGroupNumber, int nSwitchNumber, bool bStatus);
char* getCodeWordC(char sFamily, int nGroup, int nDevice, bool bStatus);
char* getCodeWordD(char group, int nDevice, bool bStatus);
void transmit(struct HighLow pulses);


//int nTransmitterPin;
//int nRepeatTransmit;
    
//struct Protocol protocol;


const static unsigned int nSeparationLimit;
    /* 
     * timings[0] contains sync timing, followed by a number of bits
     */

 



#endif
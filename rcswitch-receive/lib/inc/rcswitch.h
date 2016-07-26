//rcswitch.h

#ifndef _rc_switch_
#define _rc_switch_

#include "stm8s.h"
#include "stm8s_itc.h"
#include "stm8s_uart2.h"
#include "micros.h"

#define RCSWITCH_MAX_CHANGES 67


void RCSwitch();
void enableReceive(int interrupt);
void _enableReceive();
void disableReceive();
bool available();
void resetAvailable();
unsigned long getReceivedValue();
unsigned int getReceivedBitlength();
unsigned int getReceivedDelay();
unsigned int getReceivedProtocol();
unsigned int* getReceivedRawdata();
void setReceiveTolerance(int nPercent);
void setProtocol(int nProtocol);
static void handleInterrupt();
static bool receiveProtocol(const int p, unsigned int changeCount);


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


static int nReceiveTolerance;
static unsigned long nReceivedValue;
static unsigned int nReceivedBitlength;
static unsigned int nReceivedDelay;
static unsigned int nReceivedProtocol;
const static unsigned int nSeparationLimit = 4600;
/* 
* timings[0] contains sync timing, followed by a number of bits
*/
static unsigned int timings[RCSWITCH_MAX_CHANGES];

    

#endif
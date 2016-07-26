//rcswitch.cpp


#include "rcswitch.h"
#include "stm8s_exti.h"
  
int nReceiverInterrupt;
struct Protocol protocol;
static struct Protocol proto[] = {
  { 350, {  1, 31 }, {  1,  3 }, {  3,  1 }, FALSE },    // protocol 1
  { 650, {  1, 10 }, {  1,  2 }, {  2,  1 }, FALSE },    // protocol 2
  { 100, { 30, 71 }, {  4, 11 }, {  9,  6 }, FALSE },    // protocol 3
  { 380, {  1,  6 }, {  1,  3 }, {  3,  1 }, FALSE },    // protocol 4
  { 500, {  6, 14 }, {  1,  2 }, {  2,  1 }, FALSE },    // protocol 5
  { 450, { 23,  1 }, {  1,  2 }, {  2,  1 }, TRUE }      // protocol 6 (HT6P20B)
};

enum {
   numProto = sizeof(proto) / sizeof(proto[0])
};

void RCSwitch() {
  setProtocol(1);
  nReceiverInterrupt = -1;
  setReceiveTolerance(60);
  nReceivedValue = 0;
}
  
  
void enableReceive(int interrupt) {
  nReceiverInterrupt = interrupt;
  _enableReceive();
}


void _enableReceive() {
  if (nReceiverInterrupt != -1) {
    nReceivedValue = 0;
    nReceivedBitlength = 0;
    EXTI_SetExtIntSensitivity( EXTI_PORT_GPIOA , EXTI_SENSITIVITY_RISE_FALL ); 
    ITC_SetSoftwarePriority(ITC_IRQ_PORTA, ITC_PRIORITYLEVEL_2);
    __enable_interrupt();
  }
}
  

void setProtocol(int nProtocol) {
  if (nProtocol < 1 || nProtocol > numProto) {
    nProtocol = 1;  // TODO: trigger an error, e.g. "bad protocol" ???
  }
  protocol = proto[nProtocol-1];

}


void setReceiveTolerance(int nPercent) {
  nReceiveTolerance = nPercent;
}


bool available() {
  return nReceivedValue != 0;
}


unsigned long getReceivedValue() {
  return nReceivedValue;
}


unsigned int getReceivedBitlength() {
  return nReceivedBitlength;
}


unsigned int getReceivedProtocol() {
  return nReceivedProtocol;
}


void resetAvailable() {
  nReceivedValue = 0;
}

long int diff(uint32_t value1, uint32_t value2)
{
  long int val = value1 - value2;
  if(val < 0){
    val = -val;
  }
  return val;
}


//-------------------------------------------------------------------------------------------------------------------



bool  receiveProtocol(const int p, unsigned int changeCount) {
/*
#ifdef ESP8266
    const Protocol &pro = proto[p-1];
#else
    Protocol pro;
    memcpy_P(&pro, &proto[p-1], sizeof(Protocol));
#endif
*/  
    struct Protocol pro = proto[p-1];
    unsigned long code = 0;
    //Assuming the longer pulse length is the pulse captured in timings[0]
    const unsigned int syncLengthInPulses =  ((pro.syncFactor.low) > (pro.syncFactor.high)) ? (pro.syncFactor.low) : (pro.syncFactor.high);
    const unsigned int delay = timings[0] / syncLengthInPulses;
    const unsigned int delayTolerance = delay * nReceiveTolerance / 100;
    
    /* For protocols that start low, the sync period looks like
     *               _________
     * _____________|         |XXXXXXXXXXXX|
     *
     * |--1st dur--|-2nd dur-|-Start data-|
     *
     * The 3rd saved duration starts the data.
     *
     * For protocols that start high, the sync period looks like
     *
     *  ______________
     * |              |____________|XXXXXXXXXXXXX|
     *
     * |-filtered out-|--1st dur--|--Start data--|
     *
     * The 2nd saved duration starts the data
     */
    const unsigned int firstDataTiming = (pro.invertedSignal) ? (2) : (1);

    for (unsigned int i = firstDataTiming; i < changeCount - 1; i += 2) {
        code <<= 1;
        if (diff(timings[i], delay * pro.zero.high) < delayTolerance &&
            diff(timings[i + 1], delay * pro.zero.low) < delayTolerance) {
            // zero
        } else if (diff(timings[i], delay * pro.one.high) < delayTolerance &&
                   diff(timings[i + 1], delay * pro.one.low) < delayTolerance) {
            // one
            code |= 1;
        } else {
            // Failed
            return FALSE;
        }
    }

    if (changeCount > 7) {    // ignore very short transmissions: no device sends them, so this must be noise
        nReceivedValue = code;
        nReceivedBitlength = (changeCount - 1) / 2;
        nReceivedDelay = delay;
        nReceivedProtocol = p;
    }

    return TRUE;
}


/*
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  const long time = micros;
}
*/


INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  static unsigned int changeCount = 0;
  static unsigned long lastTime = 0;
  static unsigned int repeatCount = 0;
  static int i = 0;
  const long time = micros; 
 


  
  const unsigned int duration = time - lastTime;

  if (duration > nSeparationLimit) {
    
    

    
    
    // A long stretch without signal level change occurred. This could
    // be the gap between two transmission.
    if (diff(duration, timings[0]) < 200) {
    
      
    if (i == 0)
    {
      GPIO_WriteHigh(GPIOB, GPIO_PIN_6);
      i = 1;
    }
    else
    {
      GPIO_WriteLow(GPIOB, GPIO_PIN_6);
      i = 0;
    }
    

      // This long signal is close in length to the long signal which
      // started the previously recorded timings; this suggests that
      // it may indeed by a a gap between two transmissions (we assume
      // here that a sender will send the signal multiple times,
      // with roughly the same gap between them).
      repeatCount++;
      if (repeatCount == 2) {
        for(unsigned int i = 1; i <= numProto; i++) {
          if (receiveProtocol(i, changeCount)) {
            // receive succeeded for protocol i
            break;
          }
        }
        repeatCount = 0;
      }
    }
    changeCount = 0;
  }
 
  // detect overflow
  if (changeCount >= RCSWITCH_MAX_CHANGES) {
    changeCount = 0;
    repeatCount = 0;
  }
  
   timings[changeCount++] = duration;
   lastTime = time;  
}

  
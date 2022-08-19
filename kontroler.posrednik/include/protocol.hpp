#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <Arduino.h>


#define MAXLENPROTO 17

#include "../../kontroler_lib/crc8.h"

#define POSREDNIK 1
#include "../../kontroler_lib/protocol_kontroler.hpp"

class MessageSerial : public MessageSerialBase
{
    public:
    MessageSerial();

    typedef enum _work {
        NOP,
        GET_RADIOVAL,
        WELCOME_MSG,
    } Work;

    bool check(unsigned char c);
    void init();
    Work getStatusWork() const { return actWork; }
    void reset();

    void sendRadioVal(uint16_t val1, uint16_t val2, uint16_t val3, uint16_t val4);
    void sendRadioError(uint8_t val);
    void sendRadioDebug(uint16_t val);

    void sendWelcomeMsg();
protected:



    bool isWork() { 
        return actWork != NOP; 
    }
protected:
    

    bool parseRozkaz();
    void sendMessage(uint8_t cmd, uint8_t *buf, uint8_t len);
    
    private :
    // rozkaz/dlugosc | 1 byte | 2 byte | 3 byte | 4 byte | crc
    uint8_t data[MAXLENPROTO + 4];

    Work actWork;
    uint8_t posCmd;
    uint8_t rozkaz;
    uint8_t dlugosc;
    CRC8 crc;

    uint32_t posY;
    uint32_t posX;

    bool wasWelcomeMsg;
    unsigned long timeSendRadioVal;

    uint8_t param1[17];
    uint8_t param2[17];
};

    

    

#endif // !_PROTOCOL_H_
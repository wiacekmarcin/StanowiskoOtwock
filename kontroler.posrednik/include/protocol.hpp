#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <Arduino.h>
#include "crc8.h"



#define MAXLENPROTO 17

// |      HEAD       | n1 | n2 | .... | n15 | CRC8
// | CMD 4b | LEN 4b |
// LEN(MSG) = 1 (HEAD) + LEN + 1 (CRC8)

// getVersion
// 0x10 CRC8 - req
// 0x2f [15B] CRC8 - rep

//set parameters
//0x3a REVERSEBTE X4 X3 X2 X1 CRC8- ustawianie parametrow  
//0x40 CRC8 - ACK

//setPos 
//0x58 X4 X3 X2 X1 CRC8 - req
//0x61 S/K CRC8 - reply setting position in proges S=start, K=end

//echo 
//0x70 CRC8 - req echo
//0x80 CRC8 - reply echo

//set home position
//0x90 CRC8 - req 
//0xa1 s/K CRC8 - s=start, K=stop

//0xb0 CRC8 -reset req
//0xc0 CRC8 - reply req


// setError
//0xfx MSG CRC8 - rep






class Message {
    public:
    typedef enum _cmd {
        NOP_REQ = 0,
        WELCOME_REQ = 1,
        WELCOME_REP = 2,
        SET_PARAM_REQ = 3,
        SET_PARAM_REP = 4,
        POSITION_REQ = 5,
        POSITION_REP = 6,
        ECHO_REQ = 7,
        ECHO_REP = 8,
        MOVEHOME_REQ = 9,
        MOVEHOME_REP = 10,
        RESET_REQ = 11,
        RESET_REP = 12,
        ERROR_REP = 14,
        NOP_REP = 15,
    } CMD;

    typedef enum _work {
        NOP,
        POS_START,
        //POS_DONE,
        RETURN_HOME,
        //RETURN_DONE,
        SET_PARAM,
        ECHO,
    } Work;

    void init();
    bool check1(unsigned char c);
    bool check2(unsigned char c);
    Work getStatusWork() const;
    void reset();

protected:

    bool parse(uint8_t serial) { return (serial == 0 ? parse1() : parse2()); };
    bool parse1();
    bool parse2();
    bool check(uint8_t serial, unsigned char c);

    void sendMessage(uint8_t serial, uint8_t cmd, uint8_t *buf, uint8_t len);
    void sendMessage1(uint8_t cmd, uint8_t *buf, uint8_t len) { sendMessage(0, cmd, buf, len); };
    void sendMessage2(uint8_t cmd, uint8_t *buf, uint8_t len) { sendMessage(1, cmd, buf, len); };
    
    void sendRawMessage(uint8_t serial, uint8_t *buf, uint8_t len);
    void sendRawMessage1(uint8_t *buf, uint8_t len) { sendRawMessage(0, buf, len); };
    void sendRawMessage2(uint8_t *buf, uint8_t len) { sendRawMessage(1, buf, len); };
    

    void messageWrite1(uint8_t *buf, uint8_t len);
    void messageWrite2(uint8_t *buf, uint8_t len);

    void sendError(uint8_t *buf, uint8_t len) { sendMessage1(ERROR_REP, buf, len); }; 
    void sendError(const char *buf, uint8_t len);

    uint32_t parseNumber(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
    
    // rozkaz/dlugosc | 1 byte | 2 byte | 3 byte | 4 byte | crc
    uint8_t data[2][MAXLENPROTO + 4];

    uint8_t posCmd[2];
    uint8_t rozkaz[2];
    uint8_t dlugosc[2];
    CRC8 crc[2];

    Work actWork;
};

    

    

#endif // !_PROTOCOL_H_
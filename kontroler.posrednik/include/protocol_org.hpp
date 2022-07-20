#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <Arduino.h>
#include "CRC8.h"



#define MAXLENPROTO 17

// |      HEAD       | n1 | n2 | .... | n15 | CRC8
// | CMD 4b | LEN 4b |
// LEN(MSG) = 1 (HEAD) + LEN + 1 (CRC8)

// getVersion
// 0x10 CRC8 - req
// 0x2f [15B] CRC8 - rep

// setError
//0x4f nrE CRC8 - rep

//setPos X,Y
//0x58 X4 X3 X2 X1 Y4 Y3 Y2 Y1 CRC8 - req
//0x61 S/l/P/d/G/K CRC8 - reply setting position in proges S=start, l=start lewoprawo, P=end lewoprawo, d=start goradol G=end goradol K=endboth

//getCzujVal
//0x70 CRC8 - req
//0x89 'O' X2 X1 Y2 Y1 W2 W1 Z2 Z1 CRC8 - ok, wartosci odczytane z radia
//0x81 'E' CRC8 - error polaczenia z radiem

//setUnitVal
//0x92 R 1/2/3/4 float1 float2 float3 float4 CRC8 wartość wspolczynnika mnożonego który będzie pokazywany
//0x92 U 1/2/3/4 U1 U2 U3 U4 U5 U6 U7 U8 U9 U10 CRC8 - 1/2/3/4 - nr kanalu, ujednostka pokazywana
//0xA0 CRC8 - ok 
//0xA1 'E' CRC8 - error

//set home position
//0xB0 CRC8 - req 
//0xC1 s/L/D/p/g/K CRC8 - s=start, p=start lewoprawo, L=stop lewoprawo, g=start goradol D=stop goradol K=stop all

//set parameters
//0x3a 0x01 STATUSBYTE X4 X3 X2 X1 Y4 Y3 Y2 Y1 CRC8- ustawianie parametrow 
//0x39 0x02 X4 X3 X2 X1 Y4 Y3 Y2 Y1 CRC8- ustawianie parametrow 
//0x40 CRC8 - ACK

class MessageSerial {
public:
    MessageSerial();

    typedef enum _work {
        NOP,
        POS_START,
        POS_X,
        POS_Y,
        POS_DONE,
        GET_RADIOVAL,
        RETURN_HOME,
        RETURN_DONE,
    } Work;

    typedef enum _cmd {
        WELCOME_REQ = 1,
        WELCOME_REP = 2,
        SET_PARAM_REQ = 3,
        SET_PARAM_REP = 4,
        POSITION_REQ = 5,
        POSITION_REP = 6,
        MEASVALUE_REQ = 7,
        MEASVALUE_REP = 8,
        MEASUNIT_REQ = 9,
        MEASUNIT_REP = 10,
        MOVEHOME_REQ = 11,
        MOVEHOME_REP = 12,
        
        ERROR_REP = 15,
    } CMD;

    bool check(unsigned char c);

    void init();

    Work getStatusWork() const { return actWork; }

    
    
    
    void sendPositionStart();
    void sendPositionStartX();
    void sendPositionDoneX(uint32_t step, uint32_t pos);

    void sendPositionStartY();
    void sendPositionDoneY(uint32_t step, uint32_t pos);
    
    void sendPositionDone();

    void sendRadioVal(uint16_t val1, uint16_t val2, uint16_t val3, uint16_t val4); 
    void sendRadioError();
    void sendRadioDebug(uint16_t val1);   

    void sendRetHomeStart();
    void sendRetHomeYDone(uint32_t step);
    void sendRetHomeXDone(uint32_t step);
    void sendRetHomeYStart();
    void sendRetHomeXStart();
    void sendRetHomeDone();

    void sendAckSettings(uint8_t nr);

    void sendError(short errNum);

    void setErrorHomeBack();

    void setStop() { actWork != NOP; }

    uint32_t getPosX() const { return posX; }
    uint32_t getPosY() const { return posY; }

    bool isWelcomeMsg() {
        return rozkaz == WELCOME_REQ;
    }

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
};

#endif // !_PROTOCOL_H_
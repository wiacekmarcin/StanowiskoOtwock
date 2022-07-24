#include <Arduino.h>
#include "../../kontroler.lib/crc8.h"
#define MAXLENPROTO 17

// |      HEAD       | n1 | n2 | .... | n15 | CRC8
// | CMD 4b | LEN 4b |
// LEN(MSG) = 1 (HEAD) + LEN(DATA) + 1 (CRC8)


// 00 - czyszczenie bufora

//welcome msg
// 10 70 - req
// 2f 4b 4f 4e 54 52 4f 4c 45 52 57 49 41 54 52 32 dc - rep 
//    K  O  N  T  R  O  L  E  R  W  I  A  T  R  2

//setPos X,Y or setPos R
//59 50 X4 X3 X2 X1 Y4 Y3 Y2 Y1 CRC8 - req ustawienie pozycji
//Y  P
//55 52 R4 R3 R2 R1 CRC8 - req ustawienie rolety 
//U  R

//61 S/l/P/d/G/K/R/r/  CRC8 - reply setting position in proges S=start, l=start lewoprawo, P=end lewoprawo, 
//                            d=start goradol G=end goradol K=endboth, R - start rolet, r - koniec rolet
//62 E X/Y/R  CRC8 - reply error setting position X - os x, Y - os y, R - rolety

//61 53 5e - start ustawiania pozycji ogolnie
//a  S  ^
//61 63 e3 - start ustawianie lewo-prawo
//a  l  
//61 50 57 - koniec ustawiania lewo-prawo
//a  P  W
//61 64 db - poczatek ustawianai dol-gora
//a  d  
//61 47 32 - koniec ustawienia dol-gora
//a  G  2
//61 4b 16 - koniec wszystkiego
//a  K   
//61 52 59 - start ustawiania rolety
//a  R  Y
//61 72 b9 - koniec ustawiania rolety
//a  r  

//62 45 58 3b  - blad przy pozycjonowaniu osi X
//a  E  X  ;  
//62 45 59 3c  - blad przy pozycjonowaniu osi Y
//a  E  Y  <  
//62 45 52 b0  - blad przy pozycjonowaniu osi R
//a  E  R    

//set home position
//71 50 00 - req home position
//q  P
//71 52 0e - req home roleta
//q  R 

//81 50 14 - rep home position
//   P

//81 52 1a - rep home position
//   R

//set parameters
//3a 01 STATUSBYTE IMPX4 IMPX3 IMPX2 IMPX1 IMPY4 IMPY3 IMPY2 IMPY1 CRC8- ustawianie parametrow 
// STATUSBYTE = 0 0 0 0 0 RevR RevY RevX

//3c 02 STEPX4 STEPX3 STEPX2 STEPX1 STEPY4 STEPY3 STEPY2 STEPY1 R4 R3 R2 R1 CRC8- ustawianie parametrow maksymalne kroki silnikow


//41 01 49 - ACK
//0x4f nrE CRC8 - rep







//0xC1 s/L/D/p/g/K CRC8 - s=start, p=start lewoprawo, L=stop lewoprawo, g=start goradol D=stop goradol K=stop all




// setError



//getCzujVal
//0x70 CRC8 - req
//0x89 'O' X2 X1 Y2 Y1 W2 W1 Z2 Z1 CRC8 - ok, wartosci odczytane z radia
//0x81 'E' CRC8 - error polaczenia z radiem

//setUnitVal
//0x92 R 1/2/3/4 float1 float2 float3 float4 CRC8 wartość wspolczynnika mnożonego który będzie pokazywany
//0x92 U 1/2/3/4 U1 U2 U3 U4 U5 U6 U7 U8 U9 U10 CRC8 - 1/2/3/4 - nr kanalu, ujednostka pokazywana
//0xA0 CRC8 - ok 
//0xA1 'E' CRC8 - error



//set parameters
//0x3a 0x01 STATUSBYTE X4 X3 X2 X1 Y4 Y3 Y2 Y1 CRC8- ustawianie parametrow 
//0x39 0x02 X4 X3 X2 X1 Y4 Y3 Y2 Y1 CRC8- ustawianie parametrow 
//0x40 CRC8 - ACK

class MessageSerialBase {
public:
    MessageSerialBase();

    typedef enum _work {
        NOP,
        POS_START,
        POS_X,
        POS_Y,
        POS_DONE,
        RETURN_HOME,
        RETURN_DONE,
        ROL_START,
        ROL_HOME,
        ROL_DONE,
    } Work;

    typedef enum _cmd {
        NOP_MSG = 0,
#ifdef POSREDNIK
        WELCOME_REQ = 1,
        WELCOME_REP = 2,
#endif // DEBUG                
        SET_PARAM_REQ = 3,
        SET_PARAM_REP = 4,
        POSITION_REQ = 5,
        POSITION_REP = 6,
        MOVEHOME_REQ = 7,
        MOVEHOME_REP = 8,
#ifdef POSREDNIK        
        MEASVALUE_REQ = 9,
        MEASVALUE_REP = 10,
        MEASUNIT_REQ = 11,
        MEASUNIT_REP = 12,
#endif        
        ERROR_REP = 15,
    } CMD;

    bool check(unsigned char c);

    void init();

    Work getStatusWork() const { return actWork; }

    void sendError(const char * errorStr);

    void setErrorHomeBack();

    void setStop() { actWork != NOP; }

#ifdef POSREDNIK 
    bool isWelcomeMsg() {
        return rozkaz == WELCOME_REQ;
    }
#endif

    bool isWork() { 
        return actWork != NOP; 
    }

    void sendRadioError();
    void sendRadioDebug(uint16_t val);
    void sendRadioVal(uint16_t val1, uint16_t val2, uint16_t val3, uint16_t val4);
protected:
    

    bool parseRozkaz();
    void sendMessage(uint8_t cmd, uint8_t *buf, uint8_t len);
    
    // rozkaz/dlugosc | 1 byte | 2 byte | 3 byte | 4 byte | crc
    uint8_t data[MAXLENPROTO + 4];

    Work actWork;

#ifdef SILNIK    
    uint8_t posCmd;
    uint8_t rozkaz;
    uint8_t dlugosc;
    CRC8 crc;
#endif



#ifdef POSREDNIK 
    bool wasWelcomeMsg;
    uint32_t timeSendRadioVal;
#endif    
};
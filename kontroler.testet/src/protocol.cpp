#include "protocol.hpp"

#include "../../kontroler_lib/protocol_kontroler.cpp"
#include "../../kontroler_lib/crc8.cpp"


MessageSerial::MessageSerial() 
{
    posCmd = 0;
    rozkaz = 0;
    dlugosc = 0;
    crc.restart();
}

void MessageSerial::init() 
{
    MessageSerialBase::init();
    Serial.begin(115200); 
}

bool MessageSerial::check(unsigned char c)
{
    data[posCmd++] = c;
    data[posCmd] = '\0';
  
    if (posCmd-1 == 0) {    
        crc.restart();
        crc.add(data[0]);
        rozkaz = data[0] >> 4;
        dlugosc = data[0] & 0x0f;
        return false;
    }
    
    if (posCmd == dlugosc + 2) {
        uint8_t c = crc.getCRC();
        if (data[posCmd-1] == c) {
            posCmd = 0;
            bool r = parseRozkaz();
            if (!r) {
                sendError("ZLY ROZKAZ");
            }
            return r;
        }
        posCmd = 0;
        sendError("ZLE CRC");
        return false;

    }

    crc.add(data[posCmd-1]);    
    
    
    if (posCmd == MAXLENPROTO) {
        posCmd = 0;
        sendError("ZBYT DUZA WIAD");
        return false;    
    }
    return false;
}

void MessageSerial::sendMessage(uint8_t cmd, uint8_t* buf, uint8_t len)
{
    if (len > 15)
        return;
    
    uint8_t sendData[MAXLENPROTO] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if (len > 0)
        memcpy(sendData+1, buf, len);
    sendData[0] = cmd << 4 | len ;
    crc.restart();
    crc.add(sendData, len+1);
    sendData[len+1] = crc.getCRC();
    Serial.write(sendData, len+2);
}

bool MessageSerial::parseRozkaz()
{
    
    switch(rozkaz) {
        case WELCOME_REQ:   //get info 
        {  
            actWork = WELCOME_MSG;
            return true;
        }

        case POSITION_REQ: 
        {
            if (dlugosc >= 1 && data[1] == 'P') {
                sendPosiotionMessages(false);
            } else if (dlugosc >= 1 && data[1] == 'R') {
                sendRolPositionMessages(false);
            }

            actWork=NOP;
            return true;
        }
        case MOVEHOME_REQ:
        {
            if (dlugosc >= 1 && data[1] == 'P') {
                sendPosiotionMessages(true);
            } else if (dlugosc >= 1 && data[1] == 'R') {
                sendRolPositionMessages(true);
            }
            actWork=NOP;
            return true;
        }
        case SET_PARAM_REQ:
        {
            sendAckSettings(data[1]);
            actWork=NOP;
            return true;
        }
        case NOP_MSG:
        {
            Serial.write(0xF0);
            return true;
        }
        case MEASVALUE_REQ:
        {
            sendRadioVal(0,0,0,0);
            actWork = NOP;

            return true;
        }
        case MEASUNIT_REQ:
        {
            return true;
        }
        default:
            break;

    }
    rozkaz = 0;
    return false;
}


void MessageSerial::sendRadioVal(uint16_t val1, uint16_t val2, uint16_t val3, uint16_t val4)
{
 

    //sendRadioDebug(val1);

    actWork = NOP;
    uint8_t sendData[9] = {'O', 0, 0, 0, 0, 0, 0, 0, 0};
    sendData[1] = (val1 >> 8 ) & 0xff;
    sendData[2] = (val1) & 0xff;
    sendData[3] = (val2 >> 8 ) & 0xff;
    sendData[4] = (val2) & 0xff;
    sendData[5] = (val3 >> 8 ) & 0xff;
    sendData[6] = (val3) & 0xff;
    sendData[7] = (val4 >> 8 ) & 0xff;
    sendData[8] = (val4) & 0xff;
    sendMessage(MEASVALUE_REP, sendData, 9);

}

void MessageSerial::sendRadioError(uint8_t val)
{
     

    uint8_t sendData[2] = {'E', '0'+val};
    sendMessage(MEASVALUE_REP, sendData, 2);
 
}

void MessageSerial::sendRadioDebug(uint16_t val)
{
    uint8_t sendData[12] = {'I', ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned char c1 = val / 100000;
    unsigned int val1 = val - c1 * 100000;
    sendData[2] = '0' + c1;

    unsigned char c2 = val1 / 10000;
    unsigned int val2 = val1 - c2 * 10000;
    sendData[3] = '0' + c2;

    unsigned char c3 = val2 / 1000;
    unsigned int val3 = val2 - c3 * 1000;
    sendData[4] = '0' + c3;

    unsigned char c4 = val3 / 100;
    unsigned int val4 = val3 - c4 * 100;
    sendData[5] = '0' + c4;

    unsigned char c5 = val4 / 10;
    unsigned int val5 = val4 - c5 * 10;
    sendData[6] = '0' + c5;

    sendData[7] = '0' + val5;

    sendData[8] = ' ';
    sendData[9] = 'm';
    sendData[10] = 'V';
    sendData[11] = ' ';


    sendMessage(MEASVALUE_REP, sendData, 12);
}

void MessageSerial::sendWelcomeMsg() 
{
    uint8_t sendData[15] = {'K','O','N','T','R','O','L','E','R','W','I','A','T','R', '2'};
    sendMessage(WELCOME_REP, sendData, 15);
}

void MessageSerial::sendPosiotionMessages(bool home)
{

    uint8_t sendData1[1] = {'s'};
    sendMessage(home ? MOVEHOME_REP : POSITION_REP, sendData1, 1);
    delay(500);

    uint8_t sendData2[1] = {'l'};
    sendMessage(home ? MOVEHOME_REP : POSITION_REP, sendData2, 1);
    delay(10000);

    uint8_t sendData3[9] = {'P', 0, 0, 0, 0, 0, 0, 0, 0};
    sendMessage(home ? MOVEHOME_REP : POSITION_REP, sendData3, home ? 5 : 9);
    delay(500);

    uint8_t sendData4[1] = {'d'};
    sendMessage(home ? MOVEHOME_REP : POSITION_REP, sendData4, 1);
    delay(10000);

    uint8_t sendData5[9] = {'G', 0, 0, 0, 0, 0, 0, 0, 0};
    sendMessage(home ? MOVEHOME_REP : POSITION_REP, sendData5, home ? 5 : 9);
    delay(500);

    uint8_t sendData6[1] = {'K'};
    sendMessage(home ? MOVEHOME_REP : POSITION_REP, sendData6, 1);

}

void MessageSerial::sendRolPositionMessages(bool home) 
{
    uint8_t sendData1[1] = {'r'};
    sendMessage(home ? MOVEHOME_REP : POSITION_REP, sendData1, 1);
    delay(5000);

    uint8_t sendData2[9] = {'R', 0, 0, 0, 0, 0, 0, 0, 0};
    sendMessage(home ? MOVEHOME_REP : POSITION_REP, sendData2, home ? 5 : 9);
}

void MessageSerial::sendAckSettings(uint8_t nr) 
{
    uint8_t sendData[1] = {nr};
    sendMessage(SET_PARAM_REP, sendData, 1);
}
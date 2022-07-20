#include "protocol.hpp"

//10 imp silnika na 1 mm
#include "platform.h"

extern bool reverseY;
extern bool reverseX;

extern int32_t gImpMaxX;
extern int32_t gImpMaxY;

extern int32_t gStepMaxX;
extern int32_t gStepMaxY;

MessageSerial::MessageSerial()
{
    actWork = NOP;
    posCmd = 0;
    crc.restart();
    posY = 0;
    posX = 0;

    wasWelcomeMsg = false;
}

void MessageSerial::init() {
    Serial1.begin(115200);
    actWork = NOP;
}

bool MessageSerial::check(unsigned char c)
{
    data[posCmd++] = c;

    Serial.print("NOWY ZNAK=");
    Serial.println(c, HEX);
    if (posCmd-1 == 0) {    
        crc.restart();
        crc.add(data[0]);
        rozkaz = data[0] >> 4;
        dlugosc = data[0] & 0x0f;
        Serial.print("ROZKAZ=");
        Serial.println(rozkaz,DEC);
        Serial.print("LEN=");
        Serial.println(dlugosc, DEC);
        return false;
    }
    
    
    if (posCmd == dlugosc + 2) {
        uint8_t c = crc.getCRC();
        Serial.print("CRC=");
        Serial.print(c,HEX);
        Serial.print("==");
        Serial.println(data[posCmd-1],HEX);
        if (data[posCmd-1] == c) {
            posCmd = 0;
            bool r = parseRozkaz();
            if (!r)
                Serial.println("ZLY ROZKAZ");
            return r;
        }
        posCmd = 0;
        Serial.print("CRC FAILD");
        return false;

    }

    crc.add(data[posCmd-1]);    
    
    
    if (posCmd == MAXLENPROTO) {
        posCmd = 0;
        Serial.println("ZBYT DUZA WIADOMOSC");
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
    Serial1.write(sendData, len+2);

}

bool MessageSerial::parseRozkaz()
{
    
    switch(rozkaz) {
        
        // get info |HEAD| CRC
        case WELCOME_REQ:   //get info 
        {                          //1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  
            uint8_t sendData[15] = {'K','O','N','T','R','O','L','E','R','W','I','A','T','R', 'U'};
            sendMessage(WELCOME_REP, sendData, 15);
            actWork = NOP;
            wasWelcomeMsg = true;
            return true;
        }
        case POSITION_REQ: 
        {
            posY = 0;
            for (short i = 0; i < 4; ++i) {
                posY = (posY << 8) + (data[i+1] & 0xff);
            }
            
            posX = 0;
            for (short i = 0; i < 4; ++i) {
                posX = (posX << 8) + (data[4+i+1] & 0xff);
            }
            actWork = POS_START;
            return true;
        }
        case SET_PARAM_REQ:
        {
            actWork = NOP;
            unsigned short nrU = data[1];
            Serial.print("NR=");
            Serial.println(nrU, DEC);
            if (nrU == 1) {
                unsigned short statusByte = data[2] & 0xff;
                Serial.print("STATUSBYTE=");
                Serial.println(statusByte, HEX);

                reverseY = 0x02 & statusByte;
                reverseX = 0x01 & statusByte;

                Serial.print("REVERSEGORA=");
                Serial.println(reverseY, DEC);

                Serial.print("REVERSEDOL=");
                Serial.println(reverseX, DEC);

                int32_t maxX = 0;
                for (uint8_t i = 0; i < 4; i++) {
                    maxX = (maxX << 8) + (data[3+i] & 0xff);
                }

                int32_t maxY = 0;
                for (uint8_t i = 0; i < 4; i++) {
                    maxY = (maxY << 8) + (data[7+i] & 0xff);
                }

                Serial.print("MAXX=");
                Serial.println(maxX, DEC);

                Serial.print("MAXY=");
                Serial.println(maxY, DEC);

                gImpMaxX = maxX;
                gImpMaxY = maxY;
                sendAckSettings(nrU);
                return true;

            } else if (nrU == 2) {
                int32_t maxX = 0;
                for (uint8_t i = 0; i < 4; i++) {
                    maxX = (maxX << 8) + (data[2+i] & 0xff);
                }

                int32_t maxY = 0;
                for (uint8_t i = 0; i < 4; i++) {
                    maxY = (maxY << 8) + (data[6+i] & 0xff);
                }

                Serial.print("MAXX=");
                Serial.println(maxX, DEC);

                Serial.print("MAXY=");
                Serial.println(maxY, DEC);

                gStepMaxX = maxX;
                gStepMaxY = maxY;
                sendAckSettings(nrU);     
                return true;
            }         
            return false;
        }
        
        default:
            break;

    }
    rozkaz = 0;
    return false;
}

void MessageSerial::sendRetHomeYDone(uint32_t step)
{
    uint8_t sendData[5] = {'D', 0, 0, 0, 0};
    sendData[1] = (step >> 24) & 0xff;
    sendData[2] = (step >> 16) & 0xff;
    sendData[3] = (step >> 8) & 0xff;
    sendData[4] = step & 0xff;
    sendMessage(MOVEHOME_REP, sendData, 5);
}

void MessageSerial::sendRetHomeXDone(uint32_t step)
{
    uint8_t sendData[5] = {'L', 0, 0, 0, 0};
    sendData[1] = (step >> 24) & 0xff;
    sendData[2] = (step >> 16) & 0xff;
    sendData[3] = (step >> 8) & 0xff;
    sendData[4] = step & 0xff;
    sendMessage(MOVEHOME_REP, sendData, 5);
}

void MessageSerial::sendRetHomeYStart()
{
    uint8_t sendData[1] = {'g'};
    sendMessage(MOVEHOME_REP, sendData, 1);
}

void MessageSerial::sendRetHomeXStart()
{
    uint8_t sendData[1] = {'p'};
    sendMessage(MOVEHOME_REP, sendData, 1);
}

void MessageSerial::sendRetHomeDone()
{
    uint8_t sendData[1] = {'K'};
    sendMessage(MOVEHOME_REP, sendData, 1);
}

void MessageSerial::sendRetHomeStart()
{
    uint8_t sendData[1] = {'s'};
    sendMessage(MOVEHOME_REP, sendData, 1);
}

void MessageSerial::setErrorHomeBack()
{
    actWork = NOP;
    uint8_t sendData[1] = {'E'};
    sendMessage(MOVEHOME_REP, sendData, 1);
}

void MessageSerial::sendPositionStart()
{
    uint8_t sendData[1] = {'s'};
    sendMessage(POSITION_REP, sendData, 1);
}

void MessageSerial::sendPositionStartX()
{
    uint8_t sendData[1] = {'l'};
    sendMessage(POSITION_REP, sendData, 1);
}
    
void MessageSerial::sendPositionDoneX(uint32_t step, uint32_t stepPos)
{
    uint8_t sendData[9] = {'P', 0, 0, 0, 0, 0, 0, 0, 0};
    sendData[1] = (step >> 24) & 0xff;
    sendData[2] = (step >> 16) & 0xff;
    sendData[3] = (step >> 8) & 0xff;
    sendData[4] = step & 0xff;
    sendData[5] = (stepPos >> 24) & 0xff;
    sendData[6] = (stepPos >> 16) & 0xff;
    sendData[7] = (stepPos >> 8) & 0xff;
    sendData[8] = stepPos & 0xff;
    sendMessage(POSITION_REP, sendData, 9);
}

void MessageSerial::sendPositionStartY()
{
    uint8_t sendData[1] = {'d'};
    sendMessage(POSITION_REP, sendData, 1);
}
    
void MessageSerial::sendPositionDoneY(uint32_t step, uint32_t stepPos)
{
    uint8_t sendData[9] = {'G', 0, 0, 0, 0, 0, 0, 0, 0};
    sendData[1] = (step >> 24) & 0xff;
    sendData[2] = (step >> 16) & 0xff;
    sendData[3] = (step >> 8) & 0xff;
    sendData[4] = step & 0xff;
    sendData[5] = (stepPos >> 24) & 0xff;
    sendData[6] = (stepPos >> 16) & 0xff;
    sendData[7] = (stepPos >> 8) & 0xff;
    sendData[8] = stepPos & 0xff;
    sendMessage(POSITION_REP, sendData, 9);
}

void MessageSerial::sendPositionDone()
{
    uint8_t sendData[1] = {'K'};
    sendMessage(POSITION_REP, sendData, 1);
}

void MessageSerial::sendError(short errNum)
{

}

void MessageSerial::sendAckSettings(uint8_t nr) 
{
    uint8_t sendData[1] = {nr};
    sendMessage(SET_PARAM_REP, sendData, 1);
}

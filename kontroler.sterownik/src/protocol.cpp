#include "protocol.hpp"

#include "../../kontroler.lib/protocol_kontroler.cpp"

//10 imp silnika na 1 mm
#include "platform.h"

extern bool reverseY;
extern bool reverseX;
extern bool reverseR;

extern int32_t gImpMaxX;
extern int32_t gImpMaxY;

extern int32_t gStepMaxX;
extern int32_t gStepMaxY;
extern int32_t gStepMaxR;

MessageSerial::MessageSerial() 
{
    posCmd = 0;
    rozkaz = 0;
    dlugosc = 0;
    crc.restart();

    posY = 0;
    posX = 0;
    rolP = 0;
}

void MessageSerial::init() 
{
    MessageSerialBase::init();
#ifdef DEBUG    
    Serial.begin(115200);
#endif    
    Serial1.begin(115200); 
    
}

bool MessageSerial::check(unsigned char c)
{
    data[posCmd++] = c;
    data[posCmd] = '\0';
#ifdef DEBUG        
    Serial.print("NOWY ZNAK=");
    Serial.println(c, HEX);
#endif    
    if (posCmd-1 == 0) {    
        crc.restart();
        crc.add(data[0]);
        rozkaz = data[0] >> 4;
        dlugosc = data[0] & 0x0f;
#ifdef DEBUG            
        Serial.print("ROZKAZ=");
        Serial.println(rozkaz,DEC);
        Serial.print("LEN=");
        Serial.println(dlugosc, DEC);
#endif        
        return false;
    }
    
    if (posCmd == dlugosc + 2) {
        uint8_t c = crc.getCRC();
#ifdef DEBUG            
        Serial.print("CRC=");
        Serial.print(c,HEX);
        Serial.print("==");
        Serial.println(data[posCmd-1],HEX);
#endif        
        if (data[posCmd-1] == c) {
            posCmd = 0;
            bool r = parseRozkaz();
            if (!r) {
                sendError("ZLY ROZKAZ");
#ifdef DEBUG                    
                Serial.println("ZLY ROZKAZ");
#endif                
            }
            return r;
        }
        posCmd = 0;
        sendError("ZLE CRC");
#ifdef DEBUG            
        Serial.print("CRC FAILD");
#endif        
        return false;

    }

    crc.add(data[posCmd-1]);    
    
    
    if (posCmd == MAXLENPROTO) {
        posCmd = 0;
        sendError("ZBYT DUZA WIAD");
#ifdef DEBUG           
        Serial.println("ZBYT DUZA WIADOMOSC");
#endif        
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
        case POSITION_REQ: 
        {
            char kind = data[1];
            if (kind == 'P') {
                posY = 0;
                for (short i = 0; i < 4; ++i) {
                    posY = (posY << 8) + (data[i+2] & 0xff);
                }
                
                posX = 0;
                for (short i = 0; i < 4; ++i) {
                    posX = (posX << 8) + (data[4+i+2] & 0xff);
                }
                actWork = POS_START;
                return true;
            } else if (kind == 'R') {
                rolP = 0;
                for (short i = 0; i < 4; ++i) {
                    rolP = (rolP << 8) + (data[i+2] & 0xff);
                }
                actWork = ROL_START;
                return true;
            }
            else {
                sendError("POS.RQ NIEZNANY");
#ifdef DEBUG           
                Serial.println("NIE ZNANY TYP POSITION-REQ");
#endif
            }
            return false;
        }
        case MOVEHOME_REQ:
        {
            char kind = data[1];
            if (kind == 'P') {
                actWork = RETURN_HOME;
                return true;
            } else if (kind == 'R') {
                actWork = ROL_HOME;
                return true;
            } else {
                sendError("HOME.RQ NIEZNAN");
#ifdef DEBUG           
                Serial.println("NIE ZNANY TYP POSITION-REQ");
#endif
            }
        }
        case SET_PARAM_REQ:
        {
            actWork = NOP;
            unsigned short nrU = data[1];
#ifdef DEBUG             
            Serial.print("NR=");
            Serial.println(nrU, DEC);
#endif            
            if (nrU == 1) {
                unsigned short statusByte = data[2] & 0xff;
#ifdef DEBUG                             
                Serial.print("STATUSBYTE=");
                Serial.println(statusByte, HEX);
#endif
                reverseR = 0x04 & statusByte;
                reverseY = 0x02 & statusByte;
                reverseX = 0x01 & statusByte;

#ifdef DEBUG             
                Serial.print("REVERSEGORA=");
                Serial.println(reverseY, DEC);

                Serial.print("REVERSEDOL=");
                Serial.println(reverseX, DEC);

                Serial.print("REVERSEROLETA=");
                Serial.println(reverseR, DEC);
#endif
                int32_t maxX = 0;
                for (uint8_t i = 0; i < 4; i++) {
                    maxX = (maxX << 8) + (data[3+i] & 0xff);
                }

                int32_t maxY = 0;
                for (uint8_t i = 0; i < 4; i++) {
                    maxY = (maxY << 8) + (data[7+i] & 0xff);
                }

#ifdef DEBUG 
                Serial.print("MAXX=");
                Serial.println(maxX, DEC);

                Serial.print("MAXY=");
                Serial.println(maxY, DEC);
#endif
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

                int32_t maxR = 0;
                for (uint8_t i = 0; i < 4; i++) {
                    maxY = (maxY << 8) + (data[11+i] & 0xff);
                }
#ifdef DEBUG
                Serial.print("MAXX=");
                Serial.println(maxX, DEC);

                Serial.print("MAXY=");
                Serial.println(maxY, DEC);

                Serial.print("MAXR=");
                Serial.println(maxR, DEC);
#endif
                gStepMaxX = maxX;
                gStepMaxY = maxY;
                gStepMaxR = maxR;
                sendAckSettings(nrU);     
                return true;
            } else {
                sendError("PAR.SET NIEZNAN");
#ifdef DEBUG           
                Serial.println("NIE ZNANY TYP PARAM-REQ");
#endif
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
    uint8_t sendData[2] = {'E', 'P'};
    sendMessage(MOVEHOME_REP, sendData, 2);
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

void MessageSerial::sendAckSettings(uint8_t nr) 
{
    uint8_t sendData[1] = {nr};
    sendMessage(SET_PARAM_REP, sendData, 1);
}

void MessageSerial::sendRoletaDone() 
{
    uint8_t sendData[1] = {'r'};
    sendMessage(POSITION_REP, sendData, 1);
}
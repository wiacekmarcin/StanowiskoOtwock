#include "protocol.hpp"

//10 imp silnika na 1 mm
#include "platform.h"


void Message::init() 
{
    posCmd[0] = 0;
    posCmd[1] = 0;
    rozkaz[0] = 0;
    rozkaz[1] = 0;
    dlugosc[0] = 0;
    dlugosc[1] = 0;
    crc[0].restart();
    crc[1].restart();
}

bool Message::check(uint8_t s, unsigned char c)
{
    data[s][posCmd[s]++] = c;
    if (posCmd[s]-1 == 0) {    
        crc[s].restart();
        crc[s].add(data[s][0]);
        rozkaz[s] = data[s][0] >> 4;
        dlugosc[s] = data[s][0] & 0x0f;
        return false;
    }
    
    if (posCmd[s] == dlugosc[s] + 2) {
        uint8_t c = crc[s].getCRC();
        if (data[s][posCmd[s]-1] == c) {
            posCmd[s] = 0;
            bool r = parse(s);
            if (!r)
                sendError("P:ZLY ROZKAZ", 12);
            return r;
        }
        posCmd[s] = 0;
        return false;

    }

    crc[s].add(data[s][posCmd[s]-1]);    
    
    
    if (posCmd[s] == MAXLENPROTO) {
        posCmd[s] = 0;
        sendError("P:ZBYT DUZA WI.", 15);
        return false;    
    }
    return false;
}

bool Message::check1(unsigned char c) 
{ 
    return check(0, c); 
}

bool Message::check2(unsigned char c) 
{ 
    return check(1, c); 
}

Message::Work Message::getStatusWork() const 
{ 
    return actWork; 
}

bool Message::parse1() {

    switch(rozkaz[0]) {
        case NOP_REQ: 
        {
            uint8_t sendData2[2] = {0xff,0x00};
            messageWrite1(sendData2,2);
            sendMessage1(NOP_REP, nullptr, 0);
            return true;
        }
        // get info |HEAD| CRC
        case WELCOME_REQ:   //get info 
        {                          //1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  
            uint8_t sendData[15] = {'K','O','N','T','R','O','L','E','R','P','O','S','R','E', 'D'};
            sendMessage1(WELCOME_REP, sendData, 15);
            return true;
        }
        case POSITION_REQ: 
        {
            //Serial.print("recv pos req ");
            //Serial.print(dlugosc[0] + 2);
            //sendRawMessage2(data[0], dlugosc[0] + 2);          
            uint32_t n = parseNumber(data[0][1], data[0][2], data[0][3], data[0][4]);            
            uint8_t tab[4];
            tab[0] = (n >> 24) & 0xff;
            tab[1] = (n >> 16) & 0xff;
            tab[2] = (n >> 8) & 0xff;
            tab[3] = n & 0xff;
            delay(n);
            sendMessage1(POSITION_REP, tab, 4);
            return true;
        }
        case ECHO_REQ: 
        {   
            if (data[0][1] == 0) {
                uint8_t sdata[1] = {0};
                sendMessage1(ECHO_REP, sdata, 1);
                return true;
            }
            if (data[0][1] == 1) {
                sendRawMessage2(data[0], dlugosc[0]+2);
                return true;
            }
        }
        case MOVEHOME_REQ:
        {
            //Serial.print("move home req ");
            //Serial.print(dlugosc[0] + 2);
            //sendRawMessage2(data[0], dlugosc[0]+2);
            uint32_t n = 55000;            
            uint8_t tab[4];
            tab[0] = (n >> 24) & 0xff;
            tab[1] = (n >> 16) & 0xff;
            tab[2] = (n >> 8) & 0xff;
            tab[3] = n & 0xff;
            delay(50000);
            sendMessage1(MOVEHOME_REP, tab, 4);
            return true;
        }
        case SET_PARAM_REQ:
        {
            //Serial.print("params set req ");
            //Serial.print(dlugosc[0] + 2);
            //sendRawMessage2(data[0], dlugosc[0]+2);
            sendMessage1(SET_PARAM_REP, nullptr, 0);
            return true;
        }
        case RESET_REQ: 
        {
            digitalWrite(2, LOW);
            //digitalWrite(13, HIGH);
            delay(1000);
            digitalWrite(2, HIGH);
            //digitalWrite(13, LOW);
            sendMessage1(RESET_REP, nullptr, 0);
            return true;
        }
        
        default:
            Serial.println("Nieznany rozkaz\n");
            break;

    }
    rozkaz[0] = 0;
    return false;
}

bool Message::parse2() {

    switch(rozkaz[1]) {
        
        // get info |HEAD| CRC
        case POSITION_REP: 
        {
            sendRawMessage1(data[1], dlugosc[1] + 2);            
            return true;
        }
        case MOVEHOME_REP:
        {
            sendRawMessage1(data[1], dlugosc[1]+2);
            return true;
        }
        case SET_PARAM_REP:
        {
            sendRawMessage1(data[1], dlugosc[1]+2);
            return true;
        }
        case ECHO_REP:
        {
            Serial.println("echo...rep");
            sendRawMessage1(data[1], dlugosc[1]+2);
            return true;
        }
        case ERROR_REP: 
        {
            sendRawMessage1(data[1], dlugosc[1]+2);
            return true;
        }
        case RESET_REP:
        {
            sendRawMessage1(data[1], dlugosc[1]+2);
            return true;
        }
        default:
            //Serial.print("Nie znany rozkaz z Serial1\n");
            uint8_t buf[16] = "S1:Niezn.rozkaz";
            sendError(buf, 15);
            break;

    }
    rozkaz[1] = 0;
    return false;
}

void Message::sendMessage(uint8_t s, uint8_t cmd, uint8_t* buf, uint8_t len)
{
    if (len > 15)
        return;
    
    uint8_t sendData[MAXLENPROTO] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if (len > 0)
        memcpy(sendData+1, buf, len);
    sendData[0] = cmd << 4 | len ;
    CRC8 lcrc;
    lcrc.restart();
    lcrc.add(sendData, len+1);
    sendData[len+1] = lcrc.getCRC();
    if (s == 0)
        messageWrite1(sendData, len+2);
    else
        messageWrite2(sendData, len+2);
}

void Message::sendRawMessage(uint8_t s, uint8_t* buf, uint8_t len)
{
    if (s == 0)
        messageWrite1(buf, len);
    else
        messageWrite2(buf, len);
}

void Message::messageWrite1(uint8_t* buf, uint8_t len)
{
    Serial.write(buf, len);
    Serial.flush();
}

void Message::messageWrite2(uint8_t* buf, uint8_t len)
{
    Serial1.write(buf, len);
    Serial1.flush();
}

void Message::sendError(const char *buf, uint8_t len)
{
    uint8_t b[17];
    for (short id = 0; id < len; ++id) {
        b[id] = buf[id];
    }
    sendError(b, len);
}

uint32_t Message::parseNumber(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
{
    uint32_t d1 = b1 & 0xff;
    uint32_t d2 = b2 & 0xff;
    uint32_t d3 = b3 & 0xff;
    uint32_t d4 = b4 & 0xff;
    return (d1 << 24) + (d2 << 16) + (d3 << 8) + d4;
}

void Message::reset()
{
    init();
}
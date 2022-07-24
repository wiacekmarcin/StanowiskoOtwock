

MessageSerialBase::MessageSerialBase()
{

#ifdef POSREDNIK 
    wasWelcomeMsg = false;
    timeSendRadioVal = 0;
#endif    
}

void MessageSerialBase::init()
{
    actWork = NOP;
}

void MessageSerialBase::sendMessage(uint8_t cmd, uint8_t* buf, uint8_t len)
{
    
}

bool MessageSerialBase::parseRozkaz()
{

}


bool MessageSerialBase::check(unsigned char c)
{
#ifdef SILNIK    
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
        Serial1.println("ZBYT DUZA WIADOMOSC");
        return false;    
    }
    return false;
#endif //silnik    
}

void MessageSerialBase::sendError(const char * errorStr)
{
    uint8_t buf[15];
    uint8_t i = 0;
    for (; i < 15 && *(errorStr + i) != '\0'; ++i) {
        buf[i] = (uint8_t)*(errorStr + i);
    }
    sendMessage(ERROR_REP, buf, i);
}



void MessageSerialBase::sendRadioVal(uint16_t val1, uint16_t val2, uint16_t val3, uint16_t val4)
{
#ifdef POSREDNIK    
    unsigned long actMls = millis();

    if (!wasWelcomeMsg)
        return;


    if (actMls - timeSendRadioVal < 500) 
        return ;
    sendRadioDebug(val1);
    timeSendRadioVal = actMls;
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
#endif    
}

void MessageSerialBase::sendRadioError()
{
#ifdef POSREDNIK
    unsigned long actMls = millis();
    
    if (!wasWelcomeMsg || timeSendRadioVal - actMls < 500)
        return ;
    timeSendRadioVal = actMls;
    uint8_t sendData[1] = {'E'};
    sendMessage(MEASVALUE_REP, sendData, 1);
#endif    
}

void MessageSerialBase::sendRadioDebug(uint16_t val)
{
#ifdef POSREDNIK
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
#endif
}



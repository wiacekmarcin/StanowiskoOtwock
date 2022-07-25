

MessageSerialBase::MessageSerialBase()
{

   
}

void MessageSerialBase::init()
{
    
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


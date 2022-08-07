#include <Arduino.h>
#include "protocol.hpp"

MessageSerial msg;
void checkMsg();
bool work();

void setup()
{
    msg.init();
    Serial.begin(115200);

}
//#define DEBUG_SERIAL
void loop()
{

    
    if (Serial.available())  {
        if (msg.check(Serial.read())) {
            work();
        }
    }
}

bool work()
{
    
    MessageSerial::Work actWork = msg.getStatusWork();
    if (actWork == MessageSerial::NOP) {
        return false;
    }
    
    switch(actWork) {
        case MessageSerial::WELCOME_MSG:
            delay(3000);
            actWork = MessageSerial::NOP;
            msg.sendWelcomeMsg();
            return true;
       
        default:
            //Serial.println("UNKNOWN");
        break;
    }
    return true;
}
    


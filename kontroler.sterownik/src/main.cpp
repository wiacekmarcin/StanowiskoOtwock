#include <Arduino.h>
#include "protocol.hpp"

#include "sterowniksilnika.hpp"


//#define DEBUG
MessageSerial msg;
void checkMsg();
bool work();


void setup()
{
   // Serial.begin(115200);
#ifndef DEBUG
    msg.init();
#endif // !DEBUG    

    initSilniki();
    initCzujnikiKrancowe();
    initEnkodery();
    interrupts();

#ifdef DEBUG
    Serial.begin(115200);
#endif  
    Serial1.begin(115200);
    //Serial.begin(115200);
}

void loop()
{
    if (Serial1.available())  
        if (msg.check(Serial1.read()))
            work();   
}



bool work()
{
    static MessageSerial::Work prevWork = MessageSerial::NOP;
    MessageSerial::Work actWork = msg.getStatusWork();
#ifdef DEBUG
    if (prevWork != actWork) {
        Serial.println("ACTWORK=");
        Serial.println(actWork, DEC);

        Serial.println("ACTWORK=");
        Serial.println(actWork, DEC);
        prevWork = actWork;
        delay(1000);
    }
#endif            
    if (actWork == MessageSerial::NOP) {
        return false;
    }
    
    switch(actWork) {
        case MessageSerial::POS_START:
            delay(200);
            msg.sendPositionStart();
            delay(200);
            setPosY(msg.getPosY());
            delay(200);
            setPosX(msg.getPosX());
            delay(200);
            msg.sendPositionDone();
            actWork = MessageSerial::NOP;
        return true;

        case MessageSerial::RETURN_HOME:
            delay(200);
            msg.sendRetHomeStart();
            delay(200);
            if (!returnBaseX()) {
                msg.setStop();
                delay(200);
                msg.sendRetHomeDone();
                actWork = MessageSerial::NOP;
            }
            delay(200);
            if (!returnBaseY()) {
                msg.setStop();
                delay(200);
                msg.sendRetHomeDone();
                actWork = MessageSerial::NOP;
            }
            delay(200);
            msg.sendRetHomeDone();
            actWork = MessageSerial::NOP;
        return true;
        case MessageSerial::ROL_START:
            msg.sendRoletaStart();
            delay(200);
            setPosR(msg.getRol());
            actWork = MessageSerial::NOP;
        return true;
        case MessageSerial::ROL_HOME:
            returnBaseR();
            actWork = MessageSerial::NOP;
        return true;
        default:
#ifdef DEBUG                
            Serial.println("UNKNOWN WORK TYPE");
#endif            
        break;
    }
    return true;
}
    


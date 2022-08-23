#include <Arduino.h>
#include "protocol.hpp"

#include "sterowniksilnika.hpp"


//#define DEBUG
MessageSerial msg;
void checkMsg();
bool work();

extern bool infoPos;


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
            delay(100);
            msg.sendPositionStart();
            delay(100);
            setPosX(msg.getPosX());
            delay(100);
            setPosY(msg.getPosY());
            delay(100);
            msg.sendPositionDone();
            actWork = MessageSerial::NOP;
        return true;

        case MessageSerial::RETURN_HOME:
            if (infoPos) {
                msg.sendRetHomeStart();
                delay(100);
            }
            if (!returnBaseX(infoPos)) {
                msg.setStop();
                //delay(100);
                //msg.sendRetHomeDone();
                actWork = MessageSerial::NOP;
                return false;
            }
            delay(100);
            if (!returnBaseY(infoPos)) {
                msg.setStop();
                //delay(100);
                //msg.sendRetHomeDone();
                actWork = MessageSerial::NOP;
                return false;
            }
            if (infoPos) {
                msg.sendRetHomeDone();
                delay(100);
            }
            actWork = MessageSerial::NOP;
        return true;
        case MessageSerial::ROL_START:
            msg.sendRoletaStart();
            delay(100);
            setPosR(msg.getRol());
            actWork = MessageSerial::NOP;
        return true;
        case MessageSerial::ROL_HOME:
            if (infoPos) {
                msg.sendRetHomeRStart();
                delay(100);
            }
            uint32_t step;
            if (!returnBaseR(&step)) {
                if (infoPos) {
                    msg.setErrorRoletaHomeBack();
                    delay(100);
                }
                msg.setStop();
            } else {
                if (infoPos) {
                    msg.sendRetHomeRDone(step);
                    delay(100);

                }
            }
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
    


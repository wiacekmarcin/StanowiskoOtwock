#include <Arduino.h>
#include "protocol.hpp"

#include "sterowniksilnika.hpp"


//#define DEBUG_SERIAL
MessageSerial msg;
void checkMsg();
bool work();

void setup()
{
#ifndef DEBUG_SERIAL
    msg.init();
#endif // !DEBUG_SERIAL    
    
    initSilniki();
    initCzujnikiKrancowe();
    initEnkodery();
    interrupts();

#ifdef DEBUG_SERIAL
    Serial.begin(115200);
#endif  

}

void loop()
{
#ifndef DEBUG_SERIAL
    //
    if (Serial1.available())  
        if (msg.check(Serial1.read()))
            work();


#endif // !DEBUG_SERIAL
   

}

bool wasWelcomeMsg = false;

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
            msg.sendPositionStart();
         
            setPosY(msg.getPosY());
            setPosX(msg.getPosX());
        
            msg.sendPositionDone();
            actWork = MessageSerial::NOP;
        return true;

        case MessageSerial::RETURN_HOME:
            msg.sendRetHomeStart();
            if (!returnBaseX()) {
                msg.setStop();
                delay(200);
                msg.sendRetHomeDone();
                actWork = MessageSerial::NOP;
            }

            if (!returnBaseY()) {
                msg.setStop();
                delay(200);
                msg.sendRetHomeDone();
                actWork = MessageSerial::NOP;
            }
            msg.sendRetHomeDone();
            actWork = MessageSerial::NOP;
        return true;
        case MessageSerial::ROL_START:
            
            setPosR(msg.getRol());

        return true;
        case MessageSerial::ROL_HOME:
        return true;
        case MessageSerial::ROL_DONE:
        return true;
        default:
#ifdef DEBUG                
            Serial.println("UNKNOWN WORK TYPE");
#endif            
        break;
    }
    return true;
}
    


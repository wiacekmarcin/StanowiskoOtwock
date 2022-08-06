#include <Arduino.h>
#include "protocol.hpp"
#include "radio.hpp"

#define RESET_NANO 4
//#define DEBUG_SERIAL
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

    if (actWork == MessageSerial::NOP) {
        return false;
    }
    
    switch(actWork) {
        case MessageSerial::WELCOME_MSG:
            digitalWrite(RESET_NANO, LOW);
            delay(1000);
            delay(2000);
            actWork = MessageSerial::NOP;
            return true;
        case MessageSerial::GET_RADIOVAL:
            if (isRadioConnected()) {
                uint16_t val1;
                //int8_t try3 = 3;
                //while (try3 > 0) {
                if (readRadio(val1)) {
                    msg.sendRadioVal(val1, 0, 0, 0);
                } else {
                    msg.sendRadioError(1);
                }
            } else {
                msg.sendRadioError(2);
            }
            actWork = MessageSerial::NOP;
            return true;
        
        default:
            //Serial.println("UNKNOWN");
        break;
    }
    return true;
}
    


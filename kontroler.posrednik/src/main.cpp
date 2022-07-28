#include <Arduino.h>
#include "protocol.hpp"
#include "radio.hpp"

#define RESET_NANO 4
#define DEBUG_SERIAL
MessageSerial msg;
void checkMsg();
bool work();

void setup()
{
#ifndef DEBUG_SERIAL
    msg.init();
#endif // !DEBUG_SERIAL    
    

    radioInit();
    
    interrupts();

    Serial1.begin(115200);
    Serial.begin(115200);

    pinMode(RESET_NANO, OUTPUT);
    digitalWrite(RESET_NANO, LOW);
    delay(1000);
    digitalWrite(RESET_NANO, HIGH);
}

void loop()
{
#ifndef DEBUG_SERIAL
    
    if (Serial.available())  {
        if (msg.check(Serial.read())) {
            work();
        }
    }
    if (Serial1.available()) {
        Serial.write(Serial1.read());
    }


#endif // !DEBUG_SERIAL
    
#ifdef DEBUG_SERIAL

    static bool firsttime = false;
    if (!firsttime) {
        delay(10000);
        Serial.println("Init Radio");
    
        radioInit();
        firsttime = true;
        Serial.println("Radio Init");
    }
    
    delay(1000);
    uint16_t val1 = 0;
    if (isRadioConnected()) {
        if (readRadio(val1)) {;
            Serial.print("val=");
            Serial.println(val1);
        }
        else {
            Serial.println("Cannot read data");
        }
    } else {
        Serial.println("radio nie polaczone");
    }
    
#endif
}

bool wasWelcomeMsg = false;
int tryReadRadio = 10;
bool work()
{
    static MessageSerial::Work prevWork = MessageSerial::NOP;
    MessageSerial::Work actWork = msg.getStatusWork();
    #ifdef DEBUG_SERIAL   
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
        /*
        uint16_t val1 = 0;
        if (isRadioConnected() && readRadio(val1)) {
            msg.sendRadioVal(val1, 0, 0, 0);
            tryReadRadio = 10;
        } else {
            if (!--tryReadRadio) {
                tryReadRadio = 10;
                msg.sendRadioError();
            }
        }*/
        return false;
    }
    
    switch(actWork) {
        case MessageSerial::WELCOME_MSG:
            digitalWrite(RESET_NANO, LOW);
            delay(1000);
            digitalWrite(RESET_NANO, HIGH);
            actWork = MessageSerial::NOP;
            return true;
        case MessageSerial::GET_RADIOVAL:
            if (isRadioConnected()) {
                uint16_t val1;
                if (readRadio(val1)) {
                    msg.sendRadioVal(val1, 0, 0, 0);
                }
                else {
                    msg.sendRadioError();    
                }
            } else {
                msg.sendRadioError();
            }
            msg.sendRadioError();
            actWork = MessageSerial::NOP;
            return true;
        
        default:
            Serial.println("UNKNOWN");
        break;
    }
    return true;
}
    


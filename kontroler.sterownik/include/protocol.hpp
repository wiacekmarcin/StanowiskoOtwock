#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <Arduino.h>
#include "../../kontroler_lib/crc8.h"

#define SILNIK 1
#include "../../kontroler_lib/protocol_kontroler.hpp"

class MessageSerial : public MessageSerialBase
{
public :
    MessageSerial();

    bool check(unsigned char c);

    void sendPositionStart();
    void sendPositionStartX();
    void sendPositionDoneX(uint32_t step, uint32_t pos);
    

    void sendPositionStartY();
    void sendPositionDoneY(uint32_t step, uint32_t pos);
    
    void sendPositionDone();

    void sendRoletaStart();
    void sendRoletaDone(uint32_t step, uint32_t stepPos));

    void sendRetHomeStart();
    void sendRetHomeYDone(uint32_t step);
    void sendRetHomeXDone(uint32_t step);
    void sendRetHomeYStart();
    void sendRetHomeXStart();
    void sendRetHomeDone();

    void sendRetHomeRDone();

    void sendAckSettings(uint8_t nr);
    void setErrorHomeBack();
    void setErrorRoletaHomeBack();

    uint32_t getPosX() const { return posX; }
    uint32_t getPosY() const { return posY; }
    uint32_t getRol() const { return rolP; }

    void init();
protected:
    void sendMessage(uint8_t cmd, uint8_t* buf, uint8_t len);
    bool parseRozkaz();
private:
    uint32_t posY;
    uint32_t posX;
    uint32_t rolP;

};


#endif // !_PROTOCOL_H_
#ifndef SERIALMSG_H
#define SERIALMSG_H

#pragma once

namespace SerialMsg {
union UHead {
    struct CMessage {
        uint8_t  rozkaz : 3 ;
        uint8_t  dir    : 1 ;
        uint8_t  len    : 4 ;
    } h;
    uint8_t head;
};

typedef enum _msgType {
    NOP         = 0,
    WELCOME     = 1,
    POSISTION   = 2,
    RETURNHOME  = 3,
    ROLETA      = 4,
    ROLETAHOME  = 5,
    PARAMETERS  = 6,
    MEASURE     = 7,
} MsgType;

union UMessage {
    uint_8 welcome[15] = {'K','O','N','T','R','O','L','E','R','W','I','A','T','R', 'U'};
    
}


void createMsg(uint8_t ** msg, uint8_t write, SerialMsg::MsgType mtype, uint8_t * tab, uint8_t len);

}


#endif
#include "serialmsg.h"

namespace SerialMsg {

void createMsg(uint8_t ** msg, uint8_t write, SerialMsg::MsgType mtype, uint8_t * tab, uint8_t len)
{
    *msg = new uint8_t[2+len];
    UMessage m;
    m.h.dir = read & 0x1;
    m.h.rozkaz = ((uint8_t)mtype)&0x7;
    m.h.len = len & 0xf;
    (*msg)[0] = m.head;
    memcpy((*msg) + 1, tab, len);
    
    

    struct CMessage {
        uint8_t  dir    : 1 ;
        uint8_t  rozkaz : 3 ;
        uint8_t  len    : 4 ;
        uint8_t  msg[15];
        uint8_t  crc;
    } msg;
    
}




} //namespace


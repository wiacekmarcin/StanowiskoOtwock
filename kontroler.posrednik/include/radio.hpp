#ifndef RADIO_H
#define RADIO_H



#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "platform.h"

#define CHANNEL_NUM 0x27

const byte addressSlave[6] = "CZWIA";
const byte addressMaster[6] = "KONTR";

bool radioInit();
bool isRadioConnected();
bool readRadio(uint16_t & val1);

#endif // !RADIO_H
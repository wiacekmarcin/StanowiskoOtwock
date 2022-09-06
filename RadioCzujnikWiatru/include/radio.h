#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 7
#define CSN_PIN 8
#define CHANNEL_NUM 0x27

const byte addressSlave[6] = "CZWIA";
const byte addressMaster[6] = "KONTR";
bool radioInit();
bool isRadioConnected();
bool send(uint16_t val1, uint16_t val2);
bool recvData();

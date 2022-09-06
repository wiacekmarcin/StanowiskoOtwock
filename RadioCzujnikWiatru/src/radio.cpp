#include "radio.h"
RF24 radio(CE_PIN, CSN_PIN); // CE, CSN

bool radioInit() {
    if (!radio.begin())
        return false;

    //radio.setChannel(CHANNEL_NUM);
    //radio.setAutoAck(true);
    //radio.setCRCLength(RF24_CRC_16);
    //if (!radio.setDataRate(RF24_250KBPS))
    //    return false;
    radio.setPayloadSize(4);
    radio.setPALevel(RF24_PA_MIN);
    radio.openWritingPipe(addressSlave);
    radio.openReadingPipe(1, addressMaster); // using pipe 1
    
    //najpierw nasluchujemy
    //radio.stopListening();
    radio.startListening();
    return true;
}


bool isRadioConnected()
{
    return radio.isChipConnected();
}

bool recvData() {
    uint8_t tryingNum = 10;
    while (!radio.available() && tryingNum--) {
        delay(5);
    }

    if (!tryingNum) {
        return false;
    }

    char cmd;
    //uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
    radio.read(&cmd, 1);
    if (cmd == 'G') {
       return true; 
    }
    return false; 
}

bool send(uint16_t val1, uint16_t val2)
{
    uint8_t tryingNum = 100;
    radio.stopListening();
    delay(5);

    uint8_t data[4];
    data[0] = val1 >> 8;
    data[1] = val1 & 0x00ff;
    data[2] = val2 >> 8;
    data[3] = val2 & 0x00ff;
    while(!radio.write(data, 4) && --tryingNum)
        delay(5);

    radio.startListening();
    delay(5);
    return tryingNum > 0;
}


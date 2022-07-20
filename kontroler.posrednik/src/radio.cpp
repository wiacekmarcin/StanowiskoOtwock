#include "radio.hpp"

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//#define DEBUG_RADIO

//#define DEBUG_SERIAL

RF24 radio(CE_PIN, CSN_PIN); // CE, CSN

bool radioInit() {
#ifdef DEBUG_RADIO
    return true;
#endif // DEBUG    
    if (!radio.begin())
        return false;

    //radio.setChannel(CHANNEL_NUM);
    //radio.setAutoAck(true);
    //radio.setCRCLength(RF24_CRC_16);
    //if (!radio.setDataRate(RF24_250KBPS))
    //    return false;
    
    //radio.setPALevel(RF24_PA_LOW);
    //radio.setPayloadSize(8);
    radio.enableDynamicPayloads();  // Enable dynamic payloads

    radio.setRetries(5, 15);  
    radio.openWritingPipe(addressMaster);
    radio.openReadingPipe(1, addressSlave); // using pipe 1
    radio.stopListening();
    delay(10);
    radio.startListening();
    return true;
}


bool isRadioConnected()
{
#ifdef DEBUG_RADIO
    return true;
#endif // DEBUG    

#ifdef DEBUG_SERIAL
    radio.printDetails();
#endif

    return radio.isChipConnected();
}

bool readRadio(uint16_t & val1)
{
#ifdef DEBUG_RADIO
    static uint16_t val = 0;
    val1 = val++ & 0x0fff;
    return true;
#endif // DEBUG 


    uint8_t tryingNum = 10;
    
    radio.stopListening();
    delay(5);
    
    char buf[6] = {'S', 'T', 'A', 'R', 'T'};
#ifdef DEBUG_SERIAL    
    Serial.println("Write 'START'");
#endif    
    
    while (!radio.write(buf,6) && tryingNum--) {
        delay(5);
    }
    if (!tryingNum) {
#ifdef DEBUG_SERIAL        
        Serial.println("Nie udalo sie wyslac do czujnika");
#endif        
        return false;
    }

    radio.startListening();
    delay(5);

    tryingNum = 200;
#ifdef DEBUG_SERIAL    
    Serial.println("Odbieram");
#endif    
    while (!radio.available() && --tryingNum ) {
        delay(5);
    }
    
    if (tryingNum == 0) {
#ifdef DEBUG_SERIAL         
        Serial.print("Nie udalo sie odczytac, trynum = ");
        Serial.println(tryingNum,DEC);
#endif        
        radio.stopListening();
        delay(5);
        return false;
    }
#ifdef DEBUG_SERIAL    
    Serial.print("trynum = ");
    Serial.println(tryingNum,DEC);
#endif
    uint8_t data[32];
    uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
    radio.read(data, bytes); 
#ifdef DEBUG_SERIAL    
    Serial.print("bytes=");
    for (int b = 0; b < bytes; b++) {
        Serial.print(data[b],HEX);
    }
    
    Serial.print( " size=");
    Serial.println(bytes, DEC);
#endif    
    
    
    uint32_t licznik = 0;

    
    licznik = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    val1 = (data[4] << 8) | data[5];
#ifdef DEBUG_SERIAL        
    Serial.print("val1=");
    Serial.println(val1, DEC);
    Serial.print("val2=");
    Serial.println(licznik, DEC);
#endif    
    radio.stopListening();
    delay(5);
    return licznik != 0;
}

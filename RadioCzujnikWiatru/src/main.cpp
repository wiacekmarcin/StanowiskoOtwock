/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 7 & 8

RF24 radio(7, 8);

//
// Channel info
//

const uint8_t num_channels = 126;
uint8_t values[num_channels];

//
// Setup
//

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(9600);
  printf_begin();
  Serial.println(F("\n\rRF24/examples/scanner/"));

  //
  // Setup and configure rf radio
  //

  radio.begin();
  radio.setAutoAck(false);

  // Get into standby mode
  radio.startListening();
  radio.stopListening();
  radio.printDetails();

  //delay(1000);
  // Print out header, high then low digit
  int i = 0;
  while ( i < num_channels )
  {
    Serial.print(i >> 4, HEX);
    ++i;
  }
  Serial.println();
  i = 0;
  while ( i < num_channels )
  {
    Serial.print(i & 0xf, HEX);
    ++i;
  }
  Serial.println();
  //delay(1000);
}

//
// Loop
//

const int num_reps = 100;
bool constCarrierMode = 0;

void loop(void)
{
  /****************************************/
  // Send g over Serial to begin CCW output
  // Configure the channel and power level below
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'g') {
      constCarrierMode = 1;
      radio.stopListening();
      delay(2);
      Serial.println("Starting Carrier Out");
      radio.startConstCarrier(RF24_PA_LOW, 40);
    } else if (c == 'e') {
      constCarrierMode = 0;
      radio.stopConstCarrier();
      Serial.println("Stopping Carrier Out");
    }
  }
  /****************************************/

  if (constCarrierMode == 0) {
    // Clear measurement values
    memset(values, 0, sizeof(values));

    // Scan all channels num_reps times
    int rep_counter = num_reps;
    while (rep_counter--)
    {
      int i = num_channels;
      while (i--)
      {
        // Select this channel
        radio.setChannel(i);

        // Listen for a little
        radio.startListening();
        delayMicroseconds(128);
        radio.stopListening();

        // Did we get a carrier?
        if ( radio.testCarrier() ) {
          ++values[i];
        }
      }
    }


    // Print out channel measurements, clamped to a single hex digit
    int i = 0;
    while ( i < num_channels )
    {
      Serial.print(min(0xf, values[i]), HEX);
      ++i;
    }
    Serial.println();

  }//If constCarrierMode == 0
}
#if 0

#include <Arduino.h>?
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

unsigned long premls = 0L;
void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    unsigned long mls = millis();
    Serial.print(mls - premls, DEC);
    Serial.print(" ");
    premls = mls;
    Serial.println(text);
  }
}

#endif
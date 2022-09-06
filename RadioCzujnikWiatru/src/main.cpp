#include <Arduino.h>

#include <Wire.h>
#include <DFRobot_ADS1115.h>
#include <Adafruit_SSD1306.h>

#include "lcd.h"
#include "radio.h"

DFRobot_ADS1115 ads(&Wire);
extern Adafruit_SSD1306 display;
volatile bool show_mV = false;
#define BTN_PIN 10

void changeUnit()
{
    show_mV = ! show_mV;
}

void setup() {
    Wire.begin();
    Serial.begin(9600);
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x48
    ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
    ads.setMode(eMODE_SINGLE);       // single-shot mode
    ads.setRate(eRATE_128);          // 128SPS (default)
    ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
    ads.init();

    lcdsetup();

    if (!radioInit()) {
        drawStatus(RADIO_ERROR);
    }

    if (!isRadioConnected()) {
        drawStatus(RADIO_NOPRESENT);
    }

    pinMode(BTN_PIN, INPUT);

    attachInterrupt(BTN_PIN, changeUnit, FALLING);

}
bool wasADC = true;
int16_t adc0, adc1, adc2, adc3;
bool prev_mV = false;
void loop() {
    if (prev_mV != show_mV) {
        prev_mV = show_mV;
        changeUnit(show_mV);
    }
    if (ads.checkADS1115())
    {
        if (!wasADC) {
            clearDisplay();
            wasADC = true;
        }
        adc0 = ads.readVoltage(0);
        if (!show_mV)
            drawSpeed(adc0/100.0);
        else
            drawmVolts(show_mV);

        drawPercent(100*adc0/5250);
    }
    else
    {
        drawNOD1115();
        wasADC = false;
    }

    if (isRadioConnected()) {
        //drawStatus(RADIO_SENDING);
        if (recvData()) {
            if (send(adc0, adc1)) {
                drawStatus(RADIO_SENDOK);
            } else {
                drawStatus(RADIO_ERROR);
            }
        } else {
            drawStatus(WAITING_DATA);
        }
    } else {
        drawStatus(RADIO_NOPRESENT);
    }

    delay(500);
}

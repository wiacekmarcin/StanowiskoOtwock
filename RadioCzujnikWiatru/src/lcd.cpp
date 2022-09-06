#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "lcd.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   20
#define LOGO_WIDTH    24
static const unsigned char PROGMEM logo_bmp[] =
{   B00000000, B00000000, B00001100,
    B00000000, B00111000, B00011110,
    B00000000, B01000100, B00111111,
    B00000000, B10000010, B00111111,
    B00000001, B00000001, B00011110,
    B00000001, B00000001, B00001100,
    B00000111, B11111111, B11000000,
    B00111111, B11111111, B11111000,
    B11101010, B10101010, B10101111,
    B01111111, B11111111, B11111100,
    B00011100, B00000000, B01110000,
    B00000011, B11111111, B10000000,
    B00000010, B01000100, B10000000,
    B00000011, B11111111, B10000000,
    B00000011, B11111111, B10000000,
    B00000100, B00010000, B01000000,
    B00001000, B00010000, B00100000,
    B00010000, B00010000, B00010000,
    B00111000, B00111000, B00111000,
    B11111111, B11111111, B11111111,

 };

void lcdsetup() {
  
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //Serial.println(F("SSD1306 allocation failed"));
    //for(;;); // Don't proceed, loop forever
    }


    // Clear the buffer
    display.clearDisplay();

  
    drawBitmap();    // Draw a small bitmap image
    delay(2000);
    display.clearDisplay();

    drawPercent(0);
    display.setTextSize(3);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);        // Draw white text
    display.setCursor(5,20);             // Start at top-left corner
    display.println(F("-- --"));
    
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    
    display.setCursor(100,28);
    display.setTextSize(1); 
    display.print(F(" m/s"));
    display.display();
    display.setCursor(5,55);
    display.println(F("Checking radio....")); 

    display.display();
    delay(5000);

}

void drawPercent(uint8_t perc) 
{
    static uint8_t prev = -1;

    uint8_t p = perc / 10;
    uint8_t left;
    if (prev == p) {
        return;
    }
    display.fillRect(0, 2, 128, 10, SSD1306_BLACK);

    for (int i = 0 ; i < 10; i++) {
        left = 4 + i*(10+2);
        if (i <= p)
            display.fillRect(left+1, 2, 10, 8, SSD1306_WHITE);
        else
            display.drawRect(left+1, 2, 10, 8, SSD1306_WHITE);
    }
    display.display();
}



void drawSpeed(float value) {
    display.fillRect(0, 20, 100, 22, SSD1306_BLACK);

    display.setTextSize(3);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);        // Draw white text
    display.setCursor(0,20);             // Start at top-left corner
    display.print(value, 2);
    display.display();
}

void drawStatus(RadioStatus st)
{
    static RadioStatus prevstatus = RADIO_CHECKING;
    if (prevstatus == st) {
        return;
    }
    display.fillRect(0, 55, 128, 16, SSD1306_BLACK);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    display.setCursor(5, 55); 
    switch(st) {
        case RADIO_OK:
            display.print(F("Radio OK"));
            break;
        case RADIO_CHECKING:
            display.print(F("Radio checking..."));
            break;
        case RADIO_ERROR:
            display.print(F("Radio send ERROR"));
            break;
        case RADIO_NOPRESENT:
            display.print(F("NO radio module"));
            break;
        case RADIO_SENDING:
            display.print(F("Sending data..."));
            break;
        case RADIO_SENDOK:
            display.print(F("Send data OK"));
            break;
        case WAITING_DATA:
            display.print(F("Waiting for data"));
            break;
        default:
            break;
    }
    display.display();
}

void drawBitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void drawNOD1115()
{
    display.fillRect(0, 0, 128, 55, SSD1306_BLACK);
    display.setTextSize(3);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.print("NO\nADS1115");
    display.display();
}

void clearDisplay() {
     // Clear the buffer
    display.clearDisplay();
    display.display();

}

void drawmVolts(uint16_t value)
{
    display.fillRect(0, 20, 100, 22, SSD1306_BLACK);

    display.setTextSize(3);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);        // Draw white text
    display.setCursor(0,20);             // Start at top-left corner
    display.print(value);
    display.display();
}

void changeUnit(bool mV)
{
    display.fillRect(100, 28, 128, 50, SSD1306_BLACK);
    display.setCursor(100,28);
    display.setTextSize(1); 
    if (mV) {
        display.print(F(" mV"));
    } else {
        display.print(F(" m/s"));
    }
    display.display();
}
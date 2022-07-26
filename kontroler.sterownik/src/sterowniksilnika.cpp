#include "sterowniksilnika.hpp"
#include <Arduino.h>
#include "protocol.hpp"

//#define NORMAL
#define OKNO_SILNIK_PRAWA
uint32_t gImpMaxX = 291000L;
uint32_t gImpMaxY = 301000L;

uint32_t gStepMaxX = 73000L;
uint32_t gStepMaxY = 76000L;


int32_t gStepMaxR = 63000L;

#ifdef OKNO_SILNIK_PRAWA
    bool reverseY = false;
    bool reverseX = true;
#endif

#ifdef NORMAL
    bool reverseY = false;
    bool reverseX = false;
#endif

#ifdef OKNO_SILNIK_LEWA
    bool reverseY = false;
    bool reverseX = false;
#endif

bool reverseR = true;

extern MessageSerial msg;


/* Aktualna pozycja na macierzy w impulsach */
volatile int32_t gActPosImpX = 0;
volatile int32_t gActPosImpY = 0;

/* Aktualna pozycja na macierzy w krokach silnika */
volatile int32_t gActPosStepX = 0;
volatile int32_t gActPosStepY = 0;

/* Aktualna pozycja silnika rolety */
volatile int32_t gActPosStepRol  = 0;


/* ilosc impulsow do przesuniecia */
volatile int32_t gMoveImpX = 0;
volatile int32_t gMoveImpY = 0;
volatile int32_t gMoveStepR = 0;

static volatile bool homePosX   = false;
static volatile bool homePosY   = false;

static volatile bool homePosRId[16];
static volatile uint8_t homeRIdx = 0;
static volatile bool homePosR   = false;

volatile bool canMoveX = false;
volatile bool canMoveY = false;


//#define TEST_SILNIKA


constexpr uint16_t const freqs[] = {
  2000,
  1225,
  850,
  630,
  500,
  415,
  355,
  312,
  280,
  250,
  225,
  200
};
 
constexpr uint8_t sizeFreq = sizeof(freqs) / sizeof(uint16_t);
constexpr uint16_t sizeImpuls = sizeFreq << 6;

/** czujniki krancowe ***/
void initCzujnikiKrancowe()
{
    pinMode(BASE_Y, INPUT);
    attachInterrupt(digitalPinToInterrupt(BASE_Y), setHomePosY, CHANGE);

    pinMode(BASE_X, INPUT);
    attachInterrupt(digitalPinToInterrupt(BASE_X), setHomePosX, CHANGE);

    pinMode(BASE_R, INPUT);
    attachInterrupt(digitalPinToInterrupt(BASE_R), setHomePosR, CHANGE);
}

void setHomePosX()
{
    homePosX = digitalRead(BASE_X) == LOW;
    //canMoveX = !homePosX;
}

void setHomePosY()
{
    homePosY = digitalRead(BASE_Y) == LOW;
    //canMoveY = !homePosY;
}

void setHomePosR()
{
    homePosRId[homeRIdx & 0x0f] = digitalRead(BASE_R) == LOW;
    homeRIdx++;
}


inline bool getHomePosX()
{
    return homePosX;
}

inline bool getHomePosY()
{
    return homePosY;
}

inline bool getHomePosR()
{
    for (uint8_t i = 0; i < 16; ++i ) {
        if (!homePosRId[i])
            return false;
    }
    return true;
}

void stepX(uint16_t delay1, uint16_t delay2)
{
    delayMicroseconds(delay1);
    //digitalWrite(PULSE_X, HIGH);
    //digitalWrite(PULSE_Y, HIGH);
    PORTF.OUTSET = 0x0c;
    delayMicroseconds(delay2);
    //digitalWrite(PULSE_X, LOW);
    //digitalWrite(PULSE_Y, LOW);
    PORTF.OUTCLR = 0x0c;
}

void stepY(uint16_t delay1, uint16_t delay2)
{
    delayMicroseconds(delay1);
    digitalWrite(PULSE_Y, HIGH);
    delayMicroseconds(delay2);
    digitalWrite(PULSE_Y, LOW);
    //PORTF.OUTCLR = 0x0d;
}

void stepR(uint16_t delay1, uint16_t delay2)
{
    delayMicroseconds(delay1);
    digitalWrite(PULSE_R, HIGH);
    delayMicroseconds(delay2);
    digitalWrite(PULSE_R, LOW);
    //PORTF.OUTCLR = 0x0d;
}

bool returnBaseX()
{
    msg.sendRetHomeXStart();
    setHomePosX();
    setDirX(X_LEFT);

    //attachEnkoderX(true);
    uint32_t step = 0;

#ifdef TEST_SILNIKA    
    msg.sendRetHomeXDone(step);
    gActPosImpX = 0;
    attachEnkoderX(false);
    delay(2000);
    return true;
#endif

    if (!getHomePosX()) {
        
        while (!getHomePosX() && ++step < gStepMaxX) {
            stepX(300, 300);
        }
        if (!getHomePosX()) {
            msg.setErrorHomeBack();
            //attachEnkoderX(false);
            return false;
        }
        //przesuwam jeszcze z 2mm glebiej karetke
        uint8_t idx = 200;
        step+=200;
        while (--idx) {
            stepX(1000, 1000);
        }
    }
    msg.sendRetHomeXDone(step);
    gActPosImpX = 0;
    gActPosStepX = 0;
    //attachEnkoderX(false);
    return true;
}

bool returnBaseY()
{
    msg.sendRetHomeYStart();
    setHomePosY();
    setDirY(Y_DOWN);

    //attachEnkoderY(true);
    uint32_t step = 0;

#ifdef TEST_SILNIKA    
    msg.sendRetHomeYDone(step);
    gActPosImpY = 0;
    attachEnkoderY(false);
    delay(2000);
    return true;
#endif


    if (!getHomePosY()) {
        
        while (!getHomePosY() && ++step < gImpMaxY) {
            stepY(300, 300);
        }
        if (!getHomePosX()) {
            msg.setErrorHomeBack();
            //attachEnkoderY(false);
            return false;

        }
        //przesuwam jeszcze z 2mm glebiej karetke
        uint8_t idx = 200;
        step += 200;
        while (--idx) {
            stepY(1000,1000);
        }
    }
    msg.sendRetHomeYDone(step);
    //printPos();
    gActPosImpY = 0;
    gActPosStepY = 0;
    //attachEnkoderY(false);
    return true;
}

bool returnBaseR()
{
    msg.sendRetHomeRStart();
    Serial.println("Return BASE");
    setHomePosR();
    setDirR(R_DOWN);
    Serial.print("digitalRead(BASE_R)=");
    Serial.println(digitalRead(BASE_R),DEC);
    uint32_t step = 0;


    if (!getHomePosR()) {
        
        while (!getHomePosR() /*&& ++step < gStepMaxR*/) {
            ++step;
            stepR(400, 400);
        }
        if (!getHomePosR()) {
            msg.setErrorRoletaHomeBack();
            return false;

        }
        Serial.print("digitalRead(BASE_R)=");
        Serial.println(digitalRead(BASE_R),DEC);
        //przesuwam jeszcze z 2mm glebiej karetke
        uint8_t idx = 20;
        step += 20;
        while (--idx) {
            stepR(1000,1000);
        }
    }
    Serial.println("Done");
    Serial.println(step,DEC);
    msg.sendRetHomeRDone();
    gActPosStepRol = 0;

    return true;
}

void setDirX(bool lewo) {
    digitalWrite(DIR_Y, (lewo ^ reverseX) ? HIGH : LOW);
    digitalWrite(DIR_X, (lewo ^ reverseX) ? HIGH : LOW);
}

void setDirY(bool gora) {
    digitalWrite(DIR_Y, (gora ^ reverseY ? LOW : HIGH));
}

void setDirR(bool gora) {
    digitalWrite(DIR_R, (gora ^ reverseR ? LOW : HIGH));
}

void attachEnkoderY(bool attach) {
    if (attach) {
        attachInterrupt(digitalPinToInterrupt(ENCODER_Y_A), moveIntY, RISING);
        //attachInterrupt(digitalPinToInterrupt(ENCODER_Y_Z), obrotIntY, RISING);
    } else {
        detachInterrupt(digitalPinToInterrupt(ENCODER_Y_A));
        //detachInterrupt(digitalPinToInterrupt(ENCODER_Y_Z));
    }
}

void attachEnkoderX(bool attach) {
    if (attach) {
        attachInterrupt(digitalPinToInterrupt(ENCODER_X_A), moveIntX, RISING);
        //attachInterrupt(digitalPinToInterrupt(ENCODER_X_Z), obrotIntX, RISING);
    } else {
        detachInterrupt(digitalPinToInterrupt(ENCODER_X_A));
        //detachInterrupt(digitalPinToInterrupt(ENCODER_X_Z));
    }
}

void initEnkodery()
{
    pinMode(ENCODER_Y_A, INPUT);
    pinMode(ENCODER_Y_B, INPUT);
    //attachInterrupt(digitalPinToInterrupt(ENCODER_Y_A), moveIntY, RISING);

    pinMode(ENCODER_X_A , INPUT);
    pinMode(ENCODER_X_B , INPUT);
    //attachInterrupt(digitalPinToInterrupt(ENCODER_X_A), moveIntX, RISING);
    
    //pinMode(ENCODER_Y_Z, INPUT);
    //attachInterrupt(digitalPinToInterrupt(ENCODER_Y_Z), obrotIntY, RISING);

    //pinMode(ENCODER_X_Z, INPUT);
    //attachInterrupt(digitalPinToInterrupt(ENCODER_X_Z), obrotIntX, RISING);
}

void moveIntY()
{
    gActPosImpY += ((digitalRead(ENCODER_Y_B) == HIGH) ^ reverseY) ? +1 : -1;
    --gMoveImpY;
    canMoveY = gActPosImpY >= 0 && gActPosImpY <= gImpMaxY;
}

void moveIntX()
{
    gActPosImpX += ((digitalRead(ENCODER_X_B) == HIGH) ^ reverseX) ? -1 : +1;
    --gMoveImpX;
    canMoveX = gActPosImpX >= 0 && gActPosImpX <= gImpMaxX;
}

void obrotIntY()
{
    //Serial.print("dol=");
    //Serial.println(gMoveImpY, DEC);
    //if (!gPrevPosGora) {
    //    gPrevPosGora = gActPosImpY;
    //    return;
    //}

    //gActPosImpY = gPrevPosGora + 2000;
    //gPrevPosGora = gActPosImpY;
}

void obrotIntX()
{
    //Serial.print("lewo=");
    //Serial.println(gMoveImpX, DEC);
    //if (!gPrevPosLewo) {
    //    gPrevPosLewo = gActPosImpX;
    //    return;
    //}
    //gActPosImpX = gPrevPosLewo + 2000;
    //gPrevPosLewo = gActPosImpX;
}

void initSilniki() 
{
    pinMode(ENABLE_Y, OUTPUT);
    pinMode(DIR_Y, OUTPUT);
    pinMode(PULSE_Y, OUTPUT);
    digitalWrite(ENABLE_Y, LOW);
    digitalWrite(DIR_Y, LOW);
    digitalWrite(PULSE_Y, LOW);

    pinMode(ENABLE_X, OUTPUT);
    pinMode(DIR_X, OUTPUT);
    pinMode(PULSE_X, OUTPUT);
    digitalWrite(ENABLE_Y, LOW);
    digitalWrite(DIR_Y, LOW);
    digitalWrite(PULSE_Y, LOW);

    pinMode(ENABLE_R, OUTPUT);
    pinMode(DIR_R, OUTPUT);
    pinMode(PULSE_R, OUTPUT);
    digitalWrite(ENABLE_R, LOW);
    digitalWrite(DIR_R, LOW);
    digitalWrite(PULSE_R, LOW);

    delay(100);
    digitalWrite(ENABLE_Y, HIGH);
    digitalWrite(ENABLE_X, HIGH);
    digitalWrite(ENABLE_R, HIGH);
    delay(100);
    digitalWrite(ENABLE_Y, LOW);
    digitalWrite(ENABLE_X, LOW);
    digitalWrite(ENABLE_R, LOW);
}

uint32_t delayPulse(uint32_t step, uint32_t steps) {
    
    //return 1000; 
    if (step > steps)
        return freqs[0];

    if (step < sizeImpuls) 
        return freqs[step >> 6];

    if (step > steps - sizeImpuls) 
        return freqs[(steps - step - 1) >> 6];
      
    return freqs[sizeFreq - 1];
}

void setPosY(uint32_t pos)
{
#ifdef DEBUG
    Serial.print("gActPosImpY=");
    Serial.println(gActPosImpY, DEC);
#endif    
    msg.sendPositionStartY();
    
#ifdef TEST_SILNIKA
    delay(2000);
    msg.sendPositionDoneY(0, gActPosStepY);
    return;
#endif
    
    if (gActPosImpY == pos) {
        msg.sendPositionDoneY(0, gActPosStepY);
#ifdef DEBUG        
        Serial.println("gPos == pos");
#endif        
        return;
    }

    attachEnkoderY(true);
    
    if (gActPosImpY < 0)
        gActPosImpY = 0;

    bool goraK = gActPosImpY < pos;
    if (goraK) {
        gMoveImpY = pos - gActPosImpY;
    } else {
        gMoveImpY = gActPosImpY - pos;
    }
#ifdef DEBUG    
    Serial.print(goraK ? "Ruch w dol " : "Ruch w gore ");
    Serial.print(gMoveImpY, DEC);
    Serial.println(" impulsow\n---");
#endif

    uint32_t imps = gMoveImpY;
    setDirY(goraK);
    int32_t step = 0;
    //canMoveY = true;

    if (gMoveImpY < 2*sizeImpuls + 10) {
        while(gMoveImpY >= 0 && ++step < gStepMaxY) {
            stepY(20,500);
        }
    } else {
        while(gMoveImpY >= 0 && ++step < gStepMaxY) {
            stepY(20,delayPulse(imps - gMoveImpY, imps));
        }
    }
#ifdef DEBUG    
    Serial.print("gActPosImpY=");
    Serial.println(gActPosImpY, DEC);
    Serial.print("step=");
    Serial.println(step, DEC);
    Serial.print("canMoveY=");
    Serial.println(canMoveY,DEC);
    Serial.print("gStepMaxY=");
    Serial.println(gStepMaxY, DEC);
#endif    
    attachEnkoderY(false);
    gActPosStepY += goraK ? step : -step;
    msg.sendPositionDoneY(step, gActPosStepY);
}

void setPosX(uint32_t pos)
{
    msg.sendPositionStartX();
#ifdef DEBUG    
    Serial.print("gActPosImpX=");
    Serial.print(gActPosImpX, DEC);
#endif

#ifdef TEST_SILNIKA
    delay(2000);
    msg.sendPositionDoneX(0, gActPosStepX);
    return;
#endif

    if (gActPosImpX == pos) {
        msg.sendPositionDoneX(0, gActPosStepX);
#ifdef DEBUG        
        Serial.println("gActPosImpX == pos");
#endif        
        return;
    }
    attachEnkoderX(true);
    
    bool lewoK = gActPosImpX < pos;
    if (lewoK) {
        gMoveImpX = pos - gActPosImpX;
    } else {
        gMoveImpX = gActPosImpX - pos;
    }
    uint32_t imps = gMoveImpX;
#ifdef DEBUG
    Serial.println(lewoK ? "Ruch w Lewo " : "Ruch w prawo ");
    Serial.print(gMoveImpX, DEC);
    Serial.println(" impulsow\n---");
#endif
    setDirX(lewoK);
    int32_t step = 0;
    //canMoveX = true;

    if (gMoveImpX < 2*sizeImpuls + 10) {
        while(gMoveImpX >= 0 && step++ < gStepMaxX) {
            stepX(20, 500);
        }
    } else {
        while(gMoveImpX >= 0 && step++ < gStepMaxX) {
            stepX(20, delayPulse(imps - gMoveImpX, imps));
        }
    }

#ifdef DEBUG    
    Serial.print("gActPosImpX=");
    Serial.print(gActPosImpX, DEC);
    Serial.print("step=");
    Serial.println(step, DEC);
    Serial.print("canMoveX=");
    Serial.println(canMoveX,DEC);
    Serial.print("gStepMaxX=");
    Serial.println(gStepMaxX, DEC);
#endif

    attachEnkoderX(false);
    gActPosStepX += lewoK ? step : -step;
    msg.sendPositionDoneX(step, gActPosStepX);
}

void setPosR(uint32_t pos)
{
#ifdef DEBUG
    Serial.print("gActPosStepR=");
    Serial.println(gActPosStepRol, DEC);
#endif    
    msg.sendRoletaStart();
    
#ifdef TEST_SILNIKA
    delay(2000);
    msg.sendRoletaDone(gActPosStepRol);
    return;
#endif
    
    if (gActPosStepRol == pos) {
        msg.sendRoletaDone(0, gActPosStepRol);
#ifdef DEBUG        
        Serial.println("gPos == pos");
#endif        
        return;
    }
    
    if (gActPosStepRol < 0)
        gActPosStepRol = 0;

    bool goraK = gActPosStepRol < pos;
    if (goraK) {
        gMoveStepR = pos - gActPosStepRol;
    } else {
        gMoveStepR = gActPosStepRol - pos;
    }
#ifdef DEBUG
    Serial.print(goraK ? "Ruch w dol " : "Ruch w gore ");
    Serial.print(gMoveImpY, DEC);
    Serial.println(" impulsow\n---");
#endif

    uint32_t imps = gMoveStepR;
    setDirR(goraK);
    int32_t step = 0;
    //canMoveY = true;

    if (gMoveStepR < 2*sizeImpuls + 10) {
        while(gMoveStepR >= 0 && ++step < gStepMaxR) {
            stepR(20,500);
            --gMoveStepR;
        }
    } else {
        while(gMoveStepR >= 0 && ++step < gMoveStepR) {
            stepR(20,delayPulse(imps - gMoveStepR, imps));
            --gMoveStepR;
        }
    }
#ifdef DEBUG   
    Serial.print("gActPosStepRol=");
    Serial.println(gActPosStepRol, DEC);
    Serial.print("step=");
    Serial.println(step, DEC);
    Serial.print("gMoveStepR=");
    Serial.println(gMoveStepR, DEC);
#endif    

    gActPosStepRol += goraK ? step : -step;
    msg.sendPositionDoneY(step, gActPosStepRol);
}


void clearPos() {
    gActPosImpY = 0;
    gActPosImpX = 0;
}

void clearPosRol() {
    gActPosStepRol = 0;
}
//290674
//[16:40:48:385] POS=-68497
//[16:40:48:397] IMP=275794





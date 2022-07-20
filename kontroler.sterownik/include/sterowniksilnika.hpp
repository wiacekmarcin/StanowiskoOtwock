#ifndef STEROWNIK_SILNIKA_H_
#define STEROWNIK_SILNIKA_H_

//100 imp silnika na 1 mm
#define X_LEFT false
#define X_RIGHT true

#define Y_DOWN false
#define Y_UP true

#include "platform.h"
#include <Arduino.h>

void initCzujnikiKrancowe();

void setHomePosX();
void setHomePosY();

inline bool getHomePosX();
inline bool getHomePosY();

bool returnBaseX();
bool returnBaseY();

void setDirX(bool lewo);
void setDirY(bool gora);

void attachEnkoderY(bool attach);
void attachEnkoderX(bool attach);

void initEnkodery();

void obrotIntY();
void obrotIntX();

void moveIntY();
void moveIntX();

void initSilniki();

uint32_t delayPulse(uint32_t step, uint32_t steps);

void setPosY(uint32_t pos);

void setPosX(uint32_t pos);

void clearPos();

void checkMaxImpuls();

//void printPos();


inline void stepX(uint16_t delay1, uint16_t delay2);
inline void stepY(uint16_t delay1, uint16_t delay2);

#endif // !STEROWNIK_SILNIKA_H_
#include "mechanika.h"

Ruch::Ruch() :
    impulsyXperMM(24.96), impulsyYperMM(24.96), krokiXperMM(100), krokiYperMM(100),
    maxImpulsyX(0), maxImpulsyY(0), maxKrokiX(0), maxKrokiY(0), maxXmm(0), maxYmm(0),
    wentOffsetX(301),
    wentOffsetY(175.58),
    wentKatNach(3.85)
{
     calculate();
}

void Ruch::setPrzestrzen(unsigned int xmm, unsigned int ymm)
{
    maxXmm = xmm;
    maxYmm = ymm;
    calculate();
}

void Ruch::setImpusyXPerMM(double imp)
{
    //impulsyXperMM = imp;
    calculate();
}

void Ruch::setImpusyYPerMM(double imp)
{
    //impulsyYperMM = imp;
    calculate();
}

void Ruch::setKrokiXPerMM(double krok)
{
    //krokiXperMM = krok;
    calculate();
}

void Ruch::setKrokiYPerMM(double krok)
{
    //krokiYperMM = krok;
    calculate();
}

void Ruch::calculate() 
{
    maxImpulsyX = maxXmm * impulsyXperMM;
    maxImpulsyY = maxYmm * impulsyYperMM;
    maxKrokiX = maxXmm * krokiXperMM;
    maxKrokiY = maxYmm * krokiYperMM;
    //TODO send settings to kontroler


    timeoutX = (unsigned long) 2.05*maxKrokiX + 100; //2000+50 us
    timeoutY = (unsigned long) 2.05*maxKrokiY + 100;
}

unsigned int Ruch::getMaxYmm() const
{
    return maxYmm;
}

void Ruch::setMaxYmm(unsigned int value)
{
    maxYmm = value;
}

unsigned int Ruch::getMaxXmm() const
{
    return maxXmm;
}

void Ruch::setMaxXmm(unsigned int value)
{
    maxXmm = value;
}

bool Ruch::getReverseY() const
{
    return reverseY;
}

void Ruch::setReverseY(bool value)
{
    reverseY = value;
}

bool Ruch::getReverseX() const
{
    return reverseX;
}

void Ruch::setReverseX(bool value)
{
    reverseX = value;
}

double Ruch::getWentKatNach() const
{
    return wentKatNach;
}

void Ruch::setWentKatNach(double value)
{
    wentKatNach = value;
}

uint32_t Ruch::getX(unsigned int mmx)
{
    return impulsyXperMM * mmx;
}

uint32_t Ruch::getY(unsigned int mmy)
{
    return impulsyYperMM * mmy;
}

int Ruch::getWentX(unsigned int mmx, unsigned int mmL)
{
    return (int) (mmx + mmL + wentOffsetX);
}

int Ruch::getWentY(unsigned int mmy)
{
    double res  = (1.0 * mmy / 3.85) - wentOffsetY;
    return (int) res;
}

double Ruch::getWentOffsetY() const
{
    return wentOffsetY;
}

void Ruch::setWentOffsetY(double value)
{
    wentOffsetY = value;
}

double Ruch::getWentOffsetX() const
{
    return wentOffsetX;
}

void Ruch::setWentOffsetX(double value)
{
    wentOffsetX = value;
}


#include "mechanika.h"

Ruch::Ruch() :
    impulsyXperMM(25), impulsyYperMM(25), krokiXperMM(10), krokiYperMM(10), 
    maxImpulsyX(0), maxImpulsyY(0), maxKrokiX(0), maxKrokiY(0), maxXmm(0), maxYmm(0)
{

}

void Ruch::setPrzestrzen(unsigned int xmm, unsigned int ymm)
{
    maxXmm = xmm;
    maxYmm = ymm;
    calculate();
}

void Ruch::setImpusyXPerMM(unsigned int imp) 
{
    impulsyXperMM = imp;
    calculate();
}

void Ruch::setImpusyYPerMM(unsigned int imp) 
{
    impulsyYperMM = imp;
    calculate();
}

void Ruch::setKrokiXPerMM(unsigned int krok) 
{
    krokiXperMM = krok;
    calculate();
}

void Ruch::setKrokiYPerMM(unsigned int krok) 
{
    krokiYperMM = krok;
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


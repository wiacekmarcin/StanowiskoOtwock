#include "mechanika.h"

unsigned int RoletaRuch::obrotStala10xmm[14] = {0, 1013, 1059, 1089, 1124, 1158, 1186, 1219, 1255, 1284, 1321, 1350, 1387, 1419};


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


RoletaRuch::RoletaRuch() : krokiPerObrot(12000), maxKroki(13*12000)
{

}

unsigned int RoletaRuch::getKrokiPerObrot() const
{
    return krokiPerObrot;
}

void RoletaRuch::setKrokiPerObrot(unsigned int newKrokiPerObrot)
{
    krokiPerObrot = newKrokiPerObrot;
}

unsigned long RoletaRuch::podniescMM(unsigned int mm)
{
    unsigned short n = 0;

    if (mm > maxMM) {
        return maxKroki;
    }

    unsigned long MM = 10*mm;
    unsigned long obwod;
    while (n < 14) {
        obwod = obrotStala10xmm[n];
        if (MM < obwod) {
            return krokiPerObrot*n + (unsigned long)(12000*MM/obwod);
        }
        MM -= obwod;
        ++n;
    }
    return maxKroki;
}

unsigned int RoletaRuch::getMaxMM() const
{
    return maxMM;
}

void RoletaRuch::setMaxMM(unsigned int newMaxMM)
{
    maxMM = newMaxMM;
}

unsigned long RoletaRuch::getMaxKroki() const
{
    return maxKroki;
}

void RoletaRuch::setMaxKroki(unsigned long newMaxKroki)
{
    maxKroki = newMaxKroki;
}

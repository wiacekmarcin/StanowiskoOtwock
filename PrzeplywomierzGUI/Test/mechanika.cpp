#include "mechanika.h"
#include <QDebug>

unsigned int RoletaRuch::obrotStala10xmm[14] = {1000, 1020, 1075, 1114, 1152, 1185, 1240, 1278, 1342, 1401, 1425, 1465, 1503, 1516};


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
    \\QDebug() << "mm=" << mm << "maxMM" << maxMM;
    unsigned short n = 0;
    maxMM = 1600;
    if (mm > maxMM) {
        return maxKroki;
    }

    unsigned long MM = 10*mm;
    unsigned long obwod;

    while (n < 13) {
        \\QDebug() << "MM=" << MM << " obwod=" << obrotStala10xmm[n+1];
        obwod = obrotStala10xmm[n+1];
        if (MM < obwod) {
            \\QDebug() << "Return=" << (12000*n + (unsigned long)(12000*MM/obwod));
            return 12000*n + (unsigned long)(12000*MM/obwod) + 500*mm/1500;
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

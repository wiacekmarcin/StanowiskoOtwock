#include "mechanika.h"
#include <QDebug>

//unsigned int RoletaRuch::obrotStala10xmm[14] = {0, 1013, 1059, 1089, 1124, 1158, 1186, 1219, 1255, 1284, 1321, 1350, 1387, 1419};
unsigned int RoletaRuch::obrotStala10xmm[14] = {1000, 1020, 1075, 1114, 1152, 1185, 1240, 1278, 1342, 1401, 1425, 1465, 1503, 1516};


Ruch::Ruch() :
    impulsyXperMM(24.96), impulsyYperMM(24.96), krokiXperMM(100), krokiYperMM(100),
    maxImpulsyX(0), maxImpulsyY(0), maxKrokiX(0), maxKrokiY(0), maxXmm(0), maxYmm(0),
    wentOffsetX(301),
    wentOffsetY(175.58),
    wentKatNach(3.85),
    reverseR(true)
{

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

void Ruch::setMaxKrokiR(unsigned long newMaxKrokiR)
{
    maxKrokiR = newMaxKrokiR;
}

unsigned int Ruch::getMaxYmm() const
{
    return maxYmm;
}

void Ruch::setMaxYmm(unsigned int value)
{
    maxYmm = value;
    calculate();
}

unsigned int Ruch::getMaxXmm() const
{
    return maxXmm;
}

void Ruch::setMaxXmm(unsigned int value)
{
    maxXmm = value;
    calculate();
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

bool Ruch::getReverseR() const
{
    return reverseR;
}

void Ruch::setReverseR(bool value)
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
    qDebug() << "mm=" << mm << "maxMM" << maxMM;
    unsigned short n = 0;
    maxMM = 1600;
    if (mm > maxMM) {
        return maxKroki;
    }

    unsigned long MM = 10*mm;
    unsigned long obwod;
    unsigned int prevobwod = 1000;
    while (n < 13) {
        qDebug() << "MM=" << MM << " obwod=" << obrotStala10xmm[n+1];
        obwod = obrotStala10xmm[n+1];
        if (MM < obwod) {
            qDebug() << "Return=" << (12000*n + (unsigned long)(12000*MM/obwod));
            return 12000*n + (unsigned long)(12000*MM/obwod) + 500*mm/1500;
        }
        MM -= obwod;
        prevobwod = obwod;
        ++n;
    }
    return maxKroki;


/*




    unsigned short n = 0;

    if (mm > maxMM) {
        return maxKroki;
    }

    unsigned long MM = 10*mm;
    unsigned long obwod;
    while (n < 13) {
        obwod = obrotStala10xmm[n+1];
        if (MM < obwod) {
            return krokiPerObrot*n + (unsigned long)(12000*MM/obwod);
        }
        MM -= obwod;
        ++n;
    }
    return maxKroki;
    */
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

unsigned int RoletaRuch::getOffsetX() const
{
    return offsetX;
}

void RoletaRuch::setOffsetX(unsigned int newOffsetX)
{
    offsetX = newOffsetX;
}

unsigned int RoletaRuch::getOffsetY() const
{
    return offsetY;
}

void RoletaRuch::setOffsetY(unsigned int newOffsetY)
{
    offsetY = newOffsetY;
}

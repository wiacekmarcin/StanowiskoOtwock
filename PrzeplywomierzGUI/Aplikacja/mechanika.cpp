#include "mechanika.h"
#include <QDebug>

//unsigned int RoletaRuch::obrotStala10xmm[14] = {0, 1013, 1059, 1089, 1124, 1158, 1186, 1219, 1255, 1284, 1321, 1350, 1387, 1419};
/*unsigned int RoletaRuch::obrotStala10xmm[21] = {1000, 1020, 1075, 1114, 1152, 1185,
                                                1240, 1278, 1342, 1401, 1425, 1465,
                                                1503, 1516, 1516, 1516, 1516, 1516,
                                                1516, 1516, 1516};
*/
unsigned int RoletaRuch::obrotStala10xmm[Ustawienia::getMaxRolObrot()] = {
                                                 800,  780,  830,  910,  910,
                                                 950, 1060, 1020, 1050, 1160,
                                                1150, 1200, 1260, 1270,    0,
                                                   0,    0,    0,    0,    0,
                                                   0,    0,    0,    0,    0,
                                                   0,    0,    0,    0,    0
                                                };

Ruch::Ruch() :
    impulsyXperMM(24.96), impulsyYperMM(24.96), krokiXperMM(100), krokiYperMM(100),
    maxImpulsyX(0), maxImpulsyY(0), maxKrokiX(0), maxKrokiY(0), maxXmm(0), maxYmm(0),
    wentOffsetX(301),
    wentOffsetY(175.58),
    wentKatNach(3.85),
    reverseR(true)
{

}

void Ruch::setUstawienia(Ustawienia &ust)
{
    QString val1 = ust.getImpulsyXperMM();
    if (!val1.isEmpty())
        setImpusyXPerMM(val1.toFloat());
    else
        ust.setImpulsyXperMM(QString::number(impulsyXperMM));

    QString val2 = ust.getImpulsyYperMM();
    if (!val2.isEmpty())
        setImpusyYPerMM(val2.toFloat());
    else
        ust.setImpulsyYperMM(QString::number(impulsyYperMM));

    QString val3 = ust.getKrokiXperMM();
    if (!val3.isEmpty())
        setKrokiXPerMM(val3.toFloat());
    else
        ust.setKrokiXperMM(QString::number(krokiXperMM));

    QString val4 = ust.getKrokiYperMM();
    if (!val4.isEmpty())
        setKrokiYPerMM(val4.toFloat());
    else
        ust.setKrokiYperMM(QString::number(krokiYperMM));

    QString val5 = ust.getWentOffsetX();
    if (!val5.isEmpty())
        setWentOffsetX(val5.toDouble());
    else
        ust.setWentOffsetX(QString::number(wentOffsetX));


    QString val6 = ust.getWentOffsetY();
    if (!val6.isEmpty())
        setWentOffsetY(val6.toDouble());
    else
        ust.setWentOffsetX(QString::number(wentOffsetY));

    QString val7 = ust.getKatnachylenia();
    if (!val7.isEmpty())
        setWentKatNach(val7.toDouble());
    else
        ust.setKatnachylenia(QString::number(wentKatNach));
}

void Ruch::setPrzestrzen(unsigned int xmm, unsigned int ymm)
{
    maxXmm = xmm;
    maxYmm = ymm;
    calculate();
}

void Ruch::setImpusyXPerMM(double imp)
{
    impulsyXperMM = imp;
    calculate();
}

void Ruch::setImpusyYPerMM(double imp)
{
    impulsyYperMM = imp;
    calculate();
}

void Ruch::setKrokiXPerMM(double krok)
{
    krokiXperMM = krok;
    calculate();
}

void Ruch::setKrokiYPerMM(double krok)
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
    reverseR = value;
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

RoletaRuch::RoletaRuch() :
    krokiPerObrot(2000),
    maxMM(1500),
    maxKroki(29100)
{

}

void RoletaRuch::setReverse(bool rev)
{
    reverse = rev;
}

#define SETVALUE(GF,SF,value,conF) do { \
    QString val = ust.GF();\
    if (!val.isEmpty()) \
        value = (val.conF());\
    else\
        ust.setRolDlugosc(QString::number(value));\
    } while(false)

void RoletaRuch::setUstawienia(Ustawienia &ust)
{
    SETVALUE(getRolDlugosc,setRolDlugosc,maxMM,toUInt);
    SETVALUE(getRolStepObrot,setRolStepObrot,krokiPerObrot,toFloat);
    SETVALUE(getRolOffsetX_P,setRolOffsetX_P,offsetX_P,toUInt);
    SETVALUE(getRolOffsetY_P,setRolOffsetY_P,offsetY_P,toUInt);
    SETVALUE(getRolOffsetX_L,setRolOffsetX_L,offsetX_L,toUInt);
    SETVALUE(getRolOffsetY_L,setRolOffsetY_L,offsetY_L,toUInt);
    SETVALUE(getMaxRolKroki,setMaxRolKroki,maxKroki,toUInt);

    QString val;
    for (short i = 0; i < Ustawienia::getMaxRolObrot(); i++) {
        val = ust.getRolObrot(i);
        //QDebug() << val;
        if (!val.isEmpty()) {
            setImpPerObrot(i, val);
        } else {
            ust.setRolObrot(i, QString::number(getObrot(i)));
        }
    }
}

void RoletaRuch::setImpPerObrot(unsigned short obr, const QString & impls)
{
    bool ok;
    float value = impls.toFloat(&ok);
    if (!impls.isEmpty() && ok)
        setObrot(obr, value);
    else
        setObrot(obr, 0);
}

void RoletaRuch::setObrot(unsigned int obr, float value)
{
    obrotStala10xmm[obr] = (unsigned int) 10*value;
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
    
    if (krokiPerObrot != 2000)
        krokiPerObrot = 2000;

    unsigned long kroki = 0L;
    unsigned long MM = 10*mm;
    unsigned long obwod;
    unsigned long heightMM = 0;

    while (n < Ustawienia::getMaxRolObrot())
    {

        obwod = obrotStala10xmm[n];
        if (obwod == 0) {
            //jestesmy pod koniec obrotow
            int krokidoKonca = maxKroki - kroki;
            int mmDoKonca = maxMM-heightMM/10;
            if (mmDoKonca <= 0)
                return maxKroki;
            int krokiWyznaczone = krokidoKonca*MM/mmDoKonca/10;
            if (krokiWyznaczone < 0)
                krokiWyznaczone = 0;
            kroki += krokiWyznaczone;
            if (kroki > maxKroki)
                return maxKroki;
            return kroki;
        }

        if (MM < obwod) {
            kroki += (unsigned long)(krokiPerObrot*MM/obwod);
            if (kroki > maxKroki)
                return maxKroki;
            return kroki;
        } else {
            kroki += krokiPerObrot;
            if (kroki > maxKroki)
                return maxKroki;
        }

        MM -= obwod;
        heightMM += obwod;
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

unsigned int RoletaRuch::getOffsetX_P() const
{
    return offsetX_P;
}

void RoletaRuch::setOffsetX_P(unsigned int newOffsetX)
{
    offsetX_P = newOffsetX;
}

unsigned int RoletaRuch::getOffsetY_P() const
{
    return offsetY_P;
}

void RoletaRuch::setOffsetY_P(unsigned int newOffsetY)
{
    offsetY_P = newOffsetY;
}

unsigned int RoletaRuch::getOffsetX_L() const
{
    return offsetX_L;
}

void RoletaRuch::setOffsetX_L(unsigned int newOffsetX)
{
    offsetX_L = newOffsetX;
}

unsigned int RoletaRuch::getOffsetY_L() const
{
    return offsetY_L;
}

void RoletaRuch::setOffsetY_L(unsigned int newOffsetY)
{
    offsetY_L = newOffsetY;
}

bool RoletaRuch::getReverse() const
{
    return reverse;
}

#include "mechanika.h"
#include <QDebug>

//unsigned int RoletaRuch::obrotStala10xmm[14] = {0, 1013, 1059, 1089, 1124, 1158, 1186, 1219, 1255, 1284, 1321, 1350, 1387, 1419};
/*unsigned int RoletaRuch::obrotStala10xmm[21] = {1000, 1020, 1075, 1114, 1152, 1185,
                                                1240, 1278, 1342, 1401, 1425, 1465,
                                                1503, 1516, 1516, 1516, 1516, 1516,
                                                1516, 1516, 1516};
*/
unsigned int RoletaRuch::obrotStala10xmm[Ustawienia::getMaxRolObrot()] = {1020, 1075, 1114, 1152, 1185,
                                                1240, 1278, 1342, 1401, 1425,
                                                1465, 1503, 1516,    0,    0,
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
        setImpusyXPerMM(val1.toUInt());
    else
        ust.setImpulsyXperMM(QString::number(impulsyXperMM));

    QString val2 = ust.getImpulsyYperMM();
    if (!val2.isEmpty())
        setImpusyYPerMM(val2.toUInt());
    else
        ust.setImpulsyYperMM(QString::number(impulsyYperMM));

    QString val3 = ust.getKrokiXperMM();
    if (!val3.isEmpty())
        setKrokiXPerMM(val3.toUInt());
    else
        ust.setKrokiXperMM(QString::number(krokiXperMM));

    QString val4 = ust.getKrokiYperMM();
    if (!val4.isEmpty())
        setKrokiYPerMM(val4.toUInt());
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

RoletaRuch::RoletaRuch() :
    krokiPerObrot(12000),
    maxMM(1600),
    maxKroki(13*12000)
{

}

void RoletaRuch::setReverse(bool rev)
{
    reverse = rev;
}

void RoletaRuch::setUstawienia(Ustawienia &ust)
{
    QString val1 = ust.getRolDlugosc();
    if (!val1.isEmpty())
        maxMM = (val1.toUInt());
    else
        ust.setRolDlugosc(QString::number(maxMM));

    QString val2 = ust.getRolStepObrot();
    if (!val2.isEmpty())
        krokiPerObrot = (val2.toUInt());
    else
        ust.setRolStepObrot(QString::number(krokiPerObrot));

    QString val3 = ust.getRolOffsetX_P();
    if (!val3.isEmpty())
        offsetX_P = (val3.toUInt());
    else
        ust.setRolOffsetX_P(QString::number(offsetX_P));

    QString val4 = ust.getRolOffsetY_P();
    if (!val4.isEmpty())
        offsetY_P = (val4.toUInt());
    else
        ust.setRolOffsetX_P(QString::number(offsetY_P));

    QString val5 = ust.getRolOffsetX_L();
    if (!val5.isEmpty())
        offsetX_L = (val3.toUInt());
    else
        ust.setRolOffsetX_P(QString::number(offsetX_L));

    QString val6 = ust.getRolOffsetY_L();
    if (!val4.isEmpty())
        offsetY_L = (val6.toUInt());
    else
        ust.setRolOffsetX_P(QString::number(offsetY_L));


    QString val;
    for (short i = 0; i < Ustawienia::getMaxRolObrot(); i++) {
        val = ust.getRolObrot(i);
        qDebug() << val;
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
    //qDebug() << "mm=" << mm << "maxMM" << maxMM;
    unsigned short n = 0;
    maxMM = 1600;
    if (mm > maxMM) {
        return maxKroki;
    }

    unsigned long steps = 0L;
    unsigned long MM = 10*mm;
    unsigned long obwod;

    while (n < Ustawienia::getMaxRolObrot()) {
        //qDebug() << "MM=" << MM << " obwod=" << obrotStala10xmm[n+1];
        obwod = obrotStala10xmm[n];
        if (MM < obwod) {
            //qDebug() << "Return=" << (12000*n + (unsigned long)(12000*MM/obwod));
            return steps + (unsigned long)(krokiPerObrot*MM/obwod) + 500*mm/1500; //ostatnie to stala...
        } else if (obwod == 0) {
            return steps;
        } else {
            steps += krokiPerObrot;
            if (steps > maxKroki)
                return maxKroki;
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


void RoletaRuch::calculate()
{
    maxKroki = krokiPerObrot;
    for (short i = 0; i < Ustawienia::getMaxRolObrot(); i++) {
        if (obrotStala10xmm[i] > 0)
            maxKroki += krokiPerObrot;
        else
            break;
    }
}

bool RoletaRuch::getReverse() const
{
    return reverse;
}

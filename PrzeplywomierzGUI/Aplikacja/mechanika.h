#ifndef MECHANIKA_H
#define MECHANIKA_H

#include <QString>

class Ruch
{
public:
    Ruch();

    void setPrzestrzen(unsigned int xmm, unsigned int ymm);

    unsigned long getTimeoutX() const { return timeoutX; }
    unsigned long getTimeoutY() const { return timeoutY; }

    unsigned long getImpulsyX(unsigned int mm) const { return 1L * impulsyXperMM * mm; };
    unsigned long getImpulsyY(unsigned int mm) const { return 1L * impulsyYperMM * mm; };

    unsigned long getMaxImpusyX() const { return maxImpulsyX; }
    unsigned long getMaxImpusyY() const { return maxImpulsyY; }

    unsigned long getMaxKrokiX() const { return maxKrokiX; }
    unsigned long getMaxKrokiY() const { return maxKrokiY; }
    unsigned long getMaxKrokiR() const { return maxKrokiR; }

    void setImpusyXPerMM(double impx);
    void setImpusyYPerMM(double impy);

    void setKrokiXPerMM(double krokix);
    void setKrokiYPerMM(double krokiy);

    double getImpusyXPerMM() const { return impulsyXperMM; };
    double getImpusyYPerMM() const { return impulsyYperMM; };

    double getKrokiXPerMM() const { return krokiXperMM; };
    double getKrokiYPerMM() const { return krokiYperMM; };

    double getWentOffsetX() const;
    void setWentOffsetX(double value);

    double getWentOffsetY() const;
    void setWentOffsetY(double value);

    double getWentKatNach() const;
    void setWentKatNach(double value);

    uint32_t getX(unsigned int mmx);
    uint32_t getY(unsigned int mmy);

    int getWentX(unsigned int mmx, unsigned int mmL);
    int getWentY(unsigned int mmy);

    bool getReverseX() const;
    void setReverseX(bool value);

    bool getReverseY() const;
    void setReverseY(bool value);

    bool getReverseR() const;
    void setReverseR(bool value);

    unsigned int getMaxXmm() const;
    void setMaxXmm(unsigned int value);

    unsigned int getMaxYmm() const;
    void setMaxYmm(unsigned int value);

    void calculate();
    void setMaxKrokiR(unsigned long newMaxKrokiR);

protected:


private:
    double impulsyXperMM; //ilosc impulsow na 1mm - 25 na osi X (lewo - prawo)
    double impulsyYperMM; //ilosc impulsow na 1mm - 25 na osi Y (góra - dół)

    double krokiXperMM; //ilość kroków na 1 mm - 10 na osi X (lewo - prawo)
    double krokiYperMM; //ilość kroków na 1 mm - 10 na osi Y (gora - dół)

    //maksymalne ilosci impulsow i krokow ze wzoru przestrzen robocza w mm * impulsy?perMM
    // przestrzen robocza * kroki?perMM
    unsigned long maxImpulsyX;
    unsigned long maxImpulsyY;

    unsigned long maxKrokiX;
    unsigned long maxKrokiY;
    unsigned long maxKrokiR;

    unsigned int maxXmm; //maksymalna ilosc mm
    unsigned int maxYmm; //maksymalna ilosc mm

    unsigned long timeoutX; //ilosc czasu potrzebnego w ms na ruch liczony maks ilosc krokow razy 2ms
    unsigned long timeoutY;

    double wentOffsetX;
    double wentOffsetY;
    double wentKatNach;

    bool reverseX;
    bool reverseY;
    bool reverseR;
};

#define ObrotN(N) void setObrot##N(const QString & obr##N) { obrotStala10xmm[N]=10*obr##N.toUInt(); }\
    unsigned int getObrotx10##N() const { return obrotStala10xmm[N]; } \
    float getObrot##N() const { return 0.1*obrotStala10xmm[N]; }

class RoletaRuch {
public:
    RoletaRuch();

    ObrotN(1)
    ObrotN(2)
    ObrotN(3)
    ObrotN(4)
    ObrotN(5)
    ObrotN(6)
    ObrotN(7)
    ObrotN(8)
    ObrotN(9)
    ObrotN(10)
    ObrotN(11)
    ObrotN(12)
    ObrotN(13)

    unsigned int getKrokiPerObrot() const;
    void setKrokiPerObrot(unsigned int newKrokiPerObrot);

    unsigned long podniescMM(unsigned int mm);
    unsigned long poniescPercent(float percent) {
        return podniescMM((unsigned int)(percent * maxMM / 100));
    }

    unsigned int getMaxMM() const;
    void setMaxMM(unsigned int newMaxMM);

    unsigned long getMaxKroki() const;
    void setMaxKroki(unsigned long newMaxKroki);

    unsigned int getOffsetX() const;
    void setOffsetX(unsigned int newOffsetX);

    unsigned int getOffsetY() const;
    void setOffsetY(unsigned int newOffsetY);

private:
    static unsigned int obrotStala10xmm[14];
    unsigned int krokiPerObrot;
    unsigned int maxMM;
    unsigned long maxKroki;

    unsigned int offsetX;
    unsigned int offsetY;
};

#endif // MECHANIKA_H

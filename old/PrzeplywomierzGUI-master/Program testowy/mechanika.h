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

    void setImpusyXPerMM(unsigned int imp);
    void setImpusyYPerMM(unsigned int imp);

    void setKrokiXPerMM(unsigned int krok);
    void setKrokiYPerMM(unsigned int krok);

protected:
    void calculate();

private:
    unsigned int impulsyXperMM; //ilosc impulsow na 1mm - 25 na osi X (lewo - prawo)
    unsigned int impulsyYperMM; //ilosc impulsow na 1mm - 25 na osi Y (góra - dół)

    unsigned int krokiXperMM; //ilość kroków na 1 mm - 10 na osi X (lewo - prawo)
    unsigned int krokiYperMM; //ilość kroków na 1 mm - 10 na osi Y (gora - dół)

    //maksymalne ilosci impulsow i krokow ze wzoru przestrzen robocza w mm * impulsy?perMM
    // przestrzen robocza * kroki?perMM
    unsigned long maxImpulsyX;
    unsigned long maxImpulsyY;

    unsigned long maxKrokiX;
    unsigned long maxKrokiY;

    unsigned int maxXmm; //maksymalna ilosc mm
    unsigned int maxYmm; //maksymalna ilosc mm

    unsigned long timeoutX; //ilosc czasu potrzebnego w ms na ruch liczony maks ilosc krokow razy 2ms
    unsigned long timeoutY;
};

#endif // MECHANIKA_H

#include "pozycje.h"
#include <QDebug>
Pozycje::Pozycje()
 : QList<Pos>()
{

}

void Pozycje::append(unsigned int time, unsigned int x, unsigned int y)
{
    Pos a;
    a.time = time;
    a.x = x;
    a.y = y;
    QList::append(a);
}

PozycjeRol::PozycjeRol(bool reverse, unsigned int maxEtap_, unsigned int maxR_, unsigned int measTime_, unsigned int stableTime_,
                       unsigned int width_, unsigned int offsetX_, unsigned int offsetY_)
    : QList<PosRoleta>(),
      reverseX(reverse),
      maxEtap(maxEtap_),
      maxR(maxR_),
      measTime(measTime_),
      stableTime(stableTime_),
      width(width_),
      offsetX(offsetX_),
      offsetY(offsetY_)
{
    float A5 = 0;
    float B5 = 0.212;
    float C5 = 0.426;
    float N5[5]{-C5, -B5, A5, B5, C5};
    initNorma(5, N5);

    float A6 = 0.063;
    float B6 = 0.265;
    float C6 = 0.439;
    float N6[6] = {-C6, -B6, -A6, A6, B6, C6};
    initNorma(6, N6);

    float A7 = 0.0;
    float B7 = 0.134;
    float C7 = 0.297;
    float D7 = 0.447;
    float N7[7] = {-D7, -C7, -B7, A7, B7, C7, D7};
    initNorma(7, N7);

    float A8 = 0.098;
    float B8 = 0.278;
    float C8 = 0.416;
    float D8 = 0.490;
    float N8[8] = {-D8, -C8, -B8, -A8, A8, B8, C8, D8};
    initNorma(8, N8);

    float A9 = 0.0;
    float B9 = 0.171;
    float C9 = 0.321;
    float D9 = 0.433;
    float E9 = 0.492;
    float N9[9] = {-E9, -D9, -C9, -B9, A9, B9, C9, D9, E9};
    initNorma(9, N9);

    float A10 = 0.078;
    float B10 = 0.227;
    float C10 = 0.354;
    float D10 = 0.446;
    float E10 = 0.494;

    float N10[10] = {-E10, -D10, -C10, -B10, -A10, A10, B10, C10, D10, E10};
    initNorma(10, N10);
    width= 800;
}

void PozycjeRol::appendRoletaEtap(unsigned int roletaEtap)
{
    PosRoleta p;
    p.mmx = 0;
    p.mmy = 0;
    p.nx = 0;
    p.ny = 0;
    p.etap = roletaEtap+1;
    p.mmr = maxR*(roletaEtap+1)/maxEtap;
    p.stableTime = stableTime;
    p.measTime = 0;
    p.measPoint = false;
    append(p);
}

void PozycjeRol::appendPoints(unsigned int roletaEtap, unsigned int xNorma, unsigned int yNorma)
{
    //qDebig() << "appendPoints REtap=" << roletaEtap << " Norma[x,y] = " << xNorma << ", " << yNorma;
    //qDebig() << "offset" << offsetX << offsetY;
    float wysokosc = maxR*(roletaEtap+1)/maxEtap;
    //qDebig() << "Wysokosc" << wysokosc;
    //qDebig() << "reverse" << reverseX;
    for (unsigned int i = 0; i < xNorma; ++i ) {
        for (unsigned int j = 0; j < yNorma; ++j ) {
            PosRoleta p;
            p.nx = norma[xNorma-5][i];
            p.ny = norma[yNorma-5][j];

            p.mmx = (reverseX) ? (offsetX - width * (0.5 - p.nx) ) : ( offsetX + width * (0.5 - p.nx) );
            p.mmy = offsetY - wysokosc * (0.5 + p.ny);
            if (p.mmy < 0) {
                p.mmy = 0;
            }
            if (p.mmx < 0) {
                p.mmx = 0;
            }
            p.mmr = (unsigned int)wysokosc;
            p.etap = roletaEtap+1;
            p.stableTime = 0;
            p.measTime = measTime;
            p.measPoint = true;
            append(p);
        }
    }
}

void PozycjeRol::setInit(unsigned int maxEtap_, unsigned int maxR_,
                         unsigned int width_, unsigned int measTime_,
                         unsigned int stableTime_)
{
    //qDebig() << "SetInit" << maxEtap_ << maxR_ << width_ << measTime_ << stableTime_;
    maxEtap = maxEtap_;
    maxR = maxR_;
    measTime = measTime_;
    stableTime = stableTime_;
    width = width_;
    width = 800;
}

void PozycjeRol::setOffset(bool reverse, unsigned int offSetX_, unsigned int offSetY_)
{
    //qDebig() << "ofsset" << offSetX_ << offSetY_ << reverse;
    reverseX = reverse;
    offsetX = offSetX_;
    offsetY = offSetY_;
}

void PozycjeRol::appendPoint(unsigned int roletaEtap, float xNorma, float yNorma, unsigned int mt)
{
    float wysokosc = maxR*(roletaEtap+1)/maxEtap;
    //qDebig() << "Wysokosc" << wysokosc;
    //qDebig() << "offset" << offsetX << offsetY;
    PosRoleta p;
    p.nx = xNorma;
    p.ny = yNorma;
    p.mmx = width * (0.5 - p.nx) + offsetX;
    p.mmy = wysokosc * (0.5 - p.ny) + (offsetY-wysokosc);
    p.mmr = (unsigned int)wysokosc;
    p.etap = roletaEtap+1;
    p.stableTime = 0;
    p.measTime = mt;
    p.measPoint = true;
    append(p);
}

void PozycjeRol::initNorma(unsigned int n, float *normasrc)
{
    if (n == 5) {
        for (short i = 0; i < 5; i++) {
            norma5[i] = normasrc[i];
        }
        norma[0] = norma5;
    }
    if (n == 6) {
        for (short i = 0; i < 6; i++) {
            norma6[i] = normasrc[i];
        }
        norma[1] = norma6;
    }
    if (n == 7) {
        for (short i = 0; i < 7; i++) {
            norma7[i] = normasrc[i];
        }
        norma[2] = norma7;
    }
    if (n == 8) {
        for (short i = 0; i < 8; i++) {
            norma8[i] = normasrc[i];
        }
        norma[3] = norma8;
    }
    if (n == 9) {
        for (short i = 0; i < 9; i++) {
            norma9[i] = normasrc[i];
        }
        norma[4] = norma9;
    }
    if (n == 10) {
        for (short i = 0; i < 10; i++) {
            norma10[i] = normasrc[i];
        }
        norma[5] = norma10;
    }
}

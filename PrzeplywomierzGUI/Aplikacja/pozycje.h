#ifndef POZYCJE_H
#define POZYCJE_H

#include <QList>

typedef struct _pos {
    unsigned int time;
    unsigned int x;
    unsigned int y;
} Pos;


class Pozycje : public QList<Pos>
{
public:
    Pozycje();
    void append(unsigned int time, unsigned int x, unsigned int y);



private:

};

typedef struct _posR {
    int mmx;
    int mmy;
    float nx;
    float ny;
    unsigned short etap;
    unsigned int mmr;
    unsigned int stableTime;
    unsigned int measTime;
    bool measPoint; //czy punkt pomiarowy czy podniesienie rolety;
} PosRoleta;

class PozycjeRol : public QList<PosRoleta>
{
public:
    PozycjeRol(bool reverse = false, unsigned int maxEtap_ = 0, unsigned int maxR_ = 0, unsigned int measTime = 0, unsigned int stableTime = 0,
               unsigned int width_ = 0, unsigned int offsetX_ = 0, unsigned int offsetY_ = 0);
    void appendRoletaEtap(unsigned int roletaEtap);
    void appendPoints(unsigned int roletaEtap, unsigned int xNorma, unsigned int yNorma);
    void setInit(unsigned int maxEtap_, unsigned int maxR_ , unsigned int width_, unsigned int measTime, unsigned int stableTime);
    void setOffset(bool reverse, unsigned int offSetX_, unsigned int offSetY_);

    void appendPoint(unsigned int roletaEtap, float xNorma, float yNorma, unsigned int measTime);
private:
    void initNorma(unsigned int norma, float * normasrc);

private:
    bool reverseX;
    unsigned int maxEtap;
    unsigned int maxR;
    unsigned int measTime;
    unsigned int stableTime;
    unsigned int width;
    unsigned int offsetX;
    unsigned int offsetY;

    float norma5[5];
    float norma6[6];
    float norma7[7];
    float norma8[8];
    float norma9[9];
    float norma10[10];

    float* norma[6];
};


#endif // POZYCJE_H

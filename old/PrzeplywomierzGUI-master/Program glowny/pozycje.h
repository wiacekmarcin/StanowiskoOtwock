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

#endif // POZYCJE_H

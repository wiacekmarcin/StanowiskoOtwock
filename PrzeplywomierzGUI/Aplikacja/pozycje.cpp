#include "pozycje.h"

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

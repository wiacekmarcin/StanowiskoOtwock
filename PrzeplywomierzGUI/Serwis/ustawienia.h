#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QString>
#include <QSettings>

Q_DECLARE_METATYPE(long double)

#define RolObrotN(N) const QString &getRolObrot##N() const; void setRolObrot##N(const QString &newRolObrot##N);

class Ustawienia
{
public:

    Ustawienia();

    void read();

    void write();



    QString getRatioCzujnik1() const;
    void setRatioCzujnik1(QString value);

    QString getUnitCzujnik1() const;
    void setUnitCzujnik1(const QString &value);

    QString getImpulsyXperMM() const;
    void setImpulsyXperMM(const QString &value);

    QString getImpulsyYperMM() const;
    void setImpulsyYperMM(const QString &value);

    QString getKrokiXperMM() const;
    void setKrokiXperMM(const QString &value);

    QString getKrokiYperMM() const;
    void setKrokiYperMM(const QString &value);



    QString getKatnachylenia() const;
    void setKatnachylenia(const QString &value);

    QString getOffsetX() const;
    void setOffsetX(const QString &value);

    QString getOffsetY() const;
    void setOffsetY(const QString &value);



    const QString &getRolDlugosc() const;
    void setRolDlugosc(const QString &newRolDlugosc);

    const QString &getRolStepObrot() const;
    void setRolStepObrot(const QString &newRolImpObrot);


    RolObrotN(1)
    RolObrotN(2)
    RolObrotN(3)
    RolObrotN(4)
    RolObrotN(5)
    RolObrotN(6)
    RolObrotN(7)
    RolObrotN(8)
    RolObrotN(9)
    RolObrotN(10)
    RolObrotN(11)
    RolObrotN(12)
    RolObrotN(13)


private:
    QSettings settings;

    static char appnazwa[];
    static char firmnazwa[];

    QString ratioCzujnik1;


    QString unitCzujnik1;

#include "../Aplikacja/ustawienia.h"

};

#endif // USTAWIENIA_H

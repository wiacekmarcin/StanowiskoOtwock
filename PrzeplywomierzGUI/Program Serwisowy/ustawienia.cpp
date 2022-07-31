#include "ustawienia.h"

char Ustawienia::appnazwa[]="Przeplywomierz";
char Ustawienia::firmnazwa[]="PogromcyWiatrow";

Ustawienia::Ustawienia() :
    settings(Ustawienia::firmnazwa, Ustawienia::appnazwa)
{
    read();
}

#define rolReadN(N) rolObrot##N = settings.value("rol_obrot"#N).toString()

void Ustawienia::read()
{

    ratioCzujnik1 = settings.value("ratioczujnik1").toString();

    unitCzujnik1 = settings.value("unitczujnik1").toString();

    impulsyXperMM = settings.value("impulsyXperMM").toString();
    impulsyYperMM = settings.value("impulsyYperMM").toString();

    krokiXperMM = settings.value("krokiXperMM").toString();
    krokiYperMM = settings.value("krokiYperMM").toString();

    katnachylenia = settings.value("katnachylenia").toString();
    offsetX = settings.value("offsetx").toString();
    offsetY = settings.value("offsety").toString();

    rolDlugosc = settings.value("rol_dlugoscMM").toString();
    rolImpObrot = settings.value("rol_impulsyPerObrot").toString();

    rolReadN(1);
    rolReadN(2);
    rolReadN(3);
    rolReadN(4);
    rolReadN(5);
    rolReadN(6);
    rolReadN(7);
    rolReadN(8);
    rolReadN(9);
    rolReadN(10);
    rolReadN(11);
    rolReadN(12);
    rolReadN(13);
}

#define rolWriteN(N) settings.setValue("rol_obrot"#N, QVariant::fromValue(rolObrot##N))

void Ustawienia::write()
{
    settings.setValue("ratioczujnik1", QVariant::fromValue(ratioCzujnik1));

    settings.setValue("unitczujnik1", QVariant::fromValue(unitCzujnik1));

    settings.setValue("impulsyXperMM", QVariant::fromValue(impulsyXperMM));
    settings.setValue("impulsyYperMM", QVariant::fromValue(impulsyYperMM));

    settings.setValue("krokiXperMM", QVariant::fromValue(krokiXperMM));
    settings.setValue("krokiYperMM", QVariant::fromValue(krokiYperMM));

    settings.setValue("katnachylenia", QVariant::fromValue(katnachylenia));
    settings.setValue("offsetx", QVariant::fromValue(offsetX));
    settings.setValue("offsetY", QVariant::fromValue(offsetY));

    settings.setValue("rol_dlugoscMM", QVariant::fromValue(rolDlugosc));
    settings.setValue("rol_impulsyPerObrot", QVariant::fromValue(rolImpObrot));

    rolWriteN(1);
    rolWriteN(2);
    rolWriteN(3);
    rolWriteN(4);
    rolWriteN(5);
    rolWriteN(6);
    rolWriteN(7);
    rolWriteN(8);
    rolWriteN(9);
    rolWriteN(10);
    rolWriteN(11);
    rolWriteN(12);
    rolWriteN(13);

}

QString Ustawienia::getRatioCzujnik1() const
{
    return ratioCzujnik1;
}

void Ustawienia::setRatioCzujnik1(QString value)
{
    ratioCzujnik1 = value;
    settings.setValue("ratioczujnik1", QVariant::fromValue(ratioCzujnik1));
}



QString Ustawienia::getUnitCzujnik1() const
{
    return unitCzujnik1;
}

void Ustawienia::setUnitCzujnik1(const QString &value)
{
    unitCzujnik1 = value;
    settings.setValue("unitczujnik1", QVariant::fromValue(unitCzujnik1));
}

QString Ustawienia::getImpulsyXperMM() const
{
    return impulsyXperMM;
}

void Ustawienia::setImpulsyXperMM(const QString &value)
{
    impulsyXperMM = value;
    settings.setValue("impulsyXperMM", QVariant::fromValue(impulsyXperMM));
}

QString Ustawienia::getImpulsyYperMM() const
{
    return impulsyYperMM;
}

void Ustawienia::setImpulsyYperMM(const QString &value)
{
    impulsyYperMM = value;
    settings.setValue("impulsyYperMM", QVariant::fromValue(impulsyYperMM));
}

QString Ustawienia::getKrokiXperMM() const
{
    return krokiXperMM;
}

void Ustawienia::setKrokiXperMM(const QString &value)
{
    krokiXperMM = value;
    settings.setValue("krokiXperMM", QVariant::fromValue(krokiXperMM));
}

QString Ustawienia::getKrokiYperMM() const
{
    return krokiYperMM;
}

void Ustawienia::setKrokiYperMM(const QString &value)
{
    krokiYperMM = value;
    settings.setValue("krokiYperMM", QVariant::fromValue(krokiYperMM));
}

QString Ustawienia::getKatnachylenia() const
{
    return katnachylenia;
}

void Ustawienia::setKatnachylenia(const QString &value)
{
    katnachylenia = value;
    settings.setValue("katnachylenia", QVariant::fromValue(katnachylenia));
}

QString Ustawienia::getOffsetX() const
{
    return offsetX;
}

void Ustawienia::setOffsetX(const QString &value)
{
    offsetX = value;
    settings.setValue("offsetx", QVariant::fromValue(offsetX));
}

QString Ustawienia::getOffsetY() const
{
    return offsetY;
}

void Ustawienia::setOffsetY(const QString &value)
{
    offsetY = value;
    settings.setValue("offsety", QVariant::fromValue(offsetY));
}

const QString &Ustawienia::getRolDlugosc() const
{
    return rolDlugosc;
}

void Ustawienia::setRolDlugosc(const QString &newRolDlugosc)
{
    rolDlugosc = newRolDlugosc;
    settings.setValue("rol_dlugoscMM", QVariant::fromValue(rolDlugosc));
}

const QString &Ustawienia::getRolImpObrot() const
{
    return rolImpObrot;
}

void Ustawienia::setRolImpObrot(const QString &newRolImpObrot)
{
    rolImpObrot = newRolImpObrot;
    settings.setValue("rol_impulsyPerObrot", QVariant::fromValue(rolImpObrot));
}

#define RolObrotDefN(N) \
const QString &Ustawienia::getRolObrot##N() const \
{\
    return rolObrot##N;\
}\
void Ustawienia::setRolObrot##N(const QString &newRolObrot##N)\
{\
    rolObrot##N = newRolObrot##N;\
    settings.setValue("rol_obrot"#N, QVariant::fromValue(rolObrot##N));\
}\

RolObrotDefN(1)
RolObrotDefN(2)
RolObrotDefN(3)
RolObrotDefN(4)
RolObrotDefN(5)
RolObrotDefN(6)
RolObrotDefN(7)
RolObrotDefN(8)
RolObrotDefN(9)
RolObrotDefN(10)
RolObrotDefN(11)
RolObrotDefN(12)
RolObrotDefN(13)


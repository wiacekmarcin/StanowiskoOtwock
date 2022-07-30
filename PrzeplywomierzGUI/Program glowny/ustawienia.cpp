#include "ustawienia.h"

char Ustawienia::appnazwa[]="Przeplywomierz";
char Ustawienia::firmnazwa[]="PogromcyWiatrow";

Ustawienia::Ustawienia() :
    settings(Ustawienia::firmnazwa, Ustawienia::appnazwa)
{
    read();
}

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

}

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



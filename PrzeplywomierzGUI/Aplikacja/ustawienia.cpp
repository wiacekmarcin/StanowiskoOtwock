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
    wentOffsetX = settings.value("offsetx").toString();
    wentOffsetY = settings.value("offsety").toString();

    rolDlugosc = settings.value("rol_dlugoscMM").toString();
    rolStepObrot = settings.value("rol_stepsPerObrot").toString();
    rolkrokiperMM = settings.value("rol_krokiperMM").toString();

    for (short i = 0; i < 20; i++) {
        rolObrot[i] = settings.value(QString("rol_obrot")+QString::number(i)).toString();
    }

    rolOffsetX = settings.value("roleta_offset_x").toString();
    rolOffsetY = settings.value("roleta_offset_y").toString();

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
    settings.setValue("offsetx", QVariant::fromValue(wentOffsetX));
    settings.setValue("offsety", QVariant::fromValue(wentOffsetY));

    settings.setValue("rol_dlugoscMM", QVariant::fromValue(rolDlugosc));
    settings.setValue("rol_stepsPerObrot", QVariant::fromValue(rolStepObrot));
    settings.setValue("rol_krokiperMM", QVariant::fromValue(rolkrokiperMM));

    for (short i = 0; i < 20; i++) {
        settings.setValue(QString("rol_obrot")+QString::number(i), QVariant::fromValue(rolObrot[i]));
    }

    settings.setValue("rol_offset_x", QVariant::fromValue(rolOffsetX));
    settings.setValue("rol_offset_y", QVariant::fromValue(rolOffsetY));

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

QString Ustawienia::getWentOffsetX() const
{
    return wentOffsetX;
}

void Ustawienia::setWentOffsetX(const QString &value)
{
    wentOffsetX = value;
    settings.setValue("offsetx", QVariant::fromValue(wentOffsetX));
}

QString Ustawienia::getWentOffsetY() const
{
    return wentOffsetY;
}

void Ustawienia::setWentOffsetY(const QString &value)
{
    wentOffsetY = value;
    settings.setValue("offsety", QVariant::fromValue(wentOffsetY));
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

const QString &Ustawienia::getRolStepObrot() const
{
    return rolStepObrot;
}

void Ustawienia::setRolStepObrot(const QString &newRolImpObrot)
{
    rolStepObrot = newRolImpObrot;
    settings.setValue("rol_stepsPerObrot", QVariant::fromValue(rolStepObrot));
}

QString Ustawienia::getRolOffsetX() const
{
    return rolOffsetX;
}

void Ustawienia::setRolOffsetX(const QString &value)
{
    rolOffsetX = value;
    settings.setValue("roleta_offset_x", QVariant::fromValue(rolOffsetX));
}

QString Ustawienia::getRolOffsetY() const
{
    return rolOffsetY;
}

void Ustawienia::setRolOffsetY(const QString &value)
{
    rolOffsetY = value;
    settings.setValue("roleta_offset_y", QVariant::fromValue(rolOffsetY));
}

const QString &Ustawienia::getRolkrokiperMM() const
{
    return rolkrokiperMM;
}

void Ustawienia::setRolkrokiperMM(const QString &newRolkrokiperMM)
{
    rolkrokiperMM = newRolkrokiperMM;
    settings.setValue("rol_krokiperMM", QVariant::fromValue(rolkrokiperMM));
}

const QString &Ustawienia::getRolObrot(unsigned short obr) const
{
    return rolObrot[obr];
}

void Ustawienia::setRolObrot(unsigned short obr, const QString &newRolObrot)
{
    rolObrot[obr] = newRolObrot;
    settings.setValue(QString("rol_obrot")+QString::number(obr), QVariant::fromValue(rolObrot[obr]));
}





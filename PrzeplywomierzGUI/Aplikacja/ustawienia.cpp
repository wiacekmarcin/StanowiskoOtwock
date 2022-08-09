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

    rolObrot1 = settings.value("rol_obrot1").toString();
    rolObrot2 = settings.value("rol_obrot2").toString();
    rolObrot3 = settings.value("rol_obrot3").toString();
    rolObrot4 = settings.value("rol_obrot4").toString();
    rolObrot5 = settings.value("rol_obrot5").toString();
    rolObrot6 = settings.value("rol_obrot6").toString();
    rolObrot7 = settings.value("rol_obrot7").toString();
    rolObrot8 = settings.value("rol_obrot8").toString();
    rolObrot9 = settings.value("rol_obrot9").toString();
    rolObrot10 = settings.value("rol_obrot10").toString();
    rolObrot11 = settings.value("rol_obrot11").toString();
    rolObrot12 = settings.value("rol_obrot12").toString();
    rolObrot13 = settings.value("rol_obrot13").toString();


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

    settings.setValue("rol_obrot1", QVariant::fromValue(rolObrot1));
    settings.setValue("rol_obrot2", QVariant::fromValue(rolObrot2));
    settings.setValue("rol_obrot3", QVariant::fromValue(rolObrot3));
    settings.setValue("rol_obrot4", QVariant::fromValue(rolObrot4));
    settings.setValue("rol_obrot5", QVariant::fromValue(rolObrot5));
    settings.setValue("rol_obrot6", QVariant::fromValue(rolObrot6));
    settings.setValue("rol_obrot7", QVariant::fromValue(rolObrot7));
    settings.setValue("rol_obrot8", QVariant::fromValue(rolObrot8));
    settings.setValue("rol_obrot9", QVariant::fromValue(rolObrot9));
    settings.setValue("rol_obrot10", QVariant::fromValue(rolObrot10));
    settings.setValue("rol_obrot11", QVariant::fromValue(rolObrot11));
    settings.setValue("rol_obrot12", QVariant::fromValue(rolObrot12));
    settings.setValue("rol_obrot13", QVariant::fromValue(rolObrot13));

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

const QString &Ustawienia::getRolObrot1() const
{
    return rolObrot1;
}

void Ustawienia::setRolObrot1(const QString &newRolObrot1)
{
    rolObrot1 = newRolObrot1;
    settings.setValue("rol_obrot1", QVariant::fromValue(rolObrot1));
}

const QString &Ustawienia::getRolObrot2() const
{
    return rolObrot2;
}

void Ustawienia::setRolObrot2(const QString &newRolObrot2)
{
    rolObrot2 = newRolObrot2;
    settings.setValue("rol_obrot2", QVariant::fromValue(rolObrot2));
}

const QString &Ustawienia::getRolObrot3() const
{
    return rolObrot3;
}

void Ustawienia::setRolObrot3(const QString &newRolObrot3)
{
    rolObrot3 = newRolObrot3;
    settings.setValue("rol_obrot3", QVariant::fromValue(rolObrot3));
}

const QString &Ustawienia::getRolObrot4() const
{
    return rolObrot4;
}

void Ustawienia::setRolObrot4(const QString &newRolObrot4)
{
    rolObrot4 = newRolObrot4;
    settings.setValue("rol_obrot4", QVariant::fromValue(rolObrot4));
}

const QString &Ustawienia::getRolObrot5() const
{
    return rolObrot5;
}

void Ustawienia::setRolObrot5(const QString &newRolObrot5)
{
    rolObrot5 = newRolObrot5;
    settings.setValue("rol_obrot5", QVariant::fromValue(rolObrot5));
}

const QString &Ustawienia::getRolObrot6() const
{
    return rolObrot6;
}

void Ustawienia::setRolObrot6(const QString &newRolObrot6)
{
    rolObrot6 = newRolObrot6;
    settings.setValue("rol_obrot6", QVariant::fromValue(rolObrot6));
}

const QString &Ustawienia::getRolObrot7() const
{
    return rolObrot7;
}

void Ustawienia::setRolObrot7(const QString &newRolObrot7)
{
    rolObrot7 = newRolObrot7;
    settings.setValue("rol_obrot7", QVariant::fromValue(rolObrot7));
}

const QString &Ustawienia::getRolObrot8() const
{
    return rolObrot8;
}

void Ustawienia::setRolObrot8(const QString &newRolObrot8)
{
    rolObrot8 = newRolObrot8;
    settings.setValue("rol_obrot8", QVariant::fromValue(rolObrot8));
}

const QString &Ustawienia::getRolObrot9() const
{
    return rolObrot9;
}

void Ustawienia::setRolObrot9(const QString &newRolObrot9)
{
    rolObrot9 = newRolObrot9;
    settings.setValue("rol_obrot9", QVariant::fromValue(rolObrot9));
}

const QString &Ustawienia::getRolObrot10() const
{
    return rolObrot10;
}

void Ustawienia::setRolObrot10(const QString &newRolObrot10)
{
    rolObrot10 = newRolObrot10;
    settings.setValue("rol_obrot10", QVariant::fromValue(rolObrot10));
}

const QString &Ustawienia::getRolObrot11() const
{
    return rolObrot11;
}

void Ustawienia::setRolObrot11(const QString &newRolObrot11)
{
    rolObrot11 = newRolObrot11;
    settings.setValue("rol_obrot11", QVariant::fromValue(rolObrot11));
}

const QString &Ustawienia::getRolObrot12() const
{
    return rolObrot12;
}

void Ustawienia::setRolObrot12(const QString &newRolObrot12)
{
    rolObrot12 = newRolObrot12;
    settings.setValue("rol_obrot12", QVariant::fromValue(rolObrot12));
}

const QString &Ustawienia::getRolObrot13() const
{
    return rolObrot13;
}

void Ustawienia::setRolObrot13(const QString &newRolObrot13)
{
    rolObrot13 = newRolObrot13;
    settings.setValue("rol_obrot13", QVariant::fromValue(rolObrot13));
}





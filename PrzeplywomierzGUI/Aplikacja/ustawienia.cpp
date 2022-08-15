#include "ustawienia.h"

char Ustawienia::appnazwa[]="Przeplywomierz";
char Ustawienia::firmnazwa[]="PogromcyWiatrow";

Ustawienia::Ustawienia() :
    settings(Ustawienia::firmnazwa, Ustawienia::appnazwa),
    ratioCzujnik1("0.01"),
    unitCzujnik1("m/s"),
    impulsyXperMM("24.96"),
    impulsyYperMM("24.96"),
    krokiXperMM("100"),
    krokiYperMM("100"),
    katnachylenia("3.85"),
    wentOffsetX("301"),
    wentOffsetY("175.58"),
    rolDlugosc("1600"),
    rolStepObrot("12000"),
    rolOffsetX_P("115"),
    rolOffsetY_P("1172"),
    rolOffsetX_L("1050"),
    rolOffsetY_L("1172"),
    rolkrokiperMM("120"),
    normaRoletaWidth("875"),
    normaRoletaHeight("1500")
{
    read();
}

void Ustawienia::read()
{

    ratioCzujnik1 = settings.value("ratioczujnik1", ratioCzujnik1).toString();

    unitCzujnik1 = settings.value("unitczujnik1", unitCzujnik1).toString();

    impulsyXperMM = settings.value("impulsyXperMM", impulsyXperMM).toString();
    impulsyYperMM = settings.value("impulsyYperMM", impulsyYperMM).toString();

    krokiXperMM = settings.value("krokiXperMM", krokiXperMM).toString();
    krokiYperMM = settings.value("krokiYperMM", krokiYperMM).toString();

    katnachylenia = settings.value("katnachylenia", katnachylenia).toString();
    wentOffsetX = settings.value("offsetx", wentOffsetX).toString();
    wentOffsetY = settings.value("offsety", wentOffsetY).toString();

    rolDlugosc = settings.value("rol_dlugoscMM", rolDlugosc).toString();
    rolStepObrot = settings.value("rol_stepsPerObrot", rolStepObrot).toString();
    rolkrokiperMM = settings.value("rol_krokiperMM", rolkrokiperMM).toString();

    for (short i = 0; i < maxObrot; i++) {
        rolObrot[i] = settings.value(QString("rol_obrot")+QString::number(i)).toString();
    }

    rolOffsetX_P = settings.value("roleta_offset_x_p", rolOffsetX_P).toString();
    rolOffsetY_P = settings.value("roleta_offset_y_p", rolOffsetY_P).toString();

    rolOffsetX_P = settings.value("roleta_offset_x_l", rolOffsetX_P).toString();
    rolOffsetY_P = settings.value("roleta_offset_y_l", rolOffsetY_P).toString();

    normaRoletaWidth = settings.value("rroletawidth", normaRoletaWidth).toString();
    normaRoletaHeight = settings.value("roletaheight", normaRoletaHeight).toString();

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

    for (short i = 0; i < maxObrot; i++) {
        settings.setValue(QString("rol_obrot")+QString::number(i), QVariant::fromValue(rolObrot[i]));
    }

    settings.setValue("rol_offset_x_p", QVariant::fromValue(rolOffsetX_P));
    settings.setValue("rol_offset_y_p", QVariant::fromValue(rolOffsetY_P));

    settings.setValue("rol_offset_x_l", QVariant::fromValue(rolOffsetX_L));
    settings.setValue("rol_offset_y_l", QVariant::fromValue(rolOffsetY_L));

    settings.setValue("rroletawidth", QVariant::fromValue(normaRoletaWidth));
    settings.setValue("roletaheight", QVariant::fromValue(normaRoletaHeight));

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

QString Ustawienia::getRolOffsetX_P() const
{
    return rolOffsetX_P;
}

void Ustawienia::setRolOffsetX_P(const QString &value)
{
    rolOffsetX_P = value;
    settings.setValue("roleta_offset_x_p", QVariant::fromValue(rolOffsetX_P));
}

QString Ustawienia::getRolOffsetY_P() const
{
    return rolOffsetY_P;
}

void Ustawienia::setRolOffsetY_P(const QString &value)
{
    rolOffsetY_P = value;
    settings.setValue("roleta_offset_y_p", QVariant::fromValue(rolOffsetY_P));
}

QString Ustawienia::getRolOffsetX_L() const
{
    return rolOffsetX_L;
}

void Ustawienia::setRolOffsetX_L(const QString &value)
{
    rolOffsetX_L = value;
    settings.setValue("roleta_offset_x_l", QVariant::fromValue(rolOffsetX_L));
}

QString Ustawienia::getRolOffsetY_L() const
{
    return rolOffsetY_L;
}

void Ustawienia::setRolOffsetY_L(const QString &value)
{
    rolOffsetY_L = value;
    settings.setValue("roleta_offset_y_l", QVariant::fromValue(rolOffsetY_L));
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

const QString &Ustawienia::getNormaRoletaWidth() const
{
    return normaRoletaWidth;
}

void Ustawienia::setNormaRoletaWidth(const QString &newNormaRoletaWidth)
{
    normaRoletaWidth = newNormaRoletaWidth;
    settings.setValue("roletawidth", QVariant::fromValue(normaRoletaWidth));
}

const QString &Ustawienia::getNormaRoletaHeight() const
{
    return normaRoletaHeight;
}

void Ustawienia::setNormaRoletaHeight(const QString &newNormaRoletaHeight)
{
    normaRoletaHeight = newNormaRoletaHeight;
    settings.setValue("roletaheight", QVariant::fromValue(normaRoletaHeight));
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


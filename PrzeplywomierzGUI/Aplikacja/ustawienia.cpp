#include "ustawienia.h"

char Ustawienia::appnazwa[]="Przeplywomierz";
char Ustawienia::firmnazwa[]="Kolodziejczyk";

int defObroty[] = {80,78,83,91,91,95,106,102,105,116,115,120,126,127};

#define K_ratioczujnik1 "czujniki_ratio1"
#define K_unitczujnik1 "czyjniki_unit1"
#define K_impulsyXperMM "silniki_impulsyXperMM"
#define K_impulsyYperMM "silniki_impulsyYperMM"
#define K_krokiXperMM "silniki_krokiXperMM"
#define K_krokiYperMM "silniki_krokiYperMM"
#define K_katnachylenia "wentylator_katnachylenia"
#define K_offsetx "wentylator_offset_x"
#define K_offsety "wentylator_offset_y"
#define K_rol_dlugoscMM "roleta_dlugoscMM"
#define K_rol_stepsPerObrot "roleta_stepsPerObrot"
#define K_rol_krokiperMM "roleta_krokiperMM"
#define K_rol_obrot "roleta_z_obrot"
#define K_rol_offset_x_p "roleta_offset_x_p"
#define K_rol_offset_y_p "roleta_offset_y_p"
#define K_rol_offset_x_l "roleta_offset_x_l"
#define K_rol_offset_y_l "roleta_offset_y_l"
#define K_rol_width "roleta_width"
#define K_rol_height "roleta_height"
#define K_stacOsXNazwa "wymiary_stacOsXNazwa"
#define K_stacOsYNazwa "wymiary_stacOsYNazwa"
#define K_stacOsXReal "wymiary_stacOsXReal"
#define K_stacOsYReal "wymiary_stacOsYReal"
#define K_oknoOsXNazwa "wymiary_oknoOsXNazwa"
#define K_oknoOsYNazwa "wymiary_oknoOsYNazwa"
#define K_oknoOsXReal "wymiary_oknoOsXReal"
#define K_oknoOsYReal "wymiary_oknoOsYReal"
#define K_rolOsXNazwa "wymiary_rolOsXNazwa"
#define K_rolOsYNazwa "wymiary_rolOsYNazwa"
#define K_rolOsXReal "wymiary_rolOsXReal"
#define K_rolOsYReal "wymiary_rolOsYReal"
#define K_rol_maxKroki "rol_maxRolKroki"
#define K_rolMinHomeKroki "rol_minHomeKroki"
#define K_rol_speedHome "rol_speedHome"
#define K_rol_speedPos "rol_speedPos"

static QString number2Digit(short i) {
    QString ret(K_rol_obrot);
    ret += QString("_");
    if (i < 10)
        ret += QString("0");
    ret += QString::number(i);
    return ret;
}

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
    rolDlugosc("1500"),
    rolStepObrot("2000"),
    rolOffsetX_P("115"),
    rolOffsetY_P("1172"),
    rolOffsetX_L("1050"),
    rolOffsetY_L("1720"),
    rolkrokiperMM("19.4"),
    maxRolKroki("29100"),
    normaRoletaWidth("875"),
    normaRoletaHeight("1500"),
    stacOsXNazwa("2700"),
    stacOsYNazwa("3000"),
    stacOsXReal("2700"),
    stacOsYReal("3000"),
    oknoOsXNazwa("1000"),
    oknoOsYNazwa("2000"),
    oknoOsXReal("1000"),
    oknoOsYReal("2000"),
    rolOsXNazwa("800"),
    rolOsYNazwa("1500"),
    rolOsXReal("1200"),
    rolOsYReal("2000"),
    rolMinHomeStep("500"),
    rolSpeedHome("400"),
    rolSpeedPos("1000")


    
{
    int maxDefSize = sizeof(defObroty)/sizeof(int);
    for (int i=0; i < maxObrot; i++) {
        if (i < maxDefSize)
            rolObrot[i] = QString::number(defObroty[i]);
        else
            rolObrot[i] = "0";
    }
    read();
    write();
}

void Ustawienia::read()
{

    ratioCzujnik1 = settings.value(K_ratioczujnik1, ratioCzujnik1).toString();

    unitCzujnik1 = settings.value(K_unitczujnik1, unitCzujnik1).toString();

    impulsyXperMM = settings.value(K_impulsyXperMM, impulsyXperMM).toString();
    impulsyYperMM = settings.value(K_impulsyYperMM, impulsyYperMM).toString();

    krokiXperMM = settings.value(K_krokiXperMM, krokiXperMM).toString();
    krokiYperMM = settings.value(K_krokiYperMM, krokiYperMM).toString();

    katnachylenia = settings.value(K_katnachylenia, katnachylenia).toString();
    wentOffsetX = settings.value(K_offsetx, wentOffsetX).toString();
    wentOffsetY = settings.value(K_offsety, wentOffsetY).toString();

    rolDlugosc = settings.value(K_rol_dlugoscMM, rolDlugosc).toString();
    rolStepObrot = settings.value(K_rol_stepsPerObrot, rolStepObrot).toString();
    rolkrokiperMM = settings.value(K_rol_krokiperMM, rolkrokiperMM).toString();
    maxRolKroki = settings.value(K_rol_maxKroki, maxRolKroki).toString();

    for (short i = 0; i < maxObrot; i++) {
        rolObrot[i] = settings.value(number2Digit(i), rolObrot[i]).toString();
    }

    rolOffsetX_P = settings.value(K_rol_offset_x_p, rolOffsetX_P).toString();
    rolOffsetY_P = settings.value(K_rol_offset_y_p, rolOffsetY_P).toString();

    rolOffsetX_L = settings.value(K_rol_offset_x_l, rolOffsetX_L).toString();
    rolOffsetY_L = settings.value(K_rol_offset_y_l, rolOffsetY_L).toString();

    normaRoletaWidth = settings.value(K_rol_width, normaRoletaWidth).toString();
    normaRoletaHeight = settings.value(K_rol_height, normaRoletaHeight).toString();

    stacOsXNazwa = settings.value(K_stacOsXNazwa, stacOsXNazwa).toString();
    stacOsYNazwa = settings.value(K_stacOsYNazwa, stacOsYNazwa).toString();
    stacOsXReal = settings.value(K_stacOsXReal, stacOsXReal).toString();
    stacOsYReal = settings.value(K_stacOsYReal, stacOsYReal).toString();
    oknoOsXNazwa = settings.value(K_oknoOsXNazwa, oknoOsXNazwa).toString();
    oknoOsYNazwa = settings.value(K_oknoOsYNazwa, oknoOsYNazwa).toString();
    oknoOsXReal = settings.value(K_oknoOsXReal, oknoOsXReal).toString();
    oknoOsYReal = settings.value(K_oknoOsYReal, oknoOsYReal).toString();
    rolOsXNazwa = settings.value(K_rolOsXNazwa, rolOsXNazwa).toString();
    rolOsYNazwa = settings.value(K_rolOsYNazwa, rolOsYNazwa).toString();
    rolOsXReal = settings.value(K_rolOsXReal, rolOsXReal).toString();
    rolOsYReal = settings.value(K_rolOsYReal, rolOsYReal).toString();

    rolMinHomeStep = settings.value(K_rolMinHomeKroki, rolMinHomeStep).toString();
    rolSpeedHome = settings.value(K_rol_speedHome, rolSpeedHome).toString();
    rolSpeedPos = settings.value(K_rol_speedPos, rolSpeedPos).toString();
}



void Ustawienia::write()
{
    settings.setValue(K_ratioczujnik1, QVariant::fromValue(ratioCzujnik1));
    settings.setValue(K_unitczujnik1, QVariant::fromValue(unitCzujnik1));

    settings.setValue(K_impulsyXperMM, QVariant::fromValue(impulsyXperMM));
    settings.setValue(K_impulsyYperMM, QVariant::fromValue(impulsyYperMM));

    settings.setValue(K_krokiXperMM, QVariant::fromValue(krokiXperMM));
    settings.setValue(K_krokiYperMM, QVariant::fromValue(krokiYperMM));

    settings.setValue(K_katnachylenia, QVariant::fromValue(katnachylenia));
    settings.setValue(K_offsetx, QVariant::fromValue(wentOffsetX));
    settings.setValue(K_offsety, QVariant::fromValue(wentOffsetY));

    settings.setValue(K_rol_dlugoscMM, QVariant::fromValue(rolDlugosc));
    settings.setValue(K_rol_stepsPerObrot, QVariant::fromValue(rolStepObrot));
    settings.setValue(K_rol_krokiperMM, QVariant::fromValue(rolkrokiperMM));
    settings.setValue(K_rol_maxKroki, QVariant::fromValue(maxRolKroki));
    for (short i = 0; i < maxObrot; i++) {
        settings.setValue(number2Digit(i), QVariant::fromValue(rolObrot[i]));
    }

    settings.setValue(K_rol_offset_x_p, QVariant::fromValue(rolOffsetX_P));
    settings.setValue(K_rol_offset_y_p, QVariant::fromValue(rolOffsetY_P));

    settings.setValue(K_rol_offset_x_l, QVariant::fromValue(rolOffsetX_L));
    settings.setValue(K_rol_offset_y_l, QVariant::fromValue(rolOffsetY_L));

    settings.setValue(K_rol_width, QVariant::fromValue(normaRoletaWidth));
    settings.setValue(K_rol_height, QVariant::fromValue(normaRoletaHeight));

    settings.setValue(K_stacOsXNazwa, QVariant::fromValue(stacOsXNazwa));
    settings.setValue(K_stacOsYNazwa, QVariant::fromValue(stacOsYNazwa));
    settings.setValue(K_stacOsXReal, QVariant::fromValue(stacOsXReal));
    settings.setValue(K_stacOsYReal, QVariant::fromValue(stacOsYReal));
    settings.setValue(K_oknoOsXNazwa, QVariant::fromValue(oknoOsXNazwa));
    settings.setValue(K_oknoOsYNazwa, QVariant::fromValue(oknoOsYNazwa));
    settings.setValue(K_oknoOsXReal, QVariant::fromValue(oknoOsXReal));
    settings.setValue(K_oknoOsYReal, QVariant::fromValue(oknoOsYReal));
    settings.setValue(K_rolOsXNazwa, QVariant::fromValue(rolOsXNazwa));
    settings.setValue(K_rolOsYNazwa, QVariant::fromValue(rolOsYNazwa));
    settings.setValue(K_rolOsXReal, QVariant::fromValue(rolOsXReal));
    settings.setValue(K_rolOsYReal, QVariant::fromValue(rolOsYReal));

    settings.setValue(K_rolMinHomeKroki, QVariant::fromValue(rolMinHomeStep));
    settings.setValue(K_rol_speedHome, QVariant::fromValue(rolSpeedHome));
    settings.setValue(K_rol_speedPos, QVariant::fromValue(rolSpeedPos));
}

QString Ustawienia::getRatioCzujnik1() const
{
    return ratioCzujnik1;
}

void Ustawienia::setRatioCzujnik1(QString value)
{
    ratioCzujnik1 = value;
    settings.setValue(K_ratioczujnik1, QVariant::fromValue(ratioCzujnik1));
}

QString Ustawienia::getUnitCzujnik1() const
{
    return unitCzujnik1;
}

void Ustawienia::setUnitCzujnik1(const QString &value)
{
    unitCzujnik1 = value;
    settings.setValue(K_unitczujnik1, QVariant::fromValue(unitCzujnik1));
}

QString Ustawienia::getImpulsyXperMM() const
{
    return impulsyXperMM;
}

void Ustawienia::setImpulsyXperMM(const QString &value)
{
    impulsyXperMM = value;
    settings.setValue(K_impulsyXperMM, QVariant::fromValue(impulsyXperMM));
}

QString Ustawienia::getImpulsyYperMM() const
{
    return impulsyYperMM;
}

void Ustawienia::setImpulsyYperMM(const QString &value)
{
    impulsyYperMM = value;
    settings.setValue(K_impulsyYperMM, QVariant::fromValue(impulsyYperMM));
}

QString Ustawienia::getKrokiXperMM() const
{
    return krokiXperMM;
}

void Ustawienia::setKrokiXperMM(const QString &value)
{
    krokiXperMM = value;
    settings.setValue(K_krokiXperMM, QVariant::fromValue(krokiXperMM));
}

QString Ustawienia::getKrokiYperMM() const
{
    return krokiYperMM;
}

void Ustawienia::setKrokiYperMM(const QString &value)
{
    krokiYperMM = value;
    settings.setValue(K_krokiYperMM, QVariant::fromValue(krokiYperMM));
}

QString Ustawienia::getKatnachylenia() const
{
    return katnachylenia;
}

void Ustawienia::setKatnachylenia(const QString &value)
{
    katnachylenia = value;
    settings.setValue(K_katnachylenia, QVariant::fromValue(katnachylenia));
}

QString Ustawienia::getWentOffsetX() const
{
    return wentOffsetX;
}

void Ustawienia::setWentOffsetX(const QString &value)
{
    wentOffsetX = value;
    settings.setValue(K_offsetx, QVariant::fromValue(wentOffsetX));
}

QString Ustawienia::getWentOffsetY() const
{
    return wentOffsetY;
}

void Ustawienia::setWentOffsetY(const QString &value)
{
    wentOffsetY = value;
    settings.setValue(K_offsety, QVariant::fromValue(wentOffsetY));
}

const QString &Ustawienia::getRolDlugosc() const
{
    return rolDlugosc;
}

void Ustawienia::setRolDlugosc(const QString &newRolDlugosc)
{
    rolDlugosc = newRolDlugosc;
    settings.setValue(K_rol_dlugoscMM, QVariant::fromValue(rolDlugosc));
}

const QString &Ustawienia::getRolStepObrot() const
{
    return rolStepObrot;
}

void Ustawienia::setRolStepObrot(const QString &newRolImpObrot)
{
    rolStepObrot = newRolImpObrot;
    settings.setValue(K_rol_stepsPerObrot, QVariant::fromValue(rolStepObrot));
}

QString Ustawienia::getRolOffsetX_P() const
{
    return rolOffsetX_P;
}

void Ustawienia::setRolOffsetX_P(const QString &value)
{
    rolOffsetX_P = value;
    settings.setValue(K_rol_offset_x_p, QVariant::fromValue(rolOffsetX_P));
}

QString Ustawienia::getRolOffsetY_P() const
{
    return rolOffsetY_P;
}

void Ustawienia::setRolOffsetY_P(const QString &value)
{
    rolOffsetY_P = value;
    settings.setValue(K_rol_offset_y_p, QVariant::fromValue(rolOffsetY_P));
}

QString Ustawienia::getRolOffsetX_L() const
{
    return rolOffsetX_L;
}

void Ustawienia::setRolOffsetX_L(const QString &value)
{
    rolOffsetX_L = value;
    settings.setValue(K_rol_offset_x_l, QVariant::fromValue(rolOffsetX_L));
}

QString Ustawienia::getRolOffsetY_L() const
{
    return rolOffsetY_L;
}

void Ustawienia::setRolOffsetY_L(const QString &value)
{
    rolOffsetY_L = value;
    settings.setValue(K_rol_offset_y_l, QVariant::fromValue(rolOffsetY_L));
}

const QString &Ustawienia::getRolkrokiperMM() const
{
    return rolkrokiperMM;
}

void Ustawienia::setRolkrokiperMM(const QString &newRolkrokiperMM)
{
    rolkrokiperMM = newRolkrokiperMM;
    settings.setValue(K_rol_krokiperMM, QVariant::fromValue(rolkrokiperMM));
}

const QString &Ustawienia::getNormaRoletaWidth() const
{
    return normaRoletaWidth;
}

void Ustawienia::setNormaRoletaWidth(const QString &newNormaRoletaWidth)
{
    normaRoletaWidth = newNormaRoletaWidth;
    settings.setValue(K_rol_width, QVariant::fromValue(normaRoletaWidth));
}

const QString &Ustawienia::getNormaRoletaHeight() const
{
    return normaRoletaHeight;
}

void Ustawienia::setNormaRoletaHeight(const QString &newNormaRoletaHeight)
{
    normaRoletaHeight = newNormaRoletaHeight;
    settings.setValue(K_rol_height, QVariant::fromValue(normaRoletaHeight));
}

const QString &Ustawienia::getStacOsXNazwa() const
{
    return stacOsXNazwa;
}

void Ustawienia::setStacOsXNazwa(const QString &newStacOsXNazwa)
{
    stacOsXNazwa = newStacOsXNazwa;
    settings.setValue(K_stacOsXNazwa, stacOsXNazwa);
}

const QString &Ustawienia::getStacOsYNazwa() const
{
    return stacOsYNazwa;
}

void Ustawienia::setStacOsYNazwa(const QString &newStacOsYNazwa)
{
    stacOsYNazwa = newStacOsYNazwa;
    settings.setValue(K_stacOsYNazwa, stacOsYNazwa);
}

const QString &Ustawienia::getStacOsXReal() const
{
    return stacOsXReal;
}

void Ustawienia::setStacOsXReal(const QString &newStacOsXReal)
{
    stacOsXReal = newStacOsXReal;
    settings.setValue(K_stacOsXReal, stacOsXReal);
}

const QString &Ustawienia::getStacOsYReal() const
{
    return stacOsYReal;
}

void Ustawienia::setStacOsYReal(const QString &newStacOsYReal)
{
    stacOsYReal = newStacOsYReal;
    settings.setValue(K_stacOsYReal, stacOsYReal);
}

const QString &Ustawienia::getOknoOsXNazwa() const
{
    return oknoOsXNazwa;
}

void Ustawienia::setOknoOsXNazwa(const QString &newOknoOsXNazwa)
{
    oknoOsXNazwa = newOknoOsXNazwa;
    settings.setValue(K_oknoOsXNazwa, oknoOsXNazwa);
}

const QString &Ustawienia::getOknoOsYNazwa() const
{
    return oknoOsYNazwa;
}

void Ustawienia::setOknoOsYNazwa(const QString &newOknoOsYNazwa)
{
    oknoOsYNazwa = newOknoOsYNazwa;
    settings.setValue(K_oknoOsYNazwa, oknoOsYNazwa);
}

const QString &Ustawienia::getOknoOsXReal() const
{
    return oknoOsXReal;
}

void Ustawienia::setOknoOsXReal(const QString &newOknoOsXReal)
{
    oknoOsXReal = newOknoOsXReal;
    settings.setValue(K_oknoOsXReal, oknoOsXReal);
}

const QString &Ustawienia::getOknoOsYReal() const
{
    return oknoOsYReal;
}

void Ustawienia::setOknoOsYReal(const QString &newOknoOsYReal)
{
    oknoOsYReal = newOknoOsYReal;
    settings.setValue(K_oknoOsYReal, oknoOsYReal);
}

const QString &Ustawienia::getRolOsXNazwa() const
{
    return rolOsXNazwa;
}

void Ustawienia::setRolOsXNazwa(const QString &newRolOsXNazwa)
{
    rolOsXNazwa = newRolOsXNazwa;
    settings.setValue(K_rolOsXNazwa, rolOsXNazwa);
}

const QString &Ustawienia::getRolOsYNazwa() const
{
    return rolOsYNazwa;
}

void Ustawienia::setRolOsYNazwa(const QString &newRolOsYNazwa)
{
    rolOsYNazwa = newRolOsYNazwa;
    settings.setValue(K_rolOsYNazwa, rolOsYNazwa);
}

const QString &Ustawienia::getRolOsXReal() const
{
    return rolOsXReal;
}

void Ustawienia::setRolOsXReal(const QString &newRolOsXReal)
{
    rolOsXReal = newRolOsXReal;
    settings.setValue(K_rolOsXReal, rolOsXReal);
}

const QString &Ustawienia::getRolOsYReal() const
{
    return rolOsYReal;
}

void Ustawienia::setRolOsYReal(const QString &newRolOsYReal)
{
    rolOsYReal = newRolOsYReal;
    settings.setValue(K_rolOsYReal, rolOsYReal);
}

const QString &Ustawienia::getMaxRolKroki() const
{
    return maxRolKroki;
}

void Ustawienia::setMaxRolKroki(const QString &newMaxRolKroki)
{
    maxRolKroki = newMaxRolKroki;
    settings.setValue(K_rol_maxKroki, QVariant::fromValue(maxRolKroki));
}

QString Ustawienia::getRolMinHomeStep() const
{
    return rolMinHomeStep;
}

void Ustawienia::setRolMinHomeStep(const QString &value)
{
    rolMinHomeStep = value;
    settings.setValue(K_rolMinHomeKroki, QVariant::fromValue(rolMinHomeStep));
}

QString Ustawienia::getRolSpeedHomeStep() const
{
    return rolSpeedHome;
}

void Ustawienia::setRolSpeedHomeStep(const QString &value)
{
    rolSpeedHome = value;
    settings.setValue(K_rol_speedHome, QVariant::fromValue(rolSpeedHome));
}

QString Ustawienia::getRolSpeedPosStep() const
{
    return rolSpeedPos;
}

void Ustawienia::setRolSpeedPosStep(const QString &value)
{
    rolSpeedPos = value;
    settings.setValue(K_rol_speedPos, QVariant::fromValue(rolSpeedPos));
}

const QString &Ustawienia::getRolObrot(unsigned short obr) const
{
    return rolObrot[obr];
}

void Ustawienia::setRolObrot(unsigned short obr, const QString &newRolObrot)
{
    rolObrot[obr] = newRolObrot;
    settings.setValue(number2Digit(obr), QVariant::fromValue(rolObrot[obr]));
}


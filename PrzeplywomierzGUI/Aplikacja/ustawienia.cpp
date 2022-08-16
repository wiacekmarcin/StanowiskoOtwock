#include "ustawienia.h"

char Ustawienia::appnazwa[]="Przeplywomierz";
char Ustawienia::firmnazwa[]="PogromcyWiatrow";


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
    rolDlugosc("1600"),
    rolStepObrot("12000"),
    rolOffsetX_P("115"),
    rolOffsetY_P("1172"),
    rolOffsetX_L("1050"),
    rolOffsetY_L("1720"),
    rolkrokiperMM("120"),
    normaRoletaWidth("875"),
    normaRoletaHeight("1500")
{
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

    for (short i = 0; i < maxObrot; i++) {
        rolObrot[i] = settings.value(number2Digit(i)).toString();
    }

    rolOffsetX_P = settings.value(K_rol_offset_x_p, rolOffsetX_P).toString();
    rolOffsetY_P = settings.value(K_rol_offset_y_p, rolOffsetY_P).toString();

    rolOffsetX_P = settings.value(K_rol_offset_x_l, rolOffsetX_L).toString();
    rolOffsetY_P = settings.value(K_rol_offset_y_l, rolOffsetY_L).toString();

    normaRoletaWidth = settings.value(K_rol_width, normaRoletaWidth).toString();
    normaRoletaHeight = settings.value(K_rol_height, normaRoletaHeight).toString();

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

    for (short i = 0; i < maxObrot; i++) {
        settings.setValue(number2Digit(i), QVariant::fromValue(rolObrot[i]));
    }

    settings.setValue(K_rol_offset_x_p, QVariant::fromValue(rolOffsetX_P));
    settings.setValue(K_rol_offset_y_p, QVariant::fromValue(rolOffsetY_P));

    settings.setValue(K_rol_offset_x_l, QVariant::fromValue(rolOffsetX_L));
    settings.setValue(K_rol_offset_y_l, QVariant::fromValue(rolOffsetY_L));

    settings.setValue(K_rol_width, QVariant::fromValue(normaRoletaWidth));
    settings.setValue(K_rol_height, QVariant::fromValue(normaRoletaHeight));

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

const QString &Ustawienia::getRolObrot(unsigned short obr) const
{
    return rolObrot[obr];
}

void Ustawienia::setRolObrot(unsigned short obr, const QString &newRolObrot)
{
    rolObrot[obr] = newRolObrot;
    settings.setValue(number2Digit(obr), QVariant::fromValue(rolObrot[obr]));
}


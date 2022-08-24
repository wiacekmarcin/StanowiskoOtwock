#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QObject>
#include <QString>
#include <QSettings>

Q_DECLARE_METATYPE(long double)

class Ustawienia : public QObject
{
    Q_OBJECT

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

    QString getWentOffsetX() const;
    void setWentOffsetX(const QString &value);

    QString getWentOffsetY() const;
    void setWentOffsetY(const QString &value);

    const QString &getRolDlugosc() const;
    void setRolDlugosc(const QString &newRolDlugosc);

    const QString &getRolStepObrot() const;
    void setRolStepObrot(const QString &newRolImpObrot);

    const QString &getRolObrot(unsigned short obr) const;
    void setRolObrot(unsigned short obr, const QString &newRolObrot1);

    QString getRolOffsetX_P() const;
    void setRolOffsetX_P(const QString &value);

    QString getRolOffsetY_P() const;
    void setRolOffsetY_P(const QString &value);

    QString getRolOffsetX_L() const;
    void setRolOffsetX_L(const QString &value);

    QString getRolOffsetY_L() const;
    void setRolOffsetY_L(const QString &value);

    const QString &getRolkrokiperMM() const;
    void setRolkrokiperMM(const QString &newRolkrokiperMM);

    const QString &getNormaRoletaWidth() const;
    void setNormaRoletaWidth(const QString &newNormaRoletaWidth);

    const QString &getNormaRoletaHeight() const;
    void setNormaRoletaHeight(const QString &newNormaRoletaHeight);

    static constexpr int getMaxRolObrot() { return maxObrot; }

    const QString &getStacOsXNazwa() const;
    void setStacOsXNazwa(const QString &newStacOsXNazwa);

    const QString &getStacOsYNazwa() const;
    void setStacOsYNazwa(const QString &newStacOsYNazwa);

    const QString &getStacOsXReal() const;
    void setStacOsXReal(const QString &newStacOsXReal);

    const QString &getStacOsYReal() const;
    void setStacOsYReal(const QString &newStacOsYReal);

    const QString &getOknoOsXNazwa() const;
    void setOknoOsXNazwa(const QString &newOknoOsXNazwa);

    const QString &getOknoOsYNazwa() const;
    void setOknoOsYNazwa(const QString &newOknoOsYNazwa);

    const QString &getOknoOsXReal() const;
    void setOknoOsXReal(const QString &newOknoOsXReal);

    const QString &getOknoOsYReal() const;
    void setOknoOsYReal(const QString &newOknoOsYReal);

    const QString &getRolOsXNazwa() const;
    void setRolOsXNazwa(const QString &newRolOsXNazwa);

    const QString &getRolOsYNazwa() const;
    void setRolOsYNazwa(const QString &newRolOsYNazwa);

    const QString &getRolOsXReal() const;
    void setRolOsXReal(const QString &newRolOsXReal);

    const QString &getRolOsYReal() const;
    void setRolOsYReal(const QString &newRolOsYReal);

    const QString &getMaxRolKroki() const;
    void setMaxRolKroki(const QString &newMaxRolKroki);

    QString getRolMinHomeStep() const;
    void setRolMinHomeStep(const QString &value);

    QString getRolSpeedHomeStep() const;
    void setRolSpeedHomeStep(const QString &value);

    QString getRolSpeedPosStep() const;
    void setRolSpeedPosStep(const QString &value);

private:
    QSettings settings;
    static constexpr int maxObrot = 30;
    static char appnazwa[];
    static char firmnazwa[];

    QString ratioCzujnik1;


    QString unitCzujnik1;

    QString impulsyXperMM;
    QString impulsyYperMM;
    QString krokiXperMM;
    QString krokiYperMM;

    QString katnachylenia;
    QString wentOffsetX;
    QString wentOffsetY;

    QString rolDlugosc;
    QString rolStepObrot;
    QString rolObrot[maxObrot];

    QString rolOffsetX_P;
    QString rolOffsetY_P;
    QString rolOffsetX_L;
    QString rolOffsetY_L;
    QString rolkrokiperMM;
    QString maxRolKroki;

    QString normaRoletaWidth;
    QString normaRoletaHeight;

    QString stacOsXNazwa;
    QString stacOsYNazwa;
    QString stacOsXReal;
    QString stacOsYReal;

    QString oknoOsXNazwa;
    QString oknoOsYNazwa;
    QString oknoOsXReal;
    QString oknoOsYReal;

    QString rolOsXNazwa;
    QString rolOsYNazwa;
    QString rolOsXReal;
    QString rolOsYReal;

    QString rolMinHomeStep;
    QString rolSpeedHome;
    QString rolSpeedPos;

};

#endif // USTAWIENIA_H
